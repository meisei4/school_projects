#ifndef NFSA_H
#define NFSA_H

// We want to use these in grammar.y which yacc will turn into grammar.c
// but implement them in C++ so we can use std::vector, std::string etc
#ifdef __cplusplus
extern "C" {
#endif
struct NFSA *     NFSA_Letter(int l);
struct NFSA *     NFSA_EmptyStr();
struct NFSA *     NFSA_EmptySet();
struct NFSA *     NFSA_ForUnion(struct NFSA * A, struct NFSA * B);
struct NFSA *     NFSA_ForConcat(struct NFSA * A, struct NFSA * B);
struct NFSA *     NFSA_ForStar(struct NFSA * A);
void              NFSA_Display(struct NFSA * A);
#ifdef __cplusplus
}
#endif

#endif
