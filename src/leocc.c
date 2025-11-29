#include <stdio.h>
#include "include/exceptions.h"
#include "include/scanner.h"
#include "include/preprocessor.h"
#include "include/parser.h"
#include "include/ast.h"
#include "include/codegen.h"

static void compile_translation_unit(char *filename) {
    scanner_t *scanner = scanner_create_from_file(filename);
    preprocessor_t *pp = preprocessor_create(scanner);
    parser_t *parser = parser_create(pp);
    ast_node_t *ast = parse_translation_unit(parser);
    char *assembly_prog = codegen_translation_unit(ast);
    printf("%s\n", assembly_prog);
    // TODO: nasm -f win64 .\leocc_out.asm -o leocc_out.obj
    // TODO: gcc -o leocc_out.exe .\leocc_out.obj
    free(assembly_prog);
    ast_node_destroy(ast);
    parser_destroy(parser);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [filename.c] ...\n", argv[0]);
        return FAILURE_INVALID_COMMAND_LINE_ARGS;
    }
    for (size_t idx = 1; idx < argc; idx++) {
        compile_translation_unit(argv[idx]);
    }
    return 0;
}
