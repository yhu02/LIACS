/* pagetables -- A framework to experiment with memory management
 *
 *    process.cc - Simple processor abstraction
 *
 * Copyright (C) 2020  Leiden University, The Netherlands.
 */

#include "tracereader.h"
#include "process.h"

std::ostream &
operator<<(std::ostream &stream, const MemAccess &access)
{
  switch (access.type)
    {
      case MemAccessType::Instr:
          stream << "Instr ";
          break;

      case MemAccessType::Store:
          stream << "Store ";
          break;

      case MemAccessType::Load:
          stream << "Load  ";
          break;

      case MemAccessType::Modify:
          stream << "Mod   ";
          break;
    }

  stream << std::hex << std::showbase << access.addr << " "
         << std::dec << std::noshowbase
         << (int)access.size;

  return stream;
}



Process::Process(std::istream &input)
  : reader(std::make_unique<TraceReader>(input))
{
}

Process::~Process()
{
}

uint64_t
Process::getPID(void) const
{
  /* Use the pointer to the Process as PID */
  return reinterpret_cast<const uint64_t>(this);
}

void
Process::getMemoryAccess(MemAccess &access)
{
  *reader >> access;
}

bool
Process::finished(void) const
{
  return reader->eof();
}
