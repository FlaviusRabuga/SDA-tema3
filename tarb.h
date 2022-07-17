//Rabuga Andrei-Flavius 313CB
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef ARBORE_BINAR
#define ARBORE_BINAR

typedef struct director {
    char *name;
    struct director *parent, *st, *dr, *directories;
    struct fisier *files;
} TD;

typedef struct fisier {
    char *name;
    struct director *parent;
    struct fisier *st, *dr;
} TF;

#endif

#ifndef _random_
#define _random_
#define random(n) (rand() % (n))
#define randomize() (srand((unsigned int)time(NULL)))
#endif
