#include "tree.hpp"
#include "treedump.hpp"
#include "diff.hpp"

int main(void) {
    int code_error;
    Tree tree = {};

    TreeCtor(&tree, &code_error);
    ReadTree(&tree, &code_error);
    DotTreeDump(&tree, &code_error);
    printf("%lf\n", CountTree(tree.root, &code_error));
    PrintTree(&tree, &code_error);
    tree.root = DiffTree(tree.root, &code_error);
    DotTreeDump(&tree, &code_error);
    // TexTreeDump(&tree, &code_error);
    // Tree copy_tree = {};
    // copy_tree.root = CopyTree(tree.root, NULL, &code_error);
    // DotTreeDump(&copy_tree, &code_error);
    TreeDtor(&tree, &code_error);
    // TreeDtor(&copy_tree, &code_error);

    return 0;
}