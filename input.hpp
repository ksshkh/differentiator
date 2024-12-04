#ifndef INPUT_HPP
#define INPUT_HPP

#include <stdio.h>
#include <stdlib.h>

#include "tree.hpp"
#include "dsl.hpp"

void  TokensParcing(Tree* tree, size_t* num_of_nodes, int* code_error);

Node* GetTree      (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);
Node* GetNum       (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);
Node* GetVar       (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);
Node* GetDeg       (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);
Node* GetAddAndSub (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);
Node* GetMulAndDiv (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);
Node* GetBrackets  (size_t* num_of_nodes, char* buffer, size_t* ip, int* code_error);

#endif // INPUT_HPP