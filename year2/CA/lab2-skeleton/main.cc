/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    main.cc - Command line parsing, program start.
 *
 * Copyright (C) 2016,2017  Leiden University, The Netherlands.
 */

#include "testing.h"

#include <iostream>
#include <vector>
#include <regex>

#ifdef _MSC_VER
#include "XGetopt.h"
#else
#include <getopt.h>
#endif

#include "elf-file.h"
#include "processor.h"

#ifdef _MSC_VER
/* Defined *somewhere* */
#undef AbnormalTermination
#endif

static bool
validateRegisters(const Processor &p,
				  const std::vector<RegisterInit> &expectedValues)
{
	bool allAsExpected = true;

	if (!expectedValues.empty())
	{
		for (const auto &reginit : expectedValues)
		{
			if (reginit.value != p.getRegister(reginit.number))
			{
				std::cerr << "Register R" << static_cast<int>(reginit.number)
						  << " expected " << reginit.value
						  << " (" << std::hex << std::showbase
						  << reginit.value
						  << std::dec << std::noshowbase << ")"
						  << " got " << p.getRegister(reginit.number)
						  << " (" << std::hex << std::showbase
						  << p.getRegister(reginit.number)
						  << std::dec << std::noshowbase << ")"
						  << std::endl;
				allAsExpected = false;
			}
		}
	}

	return allAsExpected;
}

/* Start the emulator by either executing a test or running a regular
 * program.
 */
static int
launcher(const char *testFilename,
		 const char *execFilename,
		 bool pipelining,
		 int debugMode,
		 std::vector<RegisterInit> initializers)
{
	try
	{
		std::string programFilename;
		std::vector<RegisterInit> postRegisters;

		if (testFilename)
		{
			std::string testConfig(testFilename);

			if (testConfig.length() < 6 or
				testConfig.substr(testConfig.length() - 5) != std::string(".conf"))
			{
				std::cerr << "Error: test filename must end with .conf"
						  << std::endl;
				return ExitCodes::InitializationError;
			}

			try
			{
				TestFile testfile(testConfig);
				initializers = testfile.getPreRegisters();
				postRegisters = testfile.getPostRegisters();
				programFilename = testfile.getExecutable();
			}
			catch (std::exception &e)
			{
				std::cerr << "Error loading test config: " << e.what() << std::endl;
				return ExitCodes::InitializationError;
			}
		}
		else
			programFilename = std::string(execFilename);

		/* Read the ELF file and start the emulator */
		ELFFile program(programFilename);
		Processor p(program, pipelining, debugMode);

		for (auto &initializer : initializers)
			p.initRegister(initializer.number, initializer.value);

		p.run(testFilename != nullptr);

		/* Dump registers and statistics when not running a unit test. */
		if (!testFilename)
		{
			p.dumpRegisters();
			p.dumpStatistics();
		}

		if (!validateRegisters(p, postRegisters))
			return ExitCodes::UnitTestFailed;
	}
	catch (std::runtime_error &e)
	{
		std::cerr << "Couldn't load program: " << e.what()
				  << std::endl;
		return ExitCodes::InitializationError;
	}
	catch (std::out_of_range &e)
	{
		std::cerr << "Out of range parameter: " << e.what()
				  << std::endl;
		return ExitCodes::InitializationError;
	}

	return ExitCodes::Success;
}

static void
showHelp(const char *progName)
{
	std::cerr << progName << " [-d] [-r reginit] <programFilename>" << std::endl;
	std::cerr << std::endl
			  << "    or" << std::endl
			  << std::endl;
	std::cerr << progName << " [-d] -t testfile" << std::endl;
	std::cerr <<
		R"HERE(
    Where 'reginit' is a register initializer in the form
    rX=Y with X a register number and Y the initializer value.
    'testfile' is a unit test configuration file.

    -d enables debug mode in which every decoded instruction is printed
    to the terminal.
)HERE";
}

int main(int argc, char **argv)
{
	char c;
	bool pipelining = false;
	int debugMode = 0;
	std::vector<RegisterInit> initializers;
	const char *testFilename = nullptr;

	/* Command line option processing */
	const char *progName = argv[0];

	while ((c = getopt(argc, argv, "dDpr:t:h")) != -1)
	{
		switch (c)
		{
		case 'd':
			debugMode = 1;
			break;

		case 'D':
			debugMode = 2;
			break;

		case 'p':
			pipelining = true;
			break;

		case 'r':
			if (testFilename != nullptr)
			{
				std::cerr << "Error: Cannot set unit test and individual "
						  << "registers at the same time." << std::endl;
				return ExitCodes::InitializationError;
			}

			try
			{
				RegisterInit init((std::string(optarg)));
				initializers.push_back(init);
			}
			catch (std::exception &)
			{
				std::cerr << "Error: Malformed register initialization specifier "
						  << optarg << std::endl;
				return ExitCodes::InitializationError;
			}
			break;

		case 't':
			if (testFilename != nullptr)
			{
				std::cerr << "Error: Cannot specify testfile more than once."
						  << std::endl;
				return ExitCodes::InitializationError;
			}

			testFilename = optarg;
			break;

		case 'h':
		default:
			showHelp(progName);
			return ExitCodes::HelpDisplayed;
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (!testFilename and argc < 1)
	{
		std::cerr << "Error: No executable specified." << std::endl
				  << std::endl;
		showHelp(progName);
		return ExitCodes::InitializationError;
	}

	return launcher(testFilename, argv[0], pipelining, debugMode, initializers);
}
