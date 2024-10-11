#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

const int HASH_TABLE_MIN_CAPACITY = 5;

typedef struct membuf {
    char* data;
    int len;
} membuf;

typedef struct hash_table {
    membuf* entries;
    int used_size;
    int capacity;
} hash_table;

hash_table* create_hash_table(int capacity) {
    if (capacity < HASH_TABLE_MIN_CAPACITY) {
        printf("Hash table capacity must be at least %d\n", HASH_TABLE_MIN_CAPACITY);
        return NULL;
    }
    hash_table* table = (hash_table*)malloc(sizeof(hash_table));
    table->entries = (membuf*)calloc(capacity, sizeof(membuf));
    table->used_size = 0;
    table->capacity = capacity;
    return table;
}

void insert_no_resize(int* capacity, int* used_size, membuf* entries, membuf* entry, unsigned long key) {
    if (entries[key].data == NULL) {
        entries[key] = *entry;
        (*used_size)++;
    } else {
        unsigned long i = key;
        int iterations = 0;
        while (entries[i].data != NULL) {
            iterations++;
            i++;
            if (i >= *capacity) {
                i = 0;
            }
        }
        entries[i] = *entry;
        (*used_size)++;
    }
}

void insert(hash_table* table, membuf* entry) {
    unsigned long key = hash(entry->data) % table->capacity;
    insert_no_resize(&table->capacity, &table->used_size, table->entries, entry, key);

    if (table->used_size >= table->capacity / 2) {
        int new_used_size = 0;
        int new_capacity = table->capacity * 2;
        membuf* new_entries = (membuf*)calloc(new_capacity, sizeof(membuf));

        for (int i = 0; i < table->capacity; i++) {
            if (table->entries[i].data != NULL) {
                unsigned long key = hash(table->entries[i].data) % new_capacity;

                membuf* newmem = (membuf*)malloc(sizeof(membuf));
                newmem->len = table->entries[i].len;
                newmem->data = (char*)malloc(newmem->len);
                memcpy(newmem->data, table->entries[i].data, newmem->len);

                insert_no_resize(&new_capacity, &new_used_size, new_entries, newmem, key);
            }
        }

        for (int i = 0; i < table->capacity; i++) {
            if (table->entries[i].data != NULL) {
                free(table->entries[i].data);
            }
        }
        free(table->entries);
        table->entries = new_entries;
        table->capacity = new_capacity;
    }
}

int main() {
    hash_table* table = create_hash_table(10);

    for (int i = 0; i < 5000000; i++) {
        membuf* mem = (membuf*)malloc(sizeof(membuf));
        int num_bytes = asprintf(&mem->data, "foo%d", i + 1);
        mem->len = num_bytes;
        insert(table, mem);
    }

    return 0;
}
