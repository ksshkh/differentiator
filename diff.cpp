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

Node* DiffTree(size_t* num_of_nodes, Node* node, int* code_error) {

    if(!node) {
        return node;
    }

    Node* new_node = NULL;

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
                    new_node = _ADD(DiffTree(num_of_nodes, node->left, code_error), DiffTree(num_of_nodes, node->right, code_error));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
                }
                case SUB: {
                    new_node = _SUB(DiffTree(num_of_nodes, node->left, code_error), DiffTree(num_of_nodes, node->right, code_error));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
                }
                case MUL: {
                    Node* dL = DiffTree(num_of_nodes, node->left,  code_error);
                    Node* dR = DiffTree(num_of_nodes, node->right, code_error);
                    Node* cL = CopyTree(num_of_nodes, node->left,  node, code_error);
                    Node* cR = CopyTree(num_of_nodes, node->right, node, code_error);

                    new_node = _ADD(_MUL(dL, cR), _MUL(cL, dR));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
                }
                case DIV: {
                    Node* dL =  DiffTree(num_of_nodes, node->left,  code_error);
                    Node* dR =  DiffTree(num_of_nodes, node->right, code_error);
                    Node* cL =  CopyTree(num_of_nodes, node->left,  node, code_error);
                    Node* cR1 = CopyTree(num_of_nodes, node->right, node, code_error);
                    Node* cR2 = CopyTree(num_of_nodes, node->right, node, code_error);

                    new_node = _DIV(_SUB(_MUL(dL, cR1), _MUL(cL, dR)), _DEG(cR2, _NUM(2.0)));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
                }
                case DEG: {
                    if(node->right->type == NUM) {
                        new_node = _MUL(_MUL(_NUM(node->right->data), (_DEG(CopyTree(num_of_nodes, node->left, node, code_error), _NUM(node->right->data - 1.0)))), DiffTree(num_of_nodes, node->left, code_error));
                        FreeNode(num_of_nodes, node, code_error);
                        return new_node;
                    }
                }
                case SIN: {
                    new_node = _MUL(_COS(CopyTree(num_of_nodes, node->left, node, code_error)), DiffTree(num_of_nodes, node->left, code_error));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
                }
                case COS: {
                    new_node = _MUL(_MUL(_SIN(CopyTree(num_of_nodes, node->left, node, code_error)), DiffTree(num_of_nodes, node->left, code_error)), _NUM(-1.0));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
                }
                case LN: {
                    new_node = _MUL(_DIV(_NUM(1.0), CopyTree(num_of_nodes, node->left, node, code_error)), DiffTree(num_of_nodes, node->left, code_error));
                    FreeNode(num_of_nodes, node, code_error);
                    return new_node;
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

Node* CopyTree(size_t* num_of_nodes, Node* node, Node* parent, int* code_error) {

    if(!node) {
        return node;
    }

    node = NodeCtor(num_of_nodes, node->type, node->data, node->left, node->right, parent, code_error);
    node->left  = CopyTree(num_of_nodes, node->left,  node, code_error);
    node->right = CopyTree(num_of_nodes, node->right, node, code_error);

    return node;
}

Node* SimplifyExpression(size_t* num_of_nodes, Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    if(!node->left || !node->right) return node;

    if(node->left->left)   node->left =  SimplifyExpression(num_of_nodes, node->left,  code_error);
    if(node->right->right) node->right = SimplifyExpression(num_of_nodes, node->right, code_error);

    Node* new_node = NULL;

    switch((Operations)node->data) {
        case ADD: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(node->left->data + node->right->data);
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
            else if(CompareDoubles(node->right->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, node->left, code_error);
            }
            else if(CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, node->right, code_error);
            }
            break;
        }
        case SUB: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(node->left->data - node->right->data);
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
            else if(CompareDoubles(node->right->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, node->left, code_error);
            }
            else if(CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, node->right, code_error);
            }
            break;
        }
        case MUL: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(node->left->data * node->right->data);
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
            else if(CompareDoubles(node->right->data, 1.0)) {
                node = ReplaceNode(num_of_nodes, node, node->left, code_error);
            }
            else if(CompareDoubles(node->left->data, 1.0)) {
                node = ReplaceNode(num_of_nodes, node, node->right, code_error);
            }
            else if(CompareDoubles(node->right->data, 0.0) || CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, _NUM(0.0), code_error);
            }
            break;
        }
        case DIV: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(node->left->data / node->right->data);
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
            else if(CompareDoubles(node->left->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, _NUM(0.0), code_error);
            }
            else if(CompareDoubles(node->right->data, 1.0)) {
                node = ReplaceNode(num_of_nodes, node, node->left, code_error);
            }
            break;
        }
        case DEG: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(pow(node->left->data, node->right->data));
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
            else if(CompareDoubles(node->right->data, 1.0)) {
                node = ReplaceNode(num_of_nodes, node, node->left, code_error);
            }
            else if(CompareDoubles(node->right->data, 0.0)) {
                node = ReplaceNode(num_of_nodes, node, _NUM(1.0), code_error);
            }
            break;
        }
        case SIN: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(sin(node->left->data));
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
            break;
        }
        case COS: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(cos(node->left->data));
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
        }
        case LN: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(log(node->left->data));
                FreeNode(num_of_nodes, node, code_error);
                return new_node;
            }
        }
        default: {
            break;
        }
    }

    return node;
}
