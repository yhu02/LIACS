/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    testing.cc - Testing framework.
 *
 * Copyright (C) 2016,2017,2021  Leiden University, The Netherlands.
 */

#ifndef TESTING_H
#define TESTING_H

#include <string>

#include "arch.h"
#include "config-file.h"

enum ExitCodes : int
{
  Success = 0,
  AbnormalTermination = 1,
  HelpDisplayed = 2,
  InitializationError = 3,
  UnitTestFailed = 5
};

/* Class to parse and store a register initializer pair consisting of
 * a register number and the value to initialize that register with.
 */
class RegisterInit
{
  public:
    RegisterInit(RegNumber number, RegValue value);

    RegisterInit(std::string_view initstr);

    RegNumber number{};
    RegValue value{};
};

/* A test file contains "pre" and "post" sections, containing the values
 * the registers should be initialized with and the values the registers
 * should have at program end respectively. The filename of a test file
 * should end with ".conf". The corresponding executable has the same
 * filename, but with extension ".bin".
 */
class TestFile : public ConfigFile
{
  public:
    TestFile(std::string_view filename);

    std::vector<RegisterInit> getPreRegisters() const;
    std::vector<RegisterInit> getPostRegisters() const;

    /* Return the name of the executable to run given the name of the
     * test file.
     */
    std::string getExecutable() const;

  private:
    std::string filename;

    void validate() const;

    /* Verify that all properties in the given section consist of an
     * register name (rXX) and integer value.
     */
    void validateSection(std::string_view sectionName) const;

    /* Note that this method can only be called after the ConfigFile
     * has been successfully validated.
     */
    std::vector<RegisterInit> getRegisters(std::string_view sectionName) const;
};

#endif /* TESTING_H */
