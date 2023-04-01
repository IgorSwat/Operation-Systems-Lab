#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "blockutil.h"

#define MAX_COMMAND_SIZE 2048

void create_table(mem_btable* table, int table_capacity) {
    table->blocks = calloc(table_capacity, sizeof(mem_block*));
    table->check = calloc(table_capacity, sizeof(int));
    table->capacity = table_capacity;
    table->size = 0;
}

long get_file_size(FILE* file) {    // To get the size of file and improve implementation of read_block
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    return fsize;
}

int read_block(mem_btable* table, const char* filename) {
    char tmp_file[] = "/tmp/tmp_file.XXXXXX";
    int fd = mkstemp(tmp_file);
    unlink(tmp_file);       // Using unlink(file) and then close(file) removes tmp file automatically
    char command[MAX_COMMAND_SIZE];
    snprintf(command, MAX_COMMAND_SIZE, "wc '%s' 1> '%s' 2>/dev/null", filename, tmp_file);     // For contatenation of strings
    system(command);
    FILE* file = fopen(tmp_file, "r");
    long count_b = get_file_size(file);
    mem_block* block = malloc(sizeof(mem_block));   // Block initialisation
    block->data = malloc(count_b);
    block->size = count_b;
    fread(block->data, sizeof(char), count_b, file);
    fclose(file);
    if (table->size == table->capacity)     // If this is true, then the table is full and new block can not be added
        return 0;
    int b_id = 0;
    while (b_id < table->capacity && table->check[b_id] == 1)   // Search for first available place for new block
        b_id += 1;
    table->blocks[b_id] = block;
    table->check[b_id] = 1;
    table->size += 1;
    close(fd);
    return 1;
}

int check_bexist(mem_btable* table, int b_id) {     // Function for simple check if the block of given id exists in table
    if (b_id >= table->capacity || table->check[b_id] == 0)
        return 0;
    return 1;
}

char* get_bdata(mem_btable* table, int b_id) {      // Gets the block of given id
    if (check_bexist(table, b_id) == 0)
        return "";
    return table->blocks[b_id]->data;
}

int remove_block(mem_btable* table, int b_id) {
    if (check_bexist(table, b_id) == 0)
        return 0;
    free(table->blocks[b_id]->data);
    free(table->blocks[b_id]);
    table->check[b_id] = 0;
    table->size -= 1;
    return 1;
}

void free_btable(mem_btable* table) {       // Removes the content of table, but not the table itself
    for (int i = 0; i < table->capacity; i++) {
        remove_block(table, i);
    }
    free(table->blocks);
}
