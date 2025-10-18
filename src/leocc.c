#include <stdio.h>
#include "include/exceptions.h"
#include "include/scanner.h"

static void compile_translation_unit(char *filename) {
    scanner_t *scanner = scanner_create_from_file(filename);
    /*
    preprocessor_t *pp = preprocessor_create(scanner); // TODO initially preprocessor can just be a pass-through to scanner
    parser_t *parser = parser_create(pp);
    ast_t *ast = parse_translation_unit(parser);
    // TODO: codegen(ast);
    ast_print(ast);
    ast_destroy(ast);
    parser_destroy(parser);
    */
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [filename.c]\n", argv[0]);
        return FAILURE_INVALID_COMMAND_LINE_ARGS;
    }
    for (size_t idx = 1; idx < argc; idx++) {
        compile_translation_unit(argv[idx]);
    }
    // TODO AST: one per transaltion unit (aka source file)
    // TODO AST = parse(preprocess(source file))
    // TODO preprocessor calls scanner on source file, then every time it hits #include it will create a new scanner for that file and push it onto the stack of current scanners
    // TODO make this process lazy. Parser only asks for one token at a time from preprocessor, which maintains a stack of scanners (one per file) and asks the top scanner for its next token. If the preprocessor encounters a hashtag token, it handles the directive and then resumes with the current scanner.
    // TODO preprocess
    // TODO scan
    // TODO parse
    // TODO generate assembly code (code generator)
    // TODO assemble into object files
    // TODO link into executable
    return 0;
}
