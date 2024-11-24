#include "tree.hpp"

int main(void) {
    int code_error;
    Tree tree = {};

    TreeCtor(&tree, &code_error);
    ReadTree(&tree, &code_error);
    fprintf(stderr, "here\n");
    DotTreeDump(&tree, &code_error);
    printf("\n%d\n", CountTree(tree.root, &code_error));
    fprintf(stderr, "here\n");
    PrintTree(&tree, &code_error);
    TreeDtor(&tree, &code_error);

    return 0;
}