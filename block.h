#ifndef MACROS_BLOCK_H
#define MACROS_BLOCK_H

// Wraps a multi-statement macro body so it takes a trailing semicolon and
// stays a single statement under an unbraced if/else.
#define MACRO_BEGIN do {
#define MACRO_END   } while(0)

#endif // MACROS_BLOCK_H
