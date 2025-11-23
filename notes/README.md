# Notes

## Compiler workflow

Conceptually, the compilation steps happen in the following order, although some are lazily (just in time) evaluated by later steps.

1. Preprocessor
    - Lazy
2. Scanner
    - Lazy
    - One per file included while processing a translation unit
3. Parser
4. Code generator
5. Assembler
6. Linker

## Code generation

One way we could do code generation.
Unverified code from ChatGPT.

```c
void generate_translation_unit(ASTNode* tu) {
    for each declaration in tu:
        if (declaration is function) {
            emit_label(declaration->name);
            generate_statement(declaration->body);
        }
}

void generate_statement(Statement* stmt) {
    if (stmt is ReturnStatement) {
        generate_expression(stmt->return_expr);
        emit("ret");
    }
}

void generate_expression(Expression* expr) {
    if (expr is IntegerLiteral) {
        emit("mov eax, %d", expr->int_value);
    }
}
```

For our simple example program, the assembly might look like this.

```asm
global main
section .text

main:
    mov eax, 2017
    ret
```
