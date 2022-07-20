/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    config-file.h - Simple config file parser.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __CONFIG_FILE__
#define __CONFIG_FILE__

#include <string>

#include <vector>
#include <list>
#include <map>

class ConfigFile
{
  public:
    using KeyValue = std::pair<std::string, std::string>;

    ConfigFile(std::string_view filename);

    void load(std::string_view filename);

    bool hasSection(std::string_view sectionName) const;
    const std::list<std::string> &getSections() const;

    bool hasProperty(std::string_view sectionName,
                     std::string_view keyName) const;
    std::vector<KeyValue> getProperties(std::string_view sectionName) const;

  private:
    using PropertyName = std::pair<std::string, std::string>;
    using PropertyMap = std::map<PropertyName, std::string>;
    PropertyMap properties{};

    std::list<std::string> sections{};

    void clear();

    void addSection(std::string_view sectionName);
    void addProperty(const PropertyName &propertyName,
                     std::string_view value);
};

#endif /* __CONFIG_FILE__ */
