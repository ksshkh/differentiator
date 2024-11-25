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
            case DEG: {
                res = pow(left_res, right_res);
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
                case SUB: {
                    return _SUB(DiffTree(node->left, code_error), DiffTree(node->right, code_error));
                }
                case MUL: {
                    Node* dL = DiffTree(node->left,  code_error);
                    Node* dR = DiffTree(node->right, code_error);
                    Node* cL = CopyTree(node->left,  node, code_error);
                    Node* cR = CopyTree(node->right, node, code_error);

                    return _ADD(_MUL(dL, cR), _MUL(cL, dR));
                }
                case DIV: {
                    Node* dL = DiffTree(node->left,  code_error);
                    Node* dR = DiffTree(node->right, code_error);
                    Node* cL = CopyTree(node->left,  node, code_error);
                    Node* cR1 = CopyTree(node->right, node, code_error);
                    Node* cR2 = CopyTree(node->right, node, code_error);

                    return _DIV(_SUB(_MUL(dL, cR1), _MUL(cL, dR)), _DEG(cR2, _NUM(2.0)));
                }
                case DEG: {
                    if(node->right->type == NUM) {
    
                    }
                }
                default: {
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