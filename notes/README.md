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
