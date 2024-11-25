#include "diff.hpp"

TreeElem CountTree(Node* node, int* code_error) {

    if(!node->left) {
        return node->data;
    }

    TreeElem left_res  = CountTree(node->left,  code_error);
    TreeElem right_res = CountTree(node->right, code_error);

    if(node->type == OP) {
        TreeElem res = 0;
        switch((int)node->data) {
            case ADD: {
                res = left_res + right_res;
                break;
            }
            case SUB: {
                res = left_res - right_res;
                break;
            }
            case MUL: {
                res = left_res * right_res;
                break;
            }
            case DIV: {
                res = left_res / right_res;
                break;
            }
            default:
                break;
        }
        return res;
    }
}

Node* DiffTree(Node* node, int* code_error) {

    if(!node) {
        return node;
    }

    switch(node->type) {
        case NUM: {
            return _NUM(0.0);
        }
        case VAR: {
            return _NUM(1.0);
        }
        case OP: {
            switch((int)node->data) {
                case ADD: {
                    return _ADD(DiffTree(node->left, code_error), DiffTree(node->right, code_error));
                }
                case MUL: {
                    node->data = (double)ADD;
                    break;
                }
            }
        }
        default: {
            break;
        }
    }

    return node;
}

Node* CopyTree(Node* node, Node* parent, int* code_error) {

    if(!node) {
        return node;
    }

    node = NodeCtor(node->type, node->data, node->left, node->right, parent, code_error);
    node->left  = CopyTree(node->left,  node, code_error);
    node->right = CopyTree(node->right, node, code_error);

    return node;
}