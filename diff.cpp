#include "diff.hpp"

TreeElem CountTree(Node* node, int* code_error) {

    if(!node->left) {
        return node->data;
    }

    TreeElem left_res  = CountTree(node->left,  code_error);
    TreeElem right_res = CountTree(node->right, code_error);

    if(node->type == OP) {
        TreeElem res = 0;
        switch((Operations)node->data) {
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
            case SIN: {
                res = cos(left_res);
                break;
            }
            case COS: {
                res = sin(left_res);
                break;
            }
            case LN: {
                res = log(left_res);
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
                        return _MUL(_MUL(_NUM(node->right->data), (_DEG(CopyTree(node->left, node, code_error), _NUM(node->right->data - 1.0)))), DiffTree(node->left, code_error));
                    }
                }
                case SIN: {
                    if(node->left->type == NUM) return _NUM(0.0);
                    return _MUL(_COS(CopyTree(node->left, node, code_error)), DiffTree(node->left, code_error));
                }
                case COS: {
                    if(node->left->type == NUM) return _NUM(0.0);
                    return _MUL(_MUL(_SIN(CopyTree(node->left, node, code_error)), DiffTree(node->left, code_error)), _NUM(-1.0));
                }
                case LN: {
                    if(node->left->type == NUM) return _NUM(0.0);
                    return _MUL(_DIV(_NUM(1.0), CopyTree(node->left, node, code_error)), DiffTree(node->left, code_error));
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

Node* SimplifyConstant(Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    if(node->left->left)   node->left =  SimplifyConstant(node->left,  code_error);
    if(node->right->right) node->right = SimplifyConstant(node->right, code_error);

    if(node->left->type == NUM && node->right->type == NUM) {
        switch((Operations)node->data) {
            case ADD: {
                return _NUM(node->left->data + node->right->data);
            }
            case SUB: {
                return _NUM(node->left->data - node->right->data);
            }
            case MUL: {
                return _NUM(node->left->data * node->right->data);
            }
            case DIV: {
                return _NUM(node->left->data / node->right->data);
            }
            case DEG: {
                return _NUM(pow(node->left->data, node->right->data));
            }
            case SIN: {
                return _NUM(sin(node->left->data));
            }
            case COS: {
                return _NUM(cos(node->left->data));
            }
            case LN: {
                return _NUM(log(node->left->data));
            }
            default: {
                break;
            }
        }
    }

    return node;
}

Node* SimplifyElementaryOperations(Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    if(node->left->left)   node->left =  SimplifyElementaryOperations(node->left,  code_error);
    if(node->right->right) node->right = SimplifyElementaryOperations(node->right, code_error);

    switch((Operations)node->data) {
        case ADD: {
            if(CompareDoubles(node->right->data, 0.0)) {
                node = ReplaceNode(node, node->left, code_error);
            }
            else if(CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(node, node->right, code_error);
            }
            break;
        }
        case SUB: {
            if(CompareDoubles(node->right->data, 0.0)) {
                node = ReplaceNode(node, node->left, code_error);
            }
            else if(CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(node, node->right, code_error);
            }
            break;
        }
        case MUL: {
            if(CompareDoubles(node->right->data, 1.0)) {
                node = ReplaceNode(node, node->left, code_error);
            }
            else if(CompareDoubles(node->left->data, 1.0)) {
                node = ReplaceNode(node, node->right, code_error);
            }
            else if(CompareDoubles(node->right->data, 0.0) || CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(node, _NUM(0.0), code_error);
            }
            break;
        }
        case DIV: {
            if(CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(node, _NUM(0.0), code_error);
            }
            else if(CompareDoubles(node->right->data, 1.0)) {
                node = ReplaceNode(node, node->left, code_error);
            }
            break;
        }
        case DEG: {
            if(CompareDoubles(node->right->data, 1.0)) {
                node = ReplaceNode(node, node->left, code_error);
            }
            else if(CompareDoubles(node->right->data, 0.0)) {
                node = ReplaceNode(node, _NUM(1.0), code_error);
            }
            break;
        }
        default: {
            break;
        }
    }

    return node;
}