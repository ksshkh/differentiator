#include "../inc/treedump.hpp"
#include "../inc/utils.hpp"

#define NODE_NUM_COLOR     "\"#FAAA82\""
#define NODE_VAR_COLOR     "\"#A24892\""
#define NODE_OP_COLOR      "\"#E07082\""
#define NODE_BORDER_COLOR  "\"#203D98\""
#define BACKGROUND_COLOR   "\"#FFE7A5\""

static const char* DOT_FILE_NAME       = "./debug/tree.dot";
static const char* IMAGE_NAME          = "./debug/tree_image.svg";
static const char* HTML_FILE_NAME      = "./debug/tree.html";
static const char* TEX_FILE_NAME       = "./differentiator.tex";
static const char* INPUT_FILE          = "./input.txt";

#define PRINT_BRACKET(br) if(node->type == OP && node->parent && (Operations)node->parent->data != DIV) {                                                                                \
                              if(((Operations)node->data == ADD || (Operations)node->data == SUB) && ((Operations)node->parent->data != ADD && (Operations)node->parent->data != SUB)) { \
                                  fprintf(stream, "%c", br);                                                                                                                                  \
                              }                                                                                                                                                          \
                              else if((Operations)node->parent->data == DEG) {                                     \
                                  fprintf(stream, "%c", br);                                                                                                                                  \
                              }                                                                                                                                                          \
                          }

void DotTreeDump(Tree* tree, int* code_error) {

    FILE* dot_file = fopen(DOT_FILE_NAME, "w");

    if(dot_file) {
        fprintf(dot_file, "digraph Tree {\n");
        fprintf(dot_file, "\trankdir = TB;\n");
        fprintf(dot_file, "\tnode [shape = record];\n");
        fprintf(dot_file, "\tedge [color = " NODE_BORDER_COLOR "];\n");
        fprintf(dot_file, "\tbgcolor = " BACKGROUND_COLOR ";\n");

        if(tree) {
            if(tree->root) {
                PrintDotNode(tree->root, dot_file);
            }
            else {
                fprintf(stderr, "tree is empty\n");
            }
        }
        else {
            fprintf(stderr, "no tree\n");
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

void PrintDotNode(Node* node, FILE* stream) {

    if(!node) return;

    if(node->type == NUM) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_NUM_COLOR ", label = \"{indx: %p | type: %d | value: %.2lf | parent: %p | { left: %p | right: %p}}\"];\n",
            node, node, node->type, node->data, node->parent, node->left, node->right);
    }
    else if(node->type == OP) {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_OP_COLOR ", label = \"{", node);

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch-enum"

        switch((Operations)node->data) {
            case COS: {
                fprintf(stream, "cos");
                break;
            }
            case SIN: {
                fprintf(stream, "sin");
                break;
            }
            case LN: {
                fprintf(stream, "ln");
                break;
            }
            default: {
                fprintf(stream, "%c", (char)node->data);
                break;
            }
        }

        #pragma GCC diagnostic pop

        fprintf(stream, "}\"];\n");
    }
    else {
        fprintf(stream, "\tnode%p [color = " NODE_BORDER_COLOR ", shape = Mrecord, style = filled, fillcolor = " NODE_VAR_COLOR ", label = \"{indx: %p | type: %d | value: %c | parent: %p | { left: %p | right: %p}}\"];\n",
            node, node, node->type, (int)node->data, node->parent, node->left, node->right);
    }


    if(node->left) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->left);
        PrintDotNode(node->left,  stream);
    }

    if(node->right) {
        fprintf(stream, "\t\tnode%p -> node%p\n", node, node->right);
        PrintDotNode(node->right, stream);
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

    InorderPrinting(tree->root, printout, code_error);

    MY_ASSERT(fclose(printout) == 0, FCLOSE_ERROR);
}

void InorderPrinting(Node* node, FILE* stream, int* code_error) {

    MY_ASSERT(stream != NULL, FILE_ERROR);

    if(!node) {
        return;
    }

    fprintf(stream, "(");

    InorderPrinting(node->left, stream, code_error);

    if (node->type == NUM) {
        fprintf(stream, " %.2lf ", node->data);
    }
    else {
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wswitch-enum"

        switch((Operations)node->data) {
            case SIN: {
                fprintf(stream, " sin ");
                break;
            }
            case COS: {
                fprintf(stream, " cos ");
                break;
            }
            case LN: {
                fprintf(stream, " ln ");
                break;
            }
            default: {
                fprintf(stream, " %c ", (int)node->data);
            }
        }
        #pragma GCC diagnostic pop
    }

    InorderPrinting(node->right, stream, code_error);

    fprintf(stream, ")");

}

