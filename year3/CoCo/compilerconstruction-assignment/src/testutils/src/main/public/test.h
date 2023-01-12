/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_TESTUTILS_FIXTURE
#define COCO_FRAMEWORK_TESTUTILS_FIXTURE

#include "gtest/gtest.h"

#include <ghc/filesystem.hpp>
#include <logger.h>
#include <sstream>

class BaseTest : public ::testing::Test {
    public:
    BaseTest() = default;
};

/** A Test class with a logger */
class LoggingTest : public BaseTest {
    public:
    LoggingTest(std::ostream& infostream, std::ostream& warnstream, std::ostream& errorstream) : logger(infostream, warnstream, errorstream) {}
    ~LoggingTest() override = default;
    Logger logger;
};

/**
 * A Test class with a buffered logger.
 * Use this when you execute multiple runs in parallel and want any output to be printed on an exact moment.
 * To print the captured output, use:
 * ```
 * std::cerr << "Sample print :" << buf.str() << '\n';
 * ```
 */
class CapturingLoggingTest : public LoggingTest {
    public:
    CapturingLoggingTest() : LoggingTest(buf, buf, buf) {}
    ~CapturingLoggingTest() override = default;
    std::ostringstream buf;
};

class IOTest : public BaseTest {
    public:

    /** Enum to define the pipe type a value should be (either stdin (IN), stderr (ERR) or stdout (OUT). */
    enum IOType {
        IN='i', ERR='e', OUT='o'
    };

    /** Simple structure containing an IO type and a value. The value is always a string, because we don't care about the type, since the executable has to do that. */
    struct Token {
        IOType ioType;
        std::string value;

        Token(IOType ioType, std::string&& value) : ioType(ioType), value(std::move(value)) {}
        Token(IOType ioType, const std::string& value) : ioType(ioType), value(value) {}
    };

    /**
     * Simple class representing the required input, error and (expected) output pipe contents for a single test of an executable.
     * Interprets a string as an IOList. Strings should have form:
     * ```
     * <IOType><arg>,<IOType><arg>,<IOType><arg>,
     * i4,i5,i6,o7,o8,o9
     * ```
     * Where:
     *  - IOType is a valid IOType character.
     *  - arg is a token to use for testing.
     */
    class IOList {
        public:
            /**
             * Constructs an IOList from a string.
             * @param string The string to interpret.
             */
            explicit IOList(const std::string& string) {
                interpret(string);
            }

            /**
             * Constructs an IOList directly from tokens.
             * @param data The tokens to list.
             */
            explicit IOList(std::vector<Token> data) : data(std::move(data)) {}

            IOList(const IOList& other) noexcept {
                this->data = other.data;
            }

            IOList(IOList&& other) noexcept {
                std::swap(this->data, other.data);
            }

            inline Token operator[](std::size_t index) const { return data[index]; }
            inline auto begin() const noexcept { return data.begin(); }
            inline auto end() const noexcept { return data.end(); }
            inline std::size_t size() const { return data.size() - 1; }

            inline std::vector<Token> get_all(IOType type) const {
                std::vector<Token> out;
                std::copy_if(begin(), end(), std::back_inserter(out), [type](const Token& token) -> bool { return token.ioType == type; });
                return out;
            }

        protected:
            void interpret(const std::string& string) {
                std::string::size_type pos_cur = 0, pos_last = 0;
                while((pos_cur = string.find(',', pos_cur)) != std::string::npos) {
                    const auto tmp = string.substr(pos_last+1, pos_cur-pos_last-1);
                    data.emplace_back(Token(IOType(string[pos_last]), tmp));
                    pos_last = pos_cur+1;
                    pos_cur += 1;
                }
            }
            std::vector<Token> data;
    };

    static inline bool has_io(const ghc::filesystem::path& testpath) {
        return ghc::filesystem::exists(get_io_path(testpath));
    }

    static inline ghc::filesystem::path get_io_path(const ghc::filesystem::path& testpath) {
        auto tmp = testpath;
        tmp.replace_extension("io");
        return tmp;
    }

    static inline std::vector<IOList> get_tests(const ghc::filesystem::path& testpath) {
        if (!has_io(testpath))
            throw std::runtime_error("Provided testpath (" + testpath.string() + ") does not have an IO file.");
        std::ifstream ifstream(get_io_path(testpath));
        if (!ifstream.is_open())
            throw std::runtime_error("Failed to open provided testpath " + testpath.string() + ".");
        std::vector<IOList> res;
        std::string line;
        while (std::getline(ifstream, line))
            res.emplace_back(IOList(line));
        return res;
    }
};

namespace test {
    inline testing::AssertionResult has_success(const Logger& logger) {
        if (logger.n_errors() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_errors() << " errors.";
        if (logger.n_warnings() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_warnings() << " warnings.";
        return testing::AssertionSuccess();
    }

    inline testing::AssertionResult has_warnings(const Logger& logger) {
        if (logger.n_errors() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_errors() << " errors.";
        if (logger.n_warnings() > 0)
            return testing::AssertionSuccess();
        return testing::AssertionFailure() << "Found no warnings, expected at least 1.";
    }

    inline testing::AssertionResult has_errors(const Logger& logger) {
        if (logger.n_warnings() > 0)
            return testing::AssertionFailure() << "Found " << logger.n_warnings() << " warnings.";
        if (logger.n_errors() > 0)
            return testing::AssertionSuccess();
        return testing::AssertionFailure() << "Found no errors, expexted at least 1.";
    }
}

#endif