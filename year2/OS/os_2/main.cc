/* pagetables -- A framework to experiment with memory management
 *
 *    main.cc - Program driver
 *
 * Copyright (C) 2017,2018  Leiden University, The Netherlands.
 */

#include <iostream>
#include <fstream>

#include <getopt.h>

#include "exceptions.h"
#include "settings.h"

#include "simple.h"
#include "aarch64.h"

uint64_t MemorySize = 1 * 1024 * 1024; /* Default to 1024 * 1024 KiB = 1 GiB */


static void
showHelp(const char *progName)
{
  std::cerr << progName << " [-l] {-s|-a} [-A] [-q quantum] [-m memsize] [-t tlbsize] [filenames ...]" << std::endl;
  std::cerr <<
R"HERE(
    -l           Log all memory accesses to stderr.
    -s           Use "simple" page table.
    -a           Use AArch64 page table.
    -A           Use ASID
    -q quantum   Configure process "time quantum" to quantum.
    -m memsize   Memory size in KiB.
    -t tlbsize   Configure sizeo f TLB (number of entries).

    One of -s or -a must be specified.
    filenames may be one or more files.
)HERE";
}

template<typename M, typename D> static void
run(uint64_t memorySize, ProcessList &processList)
{
  M mmu;
  D driver;

  Processor processor(mmu);
  OSKernel kernel(processor, driver, memorySize, processList);

  processor.run();
}

static void
exitWithError(const char *progName, const char *errorMsg)
{
  std::cerr << errorMsg;
  showHelp(progName);

  exit(-1);
}

int
main(int argc, char **argv)
{
  char c;
  const char *progName = argv[0];

  bool useSimple = false;
  bool useAArch64 = false;

  /* Parse options */
  while ((c = getopt(argc, argv, "lsaAq:hm:t:")) != -1)
    {
      switch (c)
        {
          case 'l':
            LogMemoryAccesses = true;
            break;

          case 's':
            useSimple = true;
            break;

          case 'a':
            useAArch64 = true;
            break;

          case 'A':
            UseASID = true;
            break;

          case 'q':
            ProcessTimeQuantum = std::stoi(optarg);
            break;

          case 'm':
            MemorySize = std::stoull(optarg);
            break;

          case 't':
            TLBEntries = std::stoi(optarg);
            break;

          case 'h':
          default:
            showHelp(progName);
            exit(-1);
        }
    }

  argc -= optind;
  argv += optind;

  if (not useSimple and not useAArch64)
    exitWithError(progName, "Error: no page table type specified.\n\n");

  if ((useSimple + useAArch64) > 1)
    exitWithError(progName, "Error: can only specify one of {-s|-a}.\n\n");

  if (argc == 0)
    exitWithError(progName, "Error: no input files specified.\n\n");

  /* Start main program, catch any exceptions and let the user know. */
  try
    {
      /* Store opened files in a separate list to keep ownership, pass
       * istream references to Processes.
       */
      std::list<std::ifstream> files;
      ProcessList processList;

      for (int i = 0; i < argc; ++i)
        {
          files.emplace_back(std::ifstream(argv[i]));
          if (not files.back())
            throw std::runtime_error("Could not open file " + std::string(argv[i]));
          processList.emplace_back(std::make_shared<Process>(files.back()));
        }

      if (useSimple)
        run<Simple::SimpleMMU, Simple::SimpleMMUDriver>(MemorySize << 10, processList);
      else if (useAArch64)
        run<AArch64::AArch64MMU, AArch64::AArch64MMUDriver>(MemorySize << 10, processList);
      else
        exitWithError(progName, "Error: unknown page table type specified.\n\n");
      
    }
  catch (TraceFileParseError &error)
    {
      std::cerr << "Parse error: " << error.what() << std::endl;
      return -1;
    }
  catch (ReadError &error)
    {
      std::cerr << "Read error: " << error.what() << std::endl;
      return -1;
    }
  catch (std::runtime_error &error)
    {
      std::cerr << std::endl
          << "===> Runtime error: " << error.what() << std::endl << std::endl;
      return -1;
    }
  catch (...)
    {
      std::cerr << "Unknown exception occurred." << std::endl;
      return -1;
    }

  return 0;
}
