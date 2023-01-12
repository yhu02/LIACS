/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#include <ostream>
#include <string>

#include "logger.h"

NullStream NULL_STREAM;

const Logger::Color Logger::RED = {"91"};
const Logger::Color Logger::YELLOW = {"93"};
const Logger::Color Logger::BLUE = {"94"};
const Logger::Color Logger::WHITE = {"97"};
const Logger::Color Logger::RESET = {"0"};
const Logger::Color Logger::BOLD = {"1"};

Logger::Logger(std::ostream& info_stream, std::ostream& warning_stream, std::ostream& error_stream) : info_stream(info_stream), warning_stream(warning_stream), error_stream(error_stream), infos(0), warnings(0), errors(0) {
}

std::ostream& Logger::info(int line) {
    ++infos;
    print_tag(info_stream, Logger::BLUE, "Info: ", line);
    return info_stream;
}

std::ostream& Logger::warn(int line) {
    ++warnings;
    print_tag(warning_stream, Logger::YELLOW, "Warning: ", line);
    return warning_stream;
}

std::ostream& Logger::error(int line) {
    ++errors;
    print_tag(error_stream, Logger::RED, "Error: ", line);
    return error_stream;
}

uint64_t Logger::n_infos() const {
    return infos;
}

uint64_t Logger::n_warnings() const {
    return warnings;
}

uint64_t Logger::n_errors() const {
    return errors;
}

void Logger::print_tag(std::ostream& stream, Color color, const std::string& tag, int line) {
    stream << Logger::BOLD << color << tag;

    if (line >= 0) {
        stream << Logger::WHITE << "line " << line << ": ";
    }

    stream << Logger::RESET;
}