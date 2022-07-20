/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    config-file.cc - Simple config file parser.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include "config-file.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <algorithm>
#include <regex>

/* All key-value pairs not designated to a section are placed in a
 * section __GLOBAL. Since this name is not accepted by sectionRegex,
 * there cannot be a collision.
 */
static constexpr std::string_view globalSectionName = "__GLOBAL";


ConfigFile::ConfigFile(std::string_view filename)
{
  load(filename);
}

void
ConfigFile::clear()
{
  sections.clear();
  properties.clear();
}

void
ConfigFile::load(std::string_view filename)
{
  clear();

  /* Add default "global" section and make active */
  std::string currentSection{ globalSectionName };
  sections.push_back(currentSection);

  /* Regexes to use while parsing */
  std::regex sectionRegex{ R"(\[([a-zA-Z0-9]+)\]\s*)" };
  std::regex keyValueRegex{ R"(([a-zA-Z]\S*)\s*=\s*(\S+))" };
  std::regex emptyLineRegex{ "^\\s*$" };
  std::smatch match;

  /* Open and parse the file */
  std::ifstream file{ filename.data() };
  if (!file.good())
    throw std::runtime_error("cannot open file " + std::string{ filename });

  std::string line;
  int lineNumber(0);
  while (std::getline(file, line))
    {
      ++lineNumber;

      /* Remove newline characters from string. */
      line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
      line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

      /* Is this a section header? */
      if (std::regex_match(line, match, sectionRegex))
        {
          if (hasSection(match[1].str()))
            throw std::runtime_error(std::string{ filename } + ": line " +
                                     std::to_string(lineNumber) +
                                     ": duplicate definition of section '" +
                                     match[1].str() + "'");

          currentSection = match[1];
          addSection(match[1].str());
        }
      /* A key value pair? */
      else if (std::regex_match(line, match, keyValueRegex))
        {
          if (hasProperty(currentSection, match[1].str()))
            throw std::runtime_error(std::string{ filename } + ": line " +
                                     std::to_string(lineNumber) +
                                     ": duplicate definition of '" +
                                     match[1].str() + "'");

          addProperty(std::make_pair(currentSection, match[1]), match[2].str());
        }
      /* An empty line, perhaps? Otherwise: give up. */
      else if (!std::regex_match(line, match, emptyLineRegex))
        {
          std::cerr << "warning: " << filename
                    << ": cannot parse line " << lineNumber
                    << ", ignoring." << std::endl;
        }
    }

  file.close();
}


bool
ConfigFile::hasSection(std::string_view sectionName) const
{
  return sections.end() != std::find(sections.begin(), sections.end(), sectionName);
}

const std::list<std::string> &
ConfigFile::getSections() const
{
  return sections;
}

bool
ConfigFile::hasProperty(std::string_view sectionName,
                        std::string_view keyName) const
{
  return std::any_of(properties.begin(), properties.end(),
                     [&](const auto &kvp) {
                       return kvp.first.first == sectionName &&
                              kvp.first.second == keyName;
                     });
}

/* Translate properties of a specified section from the internal
 * data structure to a simple vector of key-value pairs.
 */
std::vector<ConfigFile::KeyValue>
ConfigFile::getProperties(std::string_view sectionName) const
{
  std::vector<KeyValue> result;

  for (const auto & [key, val] : properties)
    if (key.first == sectionName)
      result.emplace_back(key.second, val);

  return result;
}


void
ConfigFile::addSection(std::string_view sectionName)
{
  sections.emplace_back(sectionName);
}

void
ConfigFile::addProperty(const PropertyName &propertyName,
                        std::string_view value)
{
  properties.emplace(propertyName, value);
}
