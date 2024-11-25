#ifndef DSL_HPP
#define DSL_HPP

#define _NUM(num) NodeCtor(NUM, num, NULL, NULL, NULL, code_error)

#define _ADD(left, right) NodeCtor(OP,  ADD, left, right, NULL, code_error)
#define _SUB(left, right) NodeCtor(OP,  SUB, left, right, NULL, code_error)
#define _MUL(left, right) NodeCtor(OP,  MUL, left, right, NULL, code_error)
#define _DIV(left, right) NodeCtor(OP,  DIV, left, right, NULL, code_error)
#define _DEG(left, right) NodeCtor(OP,  DEG, left, right, NULL, code_error)

#endif // DSL_HPP