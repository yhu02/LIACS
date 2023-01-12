/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef COCO_FRAMEWORK_GENERAL_TEMPLATE_HACKS
#define COCO_FRAMEWORK_GENERAL_TEMPLATE_HACKS

#include <memory>
#include <stdexcept>
#include <string>

namespace hack {

    // Object to hold constexpr name.
    struct string_view {
        constexpr string_view() = default;
        constexpr string_view(const char* data, size_t size) : data(data), size(size) {}

        char const* data{};
        std::size_t size{};
    };
    // Ostream overload to print name.
    inline std::ostream& operator<<(std::ostream& o, string_view const& s) {
        return o.write(s.data, s.size);
    }

    /**
     * Template hack to get the type of any templated variable as a printable string. Uesful for overloaded functions.
     * @tparam T typename to print.
     * @return Constexpr string for T.
     */
    template<class T>
    inline constexpr string_view get_name() {
        char const* p = __PRETTY_FUNCTION__;
        while (*p++ != '=')
            ;
        for (; *p == ' '; ++p)
            ;
        char const* p2 = p;
        int count = 1;
        for (;; ++p2) {
            switch (*p2) {
            case '[':
                ++count;
                break;
            case ']':
                --count;
                if (!count)
                    return {p, std::size_t(p2 - p)};
            }
        }
    }

    /**
     * Function to format strings pre-C++20, using C++11's `std::snprintf`. if accepts old-fashioned C printf format strings.
     * @tparam Args Type of passed arguments.
     * @param format Format string to apply.
     * @param args Arguments to format into the string.
     * @return The formatted string.
     * @note Do not use this function extensively. Every other formatting configuration will generate another compile-time function, making the final binary huge. Instead, use `stringstream`.
     * @note Unsafe when used
     */
    template<typename... Args>
    std::string format(const std::string& format, Args... args) {
        size_t size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
        if (size <= 0)
            throw std::runtime_error("Error during formatting.");
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
}
#endif
