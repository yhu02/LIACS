### By:
- Yvo Hu             s2962802
- Joren van den Berg s2983281

## Summary modifications:

## 1. Grammar

### grammarbuilder.cpp

#### Assignment 1
- visit_number: Added boundchecking for uint8_t and throws an error otherwise. 
- visit_unary:  Standard implementation setting node and lineno
- visit_binary: Standard implementation setting node and lineno 

#### Assignment 2
- visit_ternary: Standard implementation setting node and lineno

#### Assignment 3
- open_scope: Standard implementation scope 
- close_scope: Standard implementation scope 
- process: Copied from brightspace
- visit_identifier: Implemented finding identifiers in current scope and creating SymbolNode
- findSymbol: Implemented finding identifiers in current scope
- refactored some functions and added return types to Unary and BinaryNode

### compiler.l

#### Assignment 1
- Added a pattern 
- Added a couple more tokens like SEMI MINUS PLUS etc.

#### Assignment 2
- Added `int main()` token

#### Assignment 4
- Added `while` token

### compiler.y

#### Assignment 1
- Added grammar rules according to the slides, and an additional rule for a sign token before a factor

#### Assignment 2
- Added grammar rules for if-else statements according to the slides.

#### Assignment 3
- Added scope blocks
- Added identifiers
- Added assignment of identifiers

#### Assignment 4
- Added grammars for while loops. This is done basically the same way as the if-statements and else-statements and scope-blocks from 
  assignment 2 and 3

## 2. Intermediate Code

### assignment1/calculator.cpp
- visit BinaryNode: Added switch case functionality for ADD, SUB and MUL and throws an error for other cases.
- visit UnaryNode: Added switch case functionality for + and - nodes. Addition is implemented via x = x + 0, and subtraction uses a 
  standard IOP_UNARY_MINUS.

### assignment2/conditionals.cpp
- visit_if_statement is for handling if-statements, obviously. We get as input `IF X THEN Y`. We will create a label, with a jump 
  operator when `X == false`. `Y` comes between the jump and the label, thus only executing it if `X == true`.
- visit_if_else_statement does the same thing, except with 2 labels, since we only want to execute either `X` or `Y`.
- The various visit functions handle the various nodes (Constant, Binary, Ternary), and executes the correct function depending on the 
  node type.

### assignment3/types.cpp
- visit BinaryNode: Accepts a list (which does not create any additional IC) and an assignment (x = 5). For assignment, left child will 
  be an id, so we 
  accept it and take the SymbolIOperand from visit(SymbolNode).
- visit Symbolnode: generate SymbolIOperand for identifiers and pass it up

### assignment4/divmod.cpp
- visit_while_statement: create a label, visit the conditional expression, visit the statements inside the while-loop body, goto first 
  statement, and add label where the conditional expression can jump to last.
- visit_div_statement: simple translation from intermediate to machine code, except when 2nd operand is an immediate. Then we have to 
  first move this immediate into a register, as the div machine instruction requires both operands in registers.

### icgenerator.h

#### Assignment 1
- Added a counter for the generation of unique identifiers. Starts from the top and counts down because the lower id's are already
  taken.

#### Assignment 2
- Removed the counter again, as it was not used by the correct solution to assignment 1 posted on brightspace.

### icgenerator.cpp

#### Assignment 1
- Visits both children and adds the correct instruction to the Istatement vector

#### Assignment 2
- visit_not_condition:
- Implemented the corresponding inverse IOP for each conditional to correctly translate jump statements

## 3. Machine Code

### assignment1/calculator.cpp
- Implemented visit_unary_minus and added the correctly translated machine code instructions to the vector

### assignment2/conditionals.cpp
-  add switch statement which calls the correct functions depending on the `IStatement`'s `IOPerator`.

### assignment3/types.cpp
-  Implemented the visit_binary part but didnt get the other half to work. Could not figure out how to force RAX register to be used.

