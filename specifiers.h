#ifndef SPECIFIERS_H
#define SPECIFIERS_H

#ifdef DEBUG

#define ONLY_DBG(...) __VA_ARGS__
#define ONLY_PRD(...)

#else // DEBUG

#define ONLY_DBG(...)
#define ONLY_PRD(...) __VA_ARGS__

#endif // DEBUG

#define STATIC ONLY_PRD(static)
#define INLINE ONLY_PRD(inline)

#endif // SPECIFIERS_H
