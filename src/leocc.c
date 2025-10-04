#include <stdio.h>

int main(int argc, char **argv) {
    printf("Not yet implemented\n");
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
