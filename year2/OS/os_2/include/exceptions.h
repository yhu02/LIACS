/* pagetables -- A framework to experiment with memory management
 *
 *    exceptions.h - Exception classes
 *
 * Copyright (C) 2017-2020  Leiden University, The Netherlands.
 */

#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>

class ReadError : public std::runtime_error
{
  public:
    ReadError()
      : std::runtime_error("Error while reading trace file")
    { }

    ReadError(const std::string &err)
      : std::runtime_error(std::string("Error while reading trace file: ") + err)
    { }
};

class TraceFileParseError : public std::runtime_error
{
  public:
    TraceFileParseError()
      : std::runtime_error("Error while parsing memory trace file")
    { }

    TraceFileParseError(const std::string &err)
      : std::runtime_error(std::string("Error while parsing memory trace file: ") + err)
    { }

    TraceFileParseError(const int lineno, const std::string &err)
      : std::runtime_error(std::string("Error while parsing memory trace file: ") +
                           std::string("line ") + std::to_string(lineno) +
                           std::string(": ") + err)
    { }
};


#endif /* __EXCEPTIONS_H__ */
