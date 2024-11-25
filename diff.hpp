#ifndef DIFF_HPP
#define DIFF_HPP

#include "tree.hpp"
#include "dsl.hpp"

TreeElem CountTree(Node* node, int* code_error);

Node* CopyTree(Node* node, Node* parent, int* code_error);

Node* DiffTree(Node* node, int* code_error);

#endif // DIFF_HPP