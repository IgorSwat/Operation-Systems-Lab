#ifndef BLOCKUTIL_H_INCLUDED
#define BLOCKUTIL_H_INCLUDED

typedef struct {
    char* data;
    int size;
} mem_block;        // Simple block with avaible capacity check

typedef struct {
    mem_block** blocks;
    int* check;     // For checking if chosen block is not deleted and exists in memory
    int capacity;
    int size;
} mem_btable;       // Table of blocks

// For all used functions: 0 = false, 1  = true
void create_table(mem_btable* table, int table_capacity);  // Zad 1.1
int read_block(mem_btable* table, const char* filename);   // Zad 1.2
char* get_bdata(mem_btable* table, int b_id);               // Zad 1.3
int remove_block(mem_btable* table, int b_id);              // Zad 1.4
void free_btable(mem_btable* table);                         // Zad 1.5

#endif // BLOCKUTIL_H_INCLUDED
