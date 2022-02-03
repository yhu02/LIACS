/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    elf-file.cc - ELF file parsing class.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 *
 * This file losely based on:
 *
 * S.M.A.C.K - An operating system kernel
 * Copyright (C) 2010,2011 Mattias Holm and Kristian Rietveld
 * For licensing and a full list of authors of the kernel, see the files
 * COPYING and AUTHORS.
 */

#include "elf-file.h"
#include "memory.h"

#include "elf.h"

#include <stdexcept>

#ifndef _MSC_VER
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#endif

#include <filesystem>
#include <algorithm>
namespace fs = std::filesystem;

ELFFile::ELFFile(std::string_view filename)
{
  load(filename);
}

ELFFile::~ELFFile()
{
  if (! isBad)
    unload();
}

void
ELFFile::load(std::string_view filename)
{
#ifdef _MSC_VER
  if (fs::is_directory(filename)) {
    throw std::runtime_error("Filename is a directory.");
  }

  fd = CreateFileA(filename.data(), GENERIC_READ,
    0, nullptr, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, nullptr);

  if (fd == INVALID_HANDLE_VALUE) {
    throw std::runtime_error("Could not open file.");
  }

  mapping = CreateFileMappingA(fd, nullptr, 
    PAGE_READONLY, 0, 0, nullptr);

  if (mapping == nullptr) {
    CloseHandle(fd);
    throw std::runtime_error("Failed to create memory map.");
  }

  mapAddr = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0,0);
  if (mapAddr == nullptr) {
    CloseHandle(mapping);
    CloseHandle(fd);
    throw std::runtime_error("Failed to setup memory map.");
  }

#else
  fd = open(filename.data(), O_RDONLY);
  if (fd < 0)
    throw std::runtime_error("Could not open file.");

  struct stat statbuf;
  if (fstat(fd, &statbuf) < 0)
    {
      close(fd);
      throw std::runtime_error("Could not retrieve file attributes.");
    }

  if (statbuf.st_mode & S_IFDIR)
    {
      close(fd);
      throw std::runtime_error("Filename is a directory.");
    }

  programSize = statbuf.st_size;
  mapAddr = mmap(NULL, programSize, PROT_READ, MAP_PRIVATE, fd, 0);
  if (mapAddr == MAP_FAILED)
    {
      close(fd);
      throw std::runtime_error("Failed to setup memory map.");
    }
#endif

  /* For now, we hardcode the RISCV target */
  if (! isELF() || ! isTarget(ELFCLASS64, ELFDATA2LSB, EM_RISCV))
    {
      unload();
      throw std::runtime_error("File is not a RISC-V ELF file.");
    }

  isBad = false;
}

void
ELFFile::unload()
{
#ifdef _MSC_VER
  UnmapViewOfFile(mapAddr);
  CloseHandle(mapping);
  CloseHandle(fd);

  mapping = nullptr;
#else
  munmap(mapAddr, programSize);
  close(fd);
#endif

  mapAddr = nullptr;

  /* Select correct default on all platforms */
  fd = decltype(fd){};
}

bool
ELFFile::isELF() const
{
  const auto *elf = static_cast<const Elf64_Ehdr *>(mapAddr);

  if (!(elf->e_ident[EI_MAG0] == 0x7f
        && elf->e_ident[EI_MAG1] == 'E'
        && elf->e_ident[EI_MAG2] == 'L'
        && elf->e_ident[EI_MAG3] == 'F'))
    return false;

  if (elf->e_ident[EI_VERSION] != EV_CURRENT)
    return false;

  return true;
}

bool
ELFFile::isTarget(const uint8_t elf_class,
                  const uint8_t endianness,
                  const uint8_t machine) const
{
  const auto *elf = static_cast<Elf64_Ehdr *>(mapAddr);

  if (elf->e_ident[EI_CLASS] != elf_class)
    return false;

  if (elf->e_ident[EI_DATA] != endianness)
    return false;

  if ((elf->e_type == ET_EXEC || elf->e_type == ET_DYN) &&
      (elf->e_machine != machine || elf->e_version != EV_CURRENT))
    return false;

  if (elf->e_phoff == 0)
    // Not a proper elf executable (must have program header)
    return false;

  return true;
}

std::vector<std::unique_ptr<MemoryInterface>>
ELFFile::createMemories() const
{
  std::vector<std::unique_ptr<MemoryInterface>> memories;

  const auto *elf = static_cast<Elf64_Ehdr *>(mapAddr);
  const auto *sheaders = reinterpret_cast<Elf64_Shdr *>(reinterpret_cast<uintptr_t>(elf) + elf->e_shoff);

  for (int i = 0; i < elf->e_shnum; ++i)
    {
      const Elf64_Shdr &header = sheaders[i];
      if ((header.sh_flags & SHF_ALLOC) == SHF_ALLOC)
        {
          size_t align = header.sh_addralign;

          auto *segment =
              new (std::align_val_t{ align }, std::nothrow) std::byte[header.sh_size];
          if (!segment)
            throw std::runtime_error("Could not allocate aligned memory.");

          if (reinterpret_cast<uintptr_t>(segment) & ((align - 1) != 0))
            throw std::runtime_error("Allocated pointer for segment is not aligned.");

          /* Transfer section data or clear the section. */
          if (header.sh_type == SHT_PROGBITS)
            {
              const auto *segdata =
                  reinterpret_cast<const std::byte *>(elf) + header.sh_offset;
              std::copy_n(segdata, header.sh_size, segment);
            }
          else
            std::fill_n(segment, header.sh_size, std::byte{ 0 });

          /* FIXME: determine correct name for segment. */
          std::string name{ "data" };
          if ((header.sh_flags & SHF_EXECINSTR) == SHF_EXECINSTR)
            name = "text";

          auto memory = std::make_unique<Memory>(name, segment,
                                                 header.sh_addr,
                                                 header.sh_size,
                                                 align);
          if ((header.sh_flags & SHF_WRITE) == SHF_WRITE)
            memory->setMayWrite(true);

          memories.push_back(std::move(memory));
        }
    }

  return memories;
}

uint64_t
ELFFile::getEntrypoint() const
{
  return static_cast<Elf64_Ehdr *>(mapAddr)->e_entry;
}
