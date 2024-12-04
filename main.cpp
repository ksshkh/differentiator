#include "tree.hpp"
#include "treedump.hpp"

int main(void) {
    int code_error;
    Tree tree = {};

    TreeCtor(&tree, &code_error);
    ReadTree(&tree, &code_error);
    DotTreeDump(&tree, &code_error);
    // printf("%lf\n", CountTree(tree.root, &code_error));
    // PrintTree(&tree, &code_error);
    // printf("%ld\n", tree.num_of_nodes);
    // tree.root = DiffTree(&(tree.num_of_nodes), tree.root, &code_error);
    // printf("%ld\n", tree.num_of_nodes);
    // TexTreeDump(&tree, &code_error);
    // DotTreeDump(&tree, &code_error);
    // tree.root = SimplifyExpression(&(tree.num_of_nodes), tree.root, &code_error);
    // DotTreeDump(&tree, &code_error);
    // tree.root = SimplifyElementaryOperations(&(tree.num_of_nodes), tree.root, &code_error);
    // DotTreeDump(&tree, &code_error);
    // printf("%ld\n", tree.num_of_nodes);
    // tree.root = SimplifyExpression(tree.root, &code_error);
    // DotTreeDump(&tree, &code_error);
    TreeDtor(&tree, &code_error);

    return 0;
}