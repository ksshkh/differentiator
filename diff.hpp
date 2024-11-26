#ifndef DIFF_HPP
#define DIFF_HPP

#include <math.h>

#include "tree.hpp"
#include "dsl.hpp"

TreeElem CountTree(Node* node, int* code_error);

Node* CopyTree(Node* node, Node* parent, int* code_error);

Node* DiffTree(Node* node, int* code_error);

Node* SimplifyConstant(Node* node, int* code_error);

Node* SimplifyElementaryOperations(Node* node, int* code_error);

#endif // DIFF_HPP