/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/


#ifndef BUILD_SCOPE_H
#define BUILD_SCOPE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <limits>

/**
 * Class to keep track of the variables in a certain scope.
 * This is only required in the grammar-parsing stage, as this is the only stage in which we check
 * whether usages of variables happen in the right scope.
 */
class Scope {
public:
    Scope() = default;

    /**
     * Add multiple symbols to the scope by passing the begin and end of a std::forward_iterator
     */
    template<typename It = typename std::forward_iterator_tag>
    void addSymbols(It begin, It end) { symbols.insert(begin, end); }

    /**
     * Checks whether a variable with the provided name exists within the current Scope
     * @param name std::string of the name of the variable to check
     * @return true on existence, else false
     */
    bool exists(const std::string& name) const { return symbols.find(name) != symbols.end(); }

    /**
     * Returns the id of the Symbol with a given name
     * @param name std::string of the name of the Symbol to search
     * @return the id of the found symbol or std::numeric_limits<size_t>::max() if it could not be found
     */
    size_t getSymbol(const std::string& name) {
        auto it = symbols.find(name);
        if (it == symbols.end())
            return std::numeric_limits<size_t>::max();
        return it->second;
    }

private:
    // Note: name of symbols should be unique within each scope
    std::unordered_map<std::string, size_t> symbols = {};
};

#endif //BUILD_SCOPE_H
