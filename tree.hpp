#ifndef TREE_HPP
#define TREE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "errors.hpp"

static const char* INPUT_FILE = "./input.txt";

#define DEBUG

typedef double TreeElem;

enum Side {
    LEFT,
    RIGHT,
};


enum Type {
    DEFAULT = 0,

    OP,
    VAR,
    NUM
};

enum Operations {
    ADD = '+',
    SUB = '-',
    MUL = '*',
    DIV = '/',
    DEG = '^',
};

enum Vars {
    X = 'x',
};

struct Node {
    TreeElem data = 0;

    Type type = DEFAULT;

    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;
};

struct Tree {
    Node* root = NULL;
    size_t depth = 0;

    char* data_base = 0;
    long int size_data_base = 0;
};

#ifdef DEBUG

    #define TREE_ASSERT(tree) {                                               \
        int err = TreeVerification(tree, code_error);                         \
        if (err != NO_ERROR) {                                                \
            DotTreeDump(tree, code_error);                                    \
            fprintf(stderr, "error %d (file: %s, function: %s, line: %d)\n",  \
                    *code_error, __FILE__, __func__, __LINE__);               \
            ErrorsPrint(stderr, code_error);                                  \
        }                                                                     \
    }

    int TreeVerification(const Tree* tree, int* code_error);

    int NodeVerificator(const Node* node, int* code_error);

#else
    #define TREE_ASSERT(...)
#endif

void TreeCtor(Tree* tree, int* code_error);

Node* NodeCtor(Type type, TreeElem data, Node* left, Node* right, Node* parent, int* code_error);

void AddNewNode(Type type, Node* node, TreeElem data, Side side, int* code_error);

void TreeDtor(Tree* tree, int* code_error);

void FreeNode(Node* node, int* code_error);

void ReadTree(Tree* tree, int* code_error);

Node* ReadPreNode(Tree* tree, Node* node, Node* parent, int* code_error);

void GetTreeDepth(Tree* tree, int* code_error);

#endif // TREE_HPP