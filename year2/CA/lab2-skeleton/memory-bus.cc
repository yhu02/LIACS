/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-bus.h - "Interconnect" for different memory bus clients.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */
#include "unistd.h"
#include "memory-bus.h"

MemoryBus::MemoryBus(std::vector<std::unique_ptr<MemoryInterface> > &&clients)
  : clients{ std::move(clients) }
{
}

MemoryBus::~MemoryBus() = default;

void
MemoryBus::addClient(std::unique_ptr<MemoryInterface> client)
{
  clients.emplace_back(std::move(client));
}

uint64_t
MemoryBus::getBytesRead() const
{
  return bytesRead;
}

uint64_t
MemoryBus::getBytesWritten() const
{
  return bytesWritten;
}


uint8_t
MemoryBus::readByte(MemAddress addr)
{
  bytesRead += 1;
  return getClient(addr)->readByte(addr);
}

uint16_t
MemoryBus::readHalfWord(MemAddress addr)
{
  bytesRead += 2;
  return getClient(addr)->readHalfWord(addr);
}

uint32_t
MemoryBus::readWord(MemAddress addr)
{
  bytesRead += 4;
  return getClient(addr)->readWord(addr);
}

uint64_t
MemoryBus::readDoubleWord(MemAddress addr)
{
  bytesRead += 8;
  return getClient(addr)->readDoubleWord(addr);
}

void
MemoryBus::writeByte(MemAddress addr, uint8_t value)
{
  bytesWritten += 1;
  return getClient(addr)->writeByte(addr, value);
}

void
MemoryBus::writeHalfWord(MemAddress addr, uint16_t value)
{
  bytesWritten += 2;
  return getClient(addr)->writeHalfWord(addr, value);
}

void
MemoryBus::writeWord(MemAddress addr, uint32_t value)
{
  bytesWritten += 4;
  return getClient(addr)->writeWord(addr, value);
}

void
MemoryBus::writeDoubleWord(MemAddress addr, uint64_t value)
{
  bytesWritten += 8;
  return getClient(addr)->writeDoubleWord(addr, value);
}

bool
MemoryBus::contains(MemAddress addr) const
{
  /* dummy */
  return true;
}

void
MemoryBus::clockPulse()
{
  for (auto &client : clients)
    client->clockPulse();
}

/*
 * Private methods
 */
MemoryInterface *
MemoryBus::findClient(MemAddress addr) noexcept
{
  for (auto &client : clients)
    if (client->contains(addr))
      return client.get();

  return nullptr;
}

MemoryInterface *
MemoryBus::getClient(MemAddress addr)
{
  auto *client = findClient(addr);
  if (!client){
    throw IllegalAccess(addr);
  }
  return client;
}
