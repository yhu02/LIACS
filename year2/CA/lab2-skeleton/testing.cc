/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    testing.cc - Testing framework.
 *
 * Copyright (C) 2016,2017,2021  Leiden University, The Netherlands.
 */

#include "testing.h"

#include <regex>

RegisterInit::RegisterInit(RegNumber number, RegValue value)
  : number{ number }, value{ value }
{ }

RegisterInit::RegisterInit(std::string_view initstr)
{
  std::regex init_regex("[rR]([0-9]{1,2})=(0x[0-9]+|[0-9]+)");
  std::match_results<std::string_view::const_iterator> match;

  if (std::regex_match(initstr.begin(), initstr.end(), match, init_regex))
    {
      size_t regnum = std::stoul(match[1]);

      /* Perform a first verification, a real check whether the register
       * number is valid is performed when initializing the register file.
       */
      if (regnum < 0 || regnum >= MaxRegs)
        throw std::out_of_range("Error: register regnum out of range: " +
                                std::to_string(regnum));

      number = regnum;
      value = std::stoull(match[2], nullptr, 0);
    }
}


TestFile::TestFile(std::string_view filename)
  : ConfigFile{ filename }, filename{ filename }
{
  validate();
}

std::vector<RegisterInit>
TestFile::getPreRegisters() const
{
  return getRegisters("pre");
}

std::vector<RegisterInit>
TestFile::getPostRegisters() const
{
  return getRegisters("post");
}

std::string
TestFile::getExecutable() const
{
  std::string ret(filename);
  ret.replace(filename.length() - 5, 5, ".bin");
  return ret;
}

void
TestFile::validate() const
{
  validateSection("pre");
  validateSection("post");
}

void
TestFile::validateSection(std::string_view sectionName) const
{
  if (!hasSection(sectionName))
    throw std::runtime_error{
        "Section '" + std::string{ sectionName } + "' missing." };

  std::regex regnameRegex("r([0-9]{1,2})");
  std::smatch match;

  for (const auto & [prop, value] : getProperties(sectionName))
    {
      if (std::regex_match(prop, match, regnameRegex))
        throw std::runtime_error("Invalid register name " + prop);
    }
}

std::vector<RegisterInit>
TestFile::getRegisters(std::string_view sectionName) const
{
  std::vector<RegisterInit> result;

  /* Translate all properties into register init pairs. Note that
   * kv.first stores the register *name*, so we need to skip the R.
   * validateSection already validated that the register name is in
   * the right form.
   */
  for (const auto & [prop, value] : getProperties(sectionName))
    {
      result.emplace_back(std::stoi(prop.c_str() + 1),
                          std::stoull(value, nullptr, 0));
    }

  return result;
}
