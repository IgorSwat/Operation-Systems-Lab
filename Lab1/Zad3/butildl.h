#ifndef BUTILDL_H_INCLUDED
#define BUTILDL_H_INCLUDED

// Placeholder for dll linking to prevent any errors with declarations of functions

typedef struct {
    char* data;
    int size;
} mem_block;

typedef struct {
    mem_block** blocks;
    int* check;
    int capacity;
    int size;
} mem_btable;

void (*create_table)(mem_btable*, int);               // Zad 1.1
int (*read_block)(mem_btable*, const char*);   // Zad 1.2
char* (*get_bdata)(mem_btable*, int);               // Zad 1.3
int (*remove_block)(mem_btable*, int);              // Zad 1.4
void (*free_btable)(mem_btable*);                         // Zad 1.5

#endif // BUTILDL_H_INCLUDED
