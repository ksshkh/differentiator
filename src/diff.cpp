#include "../inc/diff.hpp"

TreeElem CountTree(Node* node, TreeElem var_value, int* code_error) {

    if(node->type == NUM) {
        return node->data;
    }

    if(node->type == VAR) {
        return var_value;
    }

    TreeElem left_res  = CountTree(node->left,  var_value, code_error);
    TreeElem right_res = CountTree(node->right, var_value, code_error);

    TreeElem res = 0;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

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
            res = sin(left_res);
            break;
        }
        case COS: {
            res = cos(left_res);
            break;
        }
        case LN: {
            res = log(left_res);
            break;
        }
        default:
            break;
    }

    #pragma GCC diagnostic pop

    return res;
}

Node* DiffTree(size_t* num_of_nodes, Node* node, int* code_error) {

    if(!node) {
        return node;
    }

    Node* new_node = NULL;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

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
                    new_node = _ADD(_DIFF(node->left), _DIFF(node->right));
                    return new_node;
                }
                case SUB: {
                    new_node = _SUB(_DIFF(node->left), _DIFF(node->right));
                    return new_node;
                }
                case MUL: {
                    Node* dL = _DIFF(node->left );
                    Node* dR = _DIFF(node->right);
                    Node* cL = _COPY(node->left,  node);
                    Node* cR = _COPY(node->right, node);
                    new_node = _ADD(_MUL(dL, cR), _MUL(cL, dR));
                    return new_node;
                }
                case DIV: {
                    Node* dL  = _DIFF(node->left );
                    Node* dR  = _DIFF(node->right);
                    Node* cL  = _COPY(node->left,  node);
                    Node* cR1 = _COPY(node->right, node);
                    Node* cR2 = _COPY(node->right, node);

                    new_node = _DIV(_SUB(_MUL(dL, cR1), _MUL(cL, dR)), _DEG(cR2, _NUM(2.0)));
                    return new_node;
                }
                case DEG: {
                    if(node->right->type == NUM) {
                        new_node = _MUL(_MUL(_NUM(node->right->data), (_DEG(_COPY(node->left,  node), _NUM(node->right->data - 1.0)))), _DIFF(node->left));
                        return new_node;
                    }
                    break;
                }
                case SIN: {
                    new_node = _MUL(_COS(_COPY(node->left,  node)), _DIFF(node->left));
                    return new_node;
                }
                case COS: {
                    new_node = _MUL(_MUL(_SIN(_COPY(node->left,  node)), _DIFF(node->left)), _NUM(-1.0));
                    return new_node;
                }
                case LN: {
                    new_node = _MUL(_DIV(_NUM(1.0), _COPY(node->left,  node)), _DIFF(node->left));
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

    #pragma GCC diagnostic pop

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

void SimplifyTree(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    size_t old_num_of_nodes = 0;

    do {
        old_num_of_nodes = tree->num_of_nodes;
        tree->root = SimplifyExpression(&(tree->num_of_nodes), tree->root, code_error);

    } while(old_num_of_nodes != tree->num_of_nodes);
}

Node* SimplifyExpression(size_t* num_of_nodes, Node* node, int* code_error) {

    MY_ASSERT(node != NULL, PTR_ERROR);

    if(!node->left || !node->right) return node;

    if(node->left->left)   node->left =  SimplifyExpression(num_of_nodes, node->left,  code_error);
    if(node->right->right) node->right = SimplifyExpression(num_of_nodes, node->right, code_error);

    Node* new_node = NULL;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"

    switch((Operations)node->data) {
        case ADD: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(node->left->data + node->right->data);
                new_node->parent = node->parent;
                _FREE(node);
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
                new_node->parent = node->parent;
                _FREE(node);
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
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
            else if(node->left->type == NUM && (Operations)node->right->data == MUL && node->right->left->type == NUM) {
                new_node = _MUL(_NUM(node->left->data * node->right->left->data), _COPY(node->right->right, node->right));
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
            else if(node->left->type == NUM && (Operations)node->right->data == MUL && node->right->right->type == NUM) {
                new_node = _MUL(_NUM(node->left->data * node->right->right->data), _COPY(node->right->left, node->right));
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
            else if(node->right->type == NUM && (Operations)node->left->data == MUL && node->left->left->type == NUM) {
                new_node = _MUL(_NUM(node->right->data * node->left->left->data), _COPY(node->left->right, node->left));
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
            else if(node->right->type == NUM && (Operations)node->left->data == MUL && node->left->right->type == NUM) {
                new_node = _MUL(_NUM(node->right->data * node->left->right->data), _COPY(node->left->left, node->left));
                new_node->parent = node->parent;
                _FREE(node);
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
                new_node->parent = node->parent;
                _FREE(node);
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
                new_node->parent = node->parent;
                _FREE(node);
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
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
            break;
        }
        case COS: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(cos(node->left->data));
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
            break;
        }
        case LN: {
            if(node->left->type == NUM && node->right->type == NUM) {
                new_node = _NUM(log(node->left->data));
                new_node->parent = node->parent;
                _FREE(node);
                return new_node;
            }
        }
        default: {
            break;
        }
    }

    #pragma GCC diagnostic pop

    return node;
}
