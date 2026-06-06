#ifndef STATIC_H
#define STATIC_H

#ifdef DEBUG

#define STATIC
#define INLINE

#else // DEBUG

#define STATIC static
#define INLINE inline

#endif // DEBUG

#endif // STATIC_H
