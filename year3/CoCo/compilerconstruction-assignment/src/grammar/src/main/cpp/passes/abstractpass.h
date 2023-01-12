/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_ABSTRACTPASS_H
#define BUILD_ABSTRACTPASS_H

#include <logger.h>

/**
 * This file contains an Abstract class for a SyntaxTree-pass
 * Passes are used to performs specific tasks in a SyntaxTree, after the tree is constructed
 * e.g. type-checking and type-setting
 */

class UnaryNodeBuilder;
class BinaryNodeBuilder;
class TernaryNodeBuilder;
class ConstantNodeBuilder;
class SymbolNodeBuilder;
class EmptyNodeBuilder;

class AbstractPass {
public:
    explicit AbstractPass(Logger& logger) : logger(logger) {};
    virtual ~AbstractPass() = default;

    virtual void process(UnaryNodeBuilder& builder) = 0;
    virtual void process(BinaryNodeBuilder& builder) = 0;
    virtual void process(TernaryNodeBuilder& builder) = 0;
    virtual void process(ConstantNodeBuilder& builder) = 0;
    virtual void process(SymbolNodeBuilder& builder) = 0;
    virtual void process(EmptyNodeBuilder& builder) = 0;

protected:
    Logger& logger;
};

#endif //BUILD_ABSTRACTPASS_H
