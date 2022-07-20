/* pagetables -- A framework to experiment with memory management
 *
 *    tracereader.h - Class to read valgrind "lackey" memory traces.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __TRACEREADER_H__
#define __TRACEREADER_H__

#include "linereader.h"
#include "process.h"  /* for MemAccess */

#include <memory>


class TraceReader
{
  private:
    std::unique_ptr<LineReader> lineReader;
    bool eofflag;
    int lineno;
    MemAccess currentAccess;

    void parseType(const std::string &str, MemAccess &access);
    bool parseLine(const std::string &line, MemAccess &access);
    void parseToNextAccess(void);

  public:
    TraceReader(std::istream &input);

    bool eof(void) const noexcept { return eofflag; }

    TraceReader &operator>>(MemAccess &access);
};

#endif /* __TRACEREADER_H__ */