void PrintTexNode(Node* node, FILE* stream) {

    PRINT_BRACKET(L_BR)

    if     ((int)node->data == DIV) fprintf(stream, "\\frac{");
    else if((int)node->data == SIN) fprintf(stream, "\\sin(");
    else if((int)node->data == COS) fprintf(stream, "\\cos(");
    else if((int)node->data == LN) fprintf(stream, "\\ln(");

    if(node->left) PrintTexNode(node->left,  stream);

    if((int)node->data == DIV) fprintf(stream, "}");

    if(node->type == NUM) fprintf(stream, "%.2lf", node->data);
    else if(node->type == VAR) fprintf(stream, "%c", (int)node->data);
    else if(!(UNARY_CHECK((int)node->data)) && (int)node->data != DIV && (int)node->data != MUL) fprintf(stream, "%c", (int)node->data);
    else if((int)node->data == MUL) fprintf(stream, " \\cdot ");

    if((int)node->data == DIV || (int)node->data == DEG) fprintf(stream, "{");
    else if(UNARY_CHECK((int)node->data)) fprintf(stream, ")");

    if(node->right && !(UNARY_CHECK((int)node->data))) PrintTexNode(node->right, stream);

    if((int)node->data == DIV || (int)node->data == DEG) fprintf(stream, "}");

    PRINT_BRACKET(R_BR)

}

void TexTreeDump(Tree* tree, TreeElem var_value, int* code_error) {

    FILE* tex_file = fopen(TEX_FILE_NAME, "w");

    if(tex_file) {
        fprintf(tex_file, "\\documentclass[12pt, letterpaper]{article}\n");
        fprintf(tex_file, "\\usepackage[top=1in, bottom=1in, left=1in, right=1in]{geometry}\n");
        fprintf(tex_file, "\\title {Study of the function, the value at a point and its Taylor expansion}\n");
        fprintf(tex_file, "\\author{Khanevskaya Ksenia}\n");
        fprintf(tex_file, "\\date{\\today}\n");
        fprintf(tex_file, "\\begin{document}\n");
        fprintf(tex_file, "\\maketitle\n");
        fprintf(tex_file, "\\[f^{(0)}(x)=");

        if(tree) {
            if(tree->root) {
                PrintTexNode(tree->root, tex_file);

                fprintf(tex_file, "\\]\n");
                fprintf(tex_file, "Function value at a point $x=%.2lf$: \\[f(%.2lf)=%.2lf\\]\n", var_value, var_value, CountTree(tree->root, var_value, code_error));

                fprintf(tex_file, "First differential before simplification:\n");
                tree->root = DiffTree(&(tree->num_of_nodes), tree->root, code_error);
                fprintf(tex_file, "\\[f^{(1)}(x)=");
                PrintTexNode(tree->root, tex_file);

                DotTreeDump(tree, code_error);

                fprintf(tex_file, "\\]\n");

                fprintf(tex_file, "After simplification:\n");
                SimplifyTree(tree, code_error);
                fprintf(tex_file, "\\[f^{(1)}(x)=");
                PrintTexNode(tree->root, tex_file);

                DotTreeDump(tree, code_error);

                fprintf(tex_file, "\\]\n");

                fprintf(tex_file, "Taylor series expansion:\n");
                fprintf(tex_file, "\\[f(x)=f^{(0)}(x)+\\frac{1}{1!} \\cdot f^{(1)}(x)+\\frac{1}{2!} \\cdot f^{(2)}(x) + o(x^{2})\\]\n");

                fprintf(tex_file, "\\[f^{(1)}(x)=");
                PrintTexNode(tree->root, tex_file);
                fprintf(tex_file, "\\]\n");

                tree->root = DiffTree(&(tree->num_of_nodes), tree->root, code_error);
                SimplifyTree(tree, code_error);

                DotTreeDump(tree, code_error);

                fprintf(tex_file, "\\[f^{(2)}(x)=");
                PrintTexNode(tree->root, tex_file);
                fprintf(tex_file, "\\]\n");
            }
            else {
                fprintf(stderr, "tree is empty\n");
            }
        }
        else {
            fprintf(stderr, "no tree\n");
        }
        fprintf(tex_file, "\\end{document}\n");

        if(fclose(tex_file)) {
            fprintf(stderr, "file did not close\n");
        }
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

#undef NODE_NUM_COLOR
#undef NODE_BORDER_COLOR
#undef BACKGROUND_COLOR
#undef PRINT_BRACKET