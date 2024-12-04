#ifndef DIFF_HPP
#define DIFF_HPP

#include <math.h>

#include "tree.hpp"
#include "dsl.hpp"

TreeElem CountTree(Node* node, int* code_error);

Node* CopyTree(size_t* num_of_nodes, Node* node, Node* parent, int* code_error);

Node* DiffTree(size_t* num_of_nodes, Node* node, int* code_error);

Node* SimplifyExpression(size_t* num_of_nodes, Node* node, int* code_error);

#endif // DIFF_HPP