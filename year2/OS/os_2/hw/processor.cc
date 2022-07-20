/* pagetables -- A framework to experiment with memory management
 *
 *    processor.h - Simple processor abstraction
 *
 * Copyright (C) 2018-2020  Leiden University, The Netherlands.
 */

#include "processor.h"

Processor::Processor(MMU &mmu)
  : mmu(mmu), current(nullptr), interruptHandler(nullptr), timerInterval(0)
{ }


void
Processor::setProcess(std::shared_ptr<Process> process) noexcept
{
  current = process;
}

void
Processor::setTimerInterval(const int interval) noexcept
{
  this->timerInterval = interval;
}

void
Processor::setInterruptHandler(InterruptHandler handler) noexcept
{
  interruptHandler = handler;
}

void
Processor::run(void)
{
  if (timerInterval == 0)
    throw std::runtime_error("no timer interval (time quantum) configured by OS kernel.");

  if (!current)
    /* No current process is active, so generate timer interrupt.
     * (Note that the Exit syscall interrupt can never be triggered in
     * case no process is active.)
     */
    interruptHandler(InterruptRequest::Timer);

  while (current)
    {
      int accesses = 0;

      /* We continue to process this trace until we have reached the end or
       * have reached the timer interval for the timer controller.  Since we do
       * not count instructions or time, we approximate the time passed by
       * counting the number of performed memory references.
       */
      while (not current->finished() && accesses < timerInterval)
        {
          MemAccess access;

          current->getMemoryAccess(access);
          mmu.processMemAccess(access);
          ++accesses;
        }

      /* Generate interrupt. */
      if (current->finished())
        /* Process is finished and "has called exit" to request termination */
        interruptHandler(InterruptRequest::SyscallExit);
      else
        interruptHandler(InterruptRequest::Timer);
    }
}
