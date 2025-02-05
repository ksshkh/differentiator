#include "../inc/tree.hpp"
#include "../inc/treedump.hpp"

int main(int argc, char* argv[]) {
    int code_error;
    Tree tree = {};

    TreeCtor   (&tree, &code_error);
    ReadTree   (&tree, &code_error);
    DotTreeDump(&tree, &code_error);

    if(argc == 1) {
        TexTreeDump(&tree, X, &code_error);
    }
    else {
        TreeElem var_value = strtod(*(argv + 1), NULL);
        TexTreeDump(&tree, var_value, &code_error);
    }

    TreeDtor(&tree, &code_error);

    return 0;
}