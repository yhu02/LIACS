/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_DECLLIST_H
#define BUILD_DECLLIST_H

#include <string>
#include <memory>
#include <vector>
#include "grammarbuilder.h"

/**
 * Class to build a 'DeclarationList'
 * In particular this is useful when declaring multiple variables of the same type,
 *  e.g. int a, b, c;
 * We can iterate over a DeclarationList in two ways: through an Iterator and SymbolIterator
 */
class DeclarationList {
public:
    DeclarationList() = default;

    /**
     * Adds a new variable to the DeclarationList with a provided name
     * @param name std::string of the name of the newly added variable
     */
    void addName(const std::string& name) { names.push_back(name); }

    /**
     * Allows iteration over the DeclarationList, while simultaneously creating Symbols for it
     * iterator designed according to: https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
     */
    struct SymbolIterator {
        using iterator_category     = std::input_iterator_tag;
        using difference_type       = std::ptrdiff_t;
        using value_type            = std::unique_ptr<Symbol>;
        using pointer               = std::unique_ptr<Symbol>*;
        using reference             = std::unique_ptr<Symbol>&;
        using internal              = std::vector<std::string>::iterator;

        explicit SymbolIterator(internal ptr, ReturnType rt, int line) : s_ptr(ptr), rt(rt), line(line) {}
        value_type operator*() const { return std::make_unique<Symbol>(*s_ptr, line, rt); }

        SymbolIterator& operator++() { ++s_ptr; return *this; }
        SymbolIterator operator++(int) { SymbolIterator tmp = *this; ++(*this); return tmp; }

        friend bool operator==(const SymbolIterator& a, const SymbolIterator& b) { return a.s_ptr == b.s_ptr; }
        friend bool operator!=(const SymbolIterator& a, const SymbolIterator& b) { return a.s_ptr != b.s_ptr; }

    private:
         internal s_ptr;
         ReturnType rt;
         int line;
    };

    /**
     * Allows enumeration (items + indices) over the DeclarationList
     */
    struct Iterator {
        using iterator_category     = std::input_iterator_tag;
        using difference_type       = std::ptrdiff_t;
        using value_type            = std::pair<std::string, size_t>;
        using pointer               = std::pair<std::string, size_t>*;
        using reference             = std::pair<std::string, size_t>&;
        using internal              = std::vector<std::string>::iterator;

        explicit Iterator(internal s_ptr, size_t start) : s_ptr(s_ptr), index(start) {}
        value_type operator*() { return std::make_pair(std::move(*s_ptr), index); }

        Iterator& operator++() { ++s_ptr; ++index; return *this; }
        Iterator& operator++(int) { Iterator* tmp = this; ++(*this); return *tmp; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.s_ptr == b.s_ptr; }
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.s_ptr != b.s_ptr; }
    private:
        internal s_ptr;
        size_t index;
    };

    // begin and end functions to retrieve iterators
    SymbolIterator begin(ReturnType rt, int line) { return SymbolIterator(names.begin(), rt, line); }
    SymbolIterator end(ReturnType rt, int line) { return SymbolIterator(names.end(), rt, line); }
    Iterator begin(size_t begin) { return Iterator(names.begin(), begin); }
    Iterator end() { return Iterator(names.end(), 0); }

private:
    std::vector<std::string> names{};
};

#endif //BUILD_DECLLIST_H
