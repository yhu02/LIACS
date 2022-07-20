/* pagetables -- A framework to experiment with memory management
 *
 *    linereader.cc - Abstraction for reading lines from differently stored
 *                    streams.
 *
 * Copyright (C) 2017-2018  Leiden University, The Netherlands.
 */

#include "linereader.h"
#include "exceptions.h"

#include <zlib.h>

/*
 * LineReaderBase
 */

class LineReaderBase final : public LineReader
{
  public:
    LineReaderBase(std::istream &input)
      : LineReader(input)
    { }

    virtual std::string getLine(void) override
    {
      std::string line;
      std::getline(input, line);

      if (input.eof())
        eofflag = true;

      return line;
    }
};

/*
 * ZippedLineReader
 */

static const int inbuffer_size(4096);
static const int outbuffer_size(256*1024);

class ZippedLineReader final : public LineReader
{
  private:
    z_stream zstrm;
    unsigned char inbuffer[inbuffer_size];
    unsigned char outbuffer[outbuffer_size];
    unsigned char *outptr;
    bool needReset;

  public:
    ZippedLineReader(std::istream &input)
      : LineReader(input), zstrm({0,}), outptr(nullptr), needReset(false)
    {
      zstrm.zalloc = Z_NULL;
      zstrm.zfree = Z_NULL;
      zstrm.opaque = Z_NULL;

      resetInputBuffer();
      resetOutputBuffer();

      /* Initialize ZLIB stream */
      if (inflateInit2(&zstrm, 16 + MAX_WBITS) != Z_OK)
        throw ReadError("cannot initialize zip-stream.");
    }

    ~ZippedLineReader()
    {
      inflateEnd(&zstrm);
    }

    /* Disallow objects from being copied, since it has a pointer member. */
    ZippedLineReader(const ZippedLineReader &reader) = delete;
    void operator=(const ZippedLineReader &reader) = delete;


    virtual std::string getLine(void) override
    {
      std::string line;

      while (true)
        {
          /* As long as there are bytes remaining in the output buffer,
           * we can read and process these.
           */
          while (outptr < zstrm.next_out)
            {
              unsigned char next = *(outptr++);

              /* If all (zipped) input has been read AND processed by
               * ZLIB, and we are at the end of the output buffer, we have
               * definitely reached the end of the stream.
               */
              if (input.eof() &&
                  outptr == zstrm.next_out &&
                  zstrm.avail_in == 0)
                eofflag = true;

              line.push_back(next);

              if (next == '\n' || eofflag)
                return line;
            }

          /* When we reach the end of the output buffer, read and process
           * the next chunk of the zipped input.
           */
          if (outptr == zstrm.next_out)
            refillOutputBuffer();
        }

      return line;
    }

    /* Peeks at the start of the given input stream to detect whether this
     * is a ZIP file. Returns true if this is the case.
     */
    static bool isZipStream(std::istream &input) noexcept
    {
      uint8_t c1 = input.get(), c2 = input.get();

      bool isZipStream = c1 == 0x1f && c2 == 0x8b;

      input.putback(c2);
      input.putback(c1);

      return isZipStream;
    }

  private:
    /* Read and process the next chunk of the zipped input stream. */
    void refillOutputBuffer(void)
    {
      if (needReset)
        {
          /* Reset to prepare for the next zipped input stream, in the case
           * multiple zipped streams are concatenated.
           */
          inflateReset(&zstrm);
          needReset = false;
        }

      /* If the entire input buffer has been consumed, we need to read
       * the next chunk from the stream.
       */
      if (zstrm.avail_in == 0)
        {
          input.read((char*)&inbuffer, inbuffer_size);
          const std::streamsize len = input.gcount();

          zstrm.avail_in = len;
          zstrm.next_in = (unsigned char *)&inbuffer;
        }

      resetOutputBuffer();

      /* Unzip (inflate) the next chunk. */
      int ret = inflate(&zstrm, input.eof() ? Z_FINISH : Z_NO_FLUSH);
      if (ret != Z_OK && ret != Z_STREAM_END)
        {
          inflateEnd(&zstrm);
          throw ReadError("error while processing zip-stream.");
        }
      else if (ret == Z_STREAM_END)
        /* The end of the zip-stream has been reached. Reset for the
         * next chunk. Important: any unconsumed bytes in the input buffer
         * are part of the next zip-stream.
         */
        needReset = true;
      else if (ret != Z_OK)
        throw ReadError("unknown error while processing zip-stream.");
    }

    inline void resetInputBuffer(void) noexcept
    {
      zstrm.avail_in = 0;
      zstrm.next_in = Z_NULL;
    }

    inline void resetOutputBuffer(void) noexcept
    {
      zstrm.avail_out = outbuffer_size;
      zstrm.next_out = (unsigned char *)&outbuffer;
      outptr = outbuffer;
    }
};


/*
 * LineReader
 */

LineReader::LineReader(std::istream &input)
  : input(input), eofflag(false)
{
}

LineReader *LineReader::create(std::istream &input)
{
  if (ZippedLineReader::isZipStream(input))
    return new ZippedLineReader(input);
  /* else */
  return new LineReaderBase(input);
}
