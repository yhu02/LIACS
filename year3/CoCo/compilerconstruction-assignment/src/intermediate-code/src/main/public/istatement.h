/*
_ _ _ ____ ____ _  _ _ _  _ ____     ___  ____    _  _ ____ ___
| | | |__| |__/ |\ | | |\ | | __ .   |  \ |  |    |\ | |  |  |
|_|_| |  | |  \ | \| | | \| |__] .   |__/ |__|    | \| |__|  |

____ _  _ ____ _  _ ____ ____    ____ _ _    ____   /
|    |__| |__| |\ | | __ |___    |___ | |    |___  /
|___ |  | |  | | \| |__] |___    |    | |___ |___ .
*/

#ifndef BUILD_ISTATEMENT_H
#define BUILD_ISTATEMENT_H

#include <memory>

#include "ioperator.h"
#include "ioperand.h"

/**
 * File contains a class representing the statements used in IntermediateCode
 * IStatements represent the 3-op code and contain the following:
 *      type   signedness   iOperator   operand1    operand2    result
 */

class IStatement {
public:
    IStatement(OperatorType type, bool is_signed, IOperator iOperator, std::shared_ptr<IOperand> op1, std::shared_ptr<IOperand> op2, std::shared_ptr<IOperand> res)
    : type(type), is_signed(is_signed), iOperator(iOperator), operand1(std::move(op1)), operand2(std::move(op2)), result(std::move(res)) {}
    IStatement(ReturnType type, IOperator iOperator, std::shared_ptr<IOperand> op1, std::shared_ptr<IOperand> op2, std::shared_ptr<IOperand> res)
            : type(util::from_return_type(type)), is_signed(util::is_signed(type)), iOperator(iOperator),
            operand1(std::move(op1)), operand2(std::move(op2)), result(std::move(res)) {}

    [[nodiscard]] OperatorType get_type() const { return type; }
    [[nodiscard]] bool get_is_signed() const { return is_signed; }
    [[nodiscard]] IOperator get_ioperator() const { return iOperator; }
    [[nodiscard]] AssignmentType get_assignment_type() const { return util::get_assignment_type(iOperator); }
    [[nodiscard]] const IOperand* get_operand1() const { return operand1.get(); }
    [[nodiscard]] const IOperand* get_operand2() const { return operand2.get(); }
    [[nodiscard]] const IOperand* get_result() const { return result.get(); }
private:
    OperatorType type = OPT_VOID;
    bool is_signed = false;
    IOperator iOperator = IOP_UNKNOWN;
    std::shared_ptr<IOperand> operand1;
    std::shared_ptr<IOperand> operand2;
    std::shared_ptr<IOperand> result;
};

#endif //BUILD_ISTATEMENT_H
