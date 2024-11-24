#include "tree.hpp"
#include "utils.hpp"

#define NODE_NUM_COLOR     "\"#FAAA82\""
#define NODE_VAR_COLOR     "\"#A24892\""
#define NODE_OP_COLOR      "\"#E07082\""
#define NODE_BORDER_COLOR  "\"#203D98\""
#define BACKGROUND_COLOR   "\"#FFE7A5\""

static const char* DOT_FILE_NAME       = "./debug/tree.dot";
static const char* IMAGE_NAME          = "./debug/tree_image.svg";
static const char* HTML_FILE_NAME      = "./debug/tree.html";
static const char* INPUT_FILE          = "./input.txt";

#define OP_CHECK(data)  data == ADD || data == SUB || data == MUL || data == DIV || data == DEG
#define VAR_CHECK(data) data == X

void TreeCtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    tree->depth = 0;
    tree->root = NULL;

    fprintf(stderr, "here\n");
    tree->data_base = ReadInBuff(INPUT_FILE, &(tree->size_data_base), code_error);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);
    fprintf(stderr, "here\n");

    GetTreeDepth(tree, code_error);
}

Node* NodeCtor(Type type, TreeElem data, Node* left, Node* right, Node* parent, int* code_error) {

    Node* new_node = (Node*)calloc(1, sizeof(Node));
    MY_ASSERT(new_node != NULL, PTR_ERROR);

    new_node->type = type;
    new_node->data = data;
    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;

    return new_node;

}

void AddNewNode(Type type, Node* node, TreeElem data, Side side, int* code_error) {

    MY_ASSERT(data != NULL, PTR_ERROR);
    MY_ASSERT(node != NULL, PTR_ERROR);

    switch(side) {
        case LEFT: {
            Node* new_node = NodeCtor(type, data, node->left, NULL, node, code_error);
            MY_ASSERT(new_node != NULL, PTR_ERROR);

            node->left = new_node;
            break;
        }
        case RIGHT: {
            Node* new_node = NodeCtor(type, data, NULL, node->right, node, code_error);
            MY_ASSERT(new_node != NULL, PTR_ERROR);

            node->right = new_node;
            break;
        }
        default:
            fprintf(stderr, "wrong side\n");
    }

}

void TreeDtor(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    TREE_ASSERT(tree);

    FreeNode(tree->root, code_error);
    tree->root = NULL;

    free(tree->data_base);
    tree->data_base = NULL;

    tree->depth = 0;
    tree->size_data_base = 0;

}

void FreeNode(Node* node, int* code_error) {

    if(!node) return;

    FreeNode(node->left, code_error);
    FreeNode(node->right, code_error);
    free(node);

}

void DotTreeDump(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);

    FILE* dot_file = fopen(DOT_FILE_NAME, "w");
    MY_ASSERT(dot_file != NULL, FOPEN_ERROR);

    if(dot_file) {
        fprintf(dot_file, "digraph Tree {\n");
        fprintf(dot_file, "\trankdir = TB;\n");
        fprintf(dot_file, "\tnode [shape = record];\n");
        fprintf(dot_file, "\tedge [color = " NODE_BORDER_COLOR "];\n");
        fprintf(dot_file, "\tbgcolor = " BACKGROUND_COLOR ";\n");

        if(tree->root) {
            PrintNode(tree->root, dot_file);
        }
        else {
            fprintf(stderr, "tree is empty\n");
        }

        fprintf(dot_file, "}\n");

        if(fclose(dot_file)) {
            fprintf(stderr, "file did not close\n");
        }

        GraphCreate();
        HtmlDump(code_error);
    }
    else {
        fprintf(stderr, "file did not open\n");
    }

}

void PrintNode(Node* node, FILE* stream) {

    if(!node) return;


    if(node->type == NUM) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_NUM_COLOR ", label = \"{indx: %p | type: %d | value: %s | parent: %p | { left: %p | right: %p}}\"];\n",
            node, node, node->type, node->data, node->parent, node->left, node->right);
    }
    else if(node->type == OP) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_OP_COLOR ", label = \"{indx: %p | type: %d | value: %s | parent: %p | { left: %p | right: %p}}\"];\n",
            node, node, node->type, node->data, node->parent, node->left, node->right);
    }
    else {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_VAR_COLOR ", label = \"{indx: %p | type: %d | value: %s | parent: %p | { left: %p | right: %p}}\"];\n",
            node, node, node->type, node->data, node->parent, node->left, node->right);
    }


    if(node->left) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->left);
        PrintNode(node->left,  stream);
    }

    if(node->right) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->right);
        PrintNode(node->right, stream);
    }

}

void GraphCreate(void) {

    char command[] = "dot -Tsvg /home/ksenia/huawei/differentiator/debug/tree.dot -o /home/ksenia/huawei/differentiator/debug/tree_image.svg";
    system(command);
}

void HtmlDump(int *code_error) {

    FILE* html = fopen(HTML_FILE_NAME, "a");
    MY_ASSERT(html != NULL, FOPEN_ERROR);

    long int image_size = 0;

    char *image_data = ReadInBuff(IMAGE_NAME, &image_size, code_error);
    MY_ASSERT(image_data != NULL, PTR_ERROR);

    fprintf(html, "%s\n", image_data);

    MY_ASSERT(fclose(html) == 0, FCLOSE_ERROR);
}

void PrintTree(Tree* tree, int* code_error) {

    TREE_ASSERT(tree);

    FILE* printout = fopen(INPUT_FILE, "w");
    MY_ASSERT(printout != NULL, FOPEN_ERROR);

    PreorderPrinting(tree->root, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);
}

void PreorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        return;
    }

    fprintf(stream, "(");
    fprintf(stream, " %s ", node->data);

    PreorderPrinting(node->left, stream, code_error);
    PreorderPrinting(node->right, stream, code_error);

    fprintf(stream, ")");
}

void PostorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        return;
    }

    fprintf(stream, "(");

    PostorderPrinting(node->left, stream, code_error);
    PostorderPrinting(node->right, stream, code_error);
    fprintf(stream, " %s ", node->data);

    fprintf(stream, ")");
}

void InorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        return;
    }

    fprintf(stream, "(");

    InorderPrinting(node->left, stream, code_error);
    fprintf(stream, " %s ", node->data);
    InorderPrinting(node->right, stream, code_error);

    fprintf(stream, ")");

}

void ReadTree(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    char* copy_data_base = tree->data_base;

    tree->root = ReadPreNode(tree, tree->root, NULL, code_error);

    tree->data_base = copy_data_base;

}



int CountTree(Node* node, int* code_error) {

    if(!node->left) {
        return atoi(node->data);
    }

    int left_res = CountTree(node->left,  code_error);
    int right_res = CountTree(node->right, code_error);

    if(node->type == OP) {
        int res = 0;
        switch(*node->data) {
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

// Node* ReadInNode(Tree* tree, Node* node, Node* left, Node* right, int* code_error) {

//     while(isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
//         tree->data_base++;
//     }

//     if(*(tree->data_base) == ')') {
//         return node;
//     }

//     while(isspace(*(tree->data_base)) || *(tree->data_base) == '(') {
//         tree->data_base++;
//     }

//     char* data = tree->data_base;

//     while(!isspace(*(tree->data_base))) {
//         tree->data_base++;
//     }

//     *(tree->data_base) = '\0';

//     Type type = DEFAULT;

//     if(OP_CHECK(*data)) {
//         type = OP;
//     }
//     else if(VAR_CHECK(*data)) {
//         type = VAR;
//     }
//     else {
//         type = NUM;
//     }


//     node = NodeCtor(type, data, NULL, NULL, NULL, code_error);

//     // if(node->type == NUM || node->type == VAR) {
//     //     while(*(tree->data_base) == ')' || isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
//     //         tree->data_base++;
//     //     }
//     //     return node;
//     // }

//     while(*(tree->data_base) == ')' || isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
//         tree->data_base++;
//     }

//     Node* parent_node = ReadInNode(tree, node->parent, node, NULL, code_error);

//     node->left = node;

//     while(*(tree->data_base) == ')' || isspace(*(tree->data_base))) {
//         tree->data_base++;
//     }

//     Node* right_node = ReadInNode(tree, node->parent, NULL, node, code_error);

//     return node;

// }

Node* ReadPreNode(Tree* tree, Node* node, Node* parent, int* code_error) {

    while(isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
        tree->data_base++;
    }

    if(*(tree->data_base) == ')') {
        return node;
    }

    while(isspace(*(tree->data_base)) || *(tree->data_base) == '(') {
        tree->data_base++;
    }

    char* data = tree->data_base;

    while(!isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    *(tree->data_base) = '\0';

    Type type = DEFAULT;

    if(OP_CHECK(*data)) {
        type = OP;
    }
    else if(VAR_CHECK(*data)) {
        type = VAR;
    }
    else {
        type = NUM;
    }


    node = NodeCtor(type, data, NULL, NULL, parent, code_error);

    if(node->type == NUM || node->type == VAR) {
        while(*(tree->data_base) == ')' || isspace(*(tree->data_base)) || *(tree->data_base) == '\0') {
            tree->data_base++;
        }
        return node;
    }

    node->left = ReadPreNode(tree, node->left, node, code_error);

    while(*(tree->data_base) == ')' || isspace(*(tree->data_base))) {
        tree->data_base++;
    }

    node->right = ReadPreNode(tree, node->right, node, code_error);

    return node;
}

void GetTreeDepth(Tree* tree, int* code_error) {

    MY_ASSERT(tree != NULL, PTR_ERROR);
    MY_ASSERT(tree->data_base != NULL, PTR_ERROR);

    size_t curtain_depth = 0;

    for(long int i = 0; i < tree->size_data_base; i++) {

        if     (tree->data_base[i] == '(') curtain_depth++;
        else if(tree->data_base[i] == ')') curtain_depth--;

        tree->depth = (curtain_depth > tree->depth) ? curtain_depth : tree->depth;
    }
}

#ifdef DEBUG

    int TreeVerification(const Tree* tree, int* code_error) {

        if(!tree) {
            *code_error |= NO_TREE;
            return *code_error;
        }

        if(!tree->data_base) {
            *code_error |= BASE_ERROR;
        }

        if(!tree->root) {
            *code_error |= NO_ROOT;
            return *code_error;
        }

        *code_error = NodeVerificator(tree->root, code_error);

        return *code_error;
    }

    int NodeVerificator(const Node* node, int* code_error) {

        if(!node) {
            return NO_ERROR;
        }

        if(node->parent != NULL) {
            if(!(node->parent->left == node || node->parent->right == node)) {
                *code_error |= CONNECT_ERROR;
            }
            if(node->parent->left == node && node->parent->right == node) {
                *code_error |= CONNECT_ERROR;
            }
        }

        *code_error |= NodeVerificator(node->left, code_error);
        *code_error |= NodeVerificator(node->right, code_error);

        return *code_error;
    }

#endif

#undef NODE_NUM_COLOR
#undef NODE_BORDER_COLOR
#undef BACKGROUND_COLOR
#undef OP_CHECK