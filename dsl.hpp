#ifndef DSL_HPP
#define DSL_HPP

#define _NUM(num) NodeCtor(NUM, num, NULL, NULL, NULL, code_error);
#define _ADD(left, right) NodeCtor(OP,  ADD, left, right, NULL, code_error);

#endif // DSL_HPP