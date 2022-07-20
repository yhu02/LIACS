/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    elf-file.h - ELF file parsing class.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ELF_FILE_H__
#define __ELF_FILE_H__

#include "memory-interface.h"

#include <vector>
#include <memory>
#include <string>

#ifdef _MSC_VER
#include <windows.h>
#endif

/* The ELFFile class loads a program from an ELF file by creating memories
 * for every section that needs to be loaded. During construction of
 * the Processor class, these memories are added to the memory bus
 * of the system.
 */
class ELFFile
{
  public:
    ELFFile(std::string_view filename);
    ~ELFFile();

    void load(std::string_view filename);
    void unload();

    std::vector<std::unique_ptr<MemoryInterface>> createMemories() const;
    uint64_t getEntrypoint() const;


    ELFFile(const ELFFile &) = delete;
    ELFFile &operator=(const ELFFile &) = delete;
    
  private:
#ifdef _MSC_VER
    HANDLE fd{};
    HANDLE mapping{};
#else
    int fd{};
    size_t programSize{};
#endif
    void *mapAddr = nullptr;

    bool isBad = true;

    bool isELF() const;
    bool isTarget(const uint8_t elf_class,
                  const uint8_t endianness,
                  const uint8_t machine) const;
};

#endif /* __ELF_FILE_H__ */