### assignment4/divmod.cpp
- accept: Added conditional to separate div and modulus operations, which call the visit_div function with different parameters correspondingly.
- visit_div:  Implemented according to the instructions in the comments. RAX and RDX are referenced, operands are moved correspondingly to these registers, and sign/zero extended where applicable. Division is performed, and the result is stored in RDX or RAX depending on whether the instruction was a div or modulo.

### mcgenerator.cpp

#### Assignment 1
- to_immediate: implemented a function that returns the MOperand of an immediateOperand which also includes some basic error checking
- visit_binary: Implemented the correct translation from intermediate code to machine code of a binary instruction.

#### Assignment 2
- visit_binary: replaced assignment 1solution with correct solution from brightspace.
- generate_label: implemented to generate a simple label (nothing complex)
- visit_jump: translates intermediate jump instruction to machine code. Also puts intermediate values into registers.

#### Assignment 5
- visit_binary: added a check for when the destination would be a memory location. Instructions such as `imul` need the destionation to 
  be a register. We solve this by first loading the value from the destination memory location to a 'bridge' destination register. 
  Afterwards we have to move the value from the register to memory again, as otherwise it will get overwritten bij subsequent instructions.

### intermediatecodevisitor.h

#### Assignment 5
- updated register- and stackallocator to our new derived classes

### meson.build

#### Assignment 5
- added betterregister- and betterstackallocator files to the build files

### registerallocators/betterregisterallocator(.cpp|.h)

#### Assignment 5
- added files
- seems like it is not actually used? method has not been implemented but all test cases succeed regardless

### stackallocators/betterstackallocator(.cpp|.h)

#### Assignment 5
- added files
- to_memory: for every id, if we already encountered it we retrieve the offset and return a moperand with that offset. If we have not 
  yet encountered this id, we add the symbols width to the total offset allocated thus far, and store it.
- get_allocated: returns the total offset allocated by to_memory aligned to 16 bytes. This is used to allocate space for the stack.

## Reflection:

### Assignment 1
We had a really difficult time starting with the project because the framework was then very unknown to us. But we've managed at last to pick up the pace and (mostly) finish the assignment.
What was also really difficult to deal with was the fact that the amount of successful test cases didn't gradually increase with the amount of functionality you were implementing, but that they all more or less succeeded at the same time because the test cases will fail even if you completed most of the program. And because of the lack of feedback, it was a lot harder to notice any mistakes that you've made anywhere in your program because you can't know if somethings wrong until you've implemented all the code. And even then it's hard to spot where you went wrong. In the end we managed to overcome these obstacles by just being thorough and using trial and error.

### Assignment 2
As with assignment 1, we had slight difficulties with understanding the framework. Especially the visit functions of the if statements 
were hard to understand. We did not know that accepting a child, would subsequently generate its corresponding intermediate code, so had trouble with implementing the visits corresponding with the machine code. 

We also had trouble with debugging the code, as is apparant with the still 4 failed test cases. We know that it can't handle if (2 > 2), but it can handle if (2 > 2 + 2), which is weird as the latter succeeds but is more complex. There may be a register problem or intermediate problem right there but we can't seem to find it

### Assignment 3
We had no problems with understanding the framework this time. The return types were difficult to fix, because at first in most cases 
the program worked fine without them, but the generated machine code would not be correct. There were some test cases which caught this 
though, which really helped in fixing it.

All test cases run correctly 1 by 1, but 17 fail with "No more registers" when all run at once.

I think this is due to types not being passed correctly somewhere, causing a 32-bit register to be used for an 8-bit value.
We started the assignment a bit late so getting help was hard, which is something we can do better next time. 

### Assignment 4
We based a lot of our code from the previous assignments and missed little mistakes in the intermediate code that took us a while to figure out until we noticed them.
Machine code instructions were mostly straightforward, except that it didn't say to separate modulo and division, but we figured that out later.

### Assignment 5
Getting started was quite hard, as we didn't know where to start. Once we figured out how we were supposed to make the derived classes, 
we were off to a good start. Most test cases already succeeded, so now all we had to do was update the existing `visit_x` functions, of 
which some were not working with memory operands. The only one we actually had to update was `visit_binary`, and then all our test cases 
succeeded!