#include "des_colections/des_colections.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// auxiliary function
char *des_str_dup(const char *str) {
    char *tmp = malloc(strlen(str));
    if (!tmp) {
        return NULL;
    }
    strcpy(tmp, str);
    return tmp;
}

////////////////////////////////////////////////////////////
////////////////////// Set Entry ///////////////////////////
////////////////////////////////////////////////////////////

static DesSetEntry *des_set_entry_new(const char *key) {
    DesSetEntry *entry = malloc(sizeof(DesSetEntry));
    if (!entry) {
        return NULL;
    }

    entry->key = des_str_dup(key);
    if (!entry->key) {
        return NULL;
    }

    entry->next = NULL;

    return entry;
}

static void des_set_entry_destroy(DesSetEntry *entry) {
    assert(entry != NULL);
    free(entry->key);
    free(entry);
}

////////////////////////////////////////////////////////////
///////////////////////// Set //////////////////////////////
////////////////////////////////////////////////////////////

DesSet *des_set_new(size_t capacity) {
    DesSet *set= malloc(sizeof(DesSet));
    if (!set) {
        return NULL;
    }

    set->size = 0;
    set->capacity = capacity;
    set->buckets = malloc(set->capacity * sizeof(DesSetEntry*));
    if (!set->buckets) {
        free(set);
        return NULL;
    }
    memset(set->buckets, 0, capacity * sizeof(DesSetEntry*));

    return set;
}

static void des_set_free_bucket(DesSetEntry *bucket) {
    while (bucket != NULL) {
        DesSetEntry *tmp = bucket;
        bucket = bucket->next;
        des_set_entry_destroy(tmp);
    }
}

void des_set_destroy(DesSet *set) {
    assert(set != NULL);
    /*if (set) {*/ // ====
    if (!des_set_empty(set)) {
        assert(set->buckets != NULL); // DEBUG
        assert(set->size > 0); // ====
        for (size_t i = 0; i < set->capacity; i++) {
            if (set->buckets[i] != NULL) {
                des_set_free_bucket(set->buckets[i]);
            }
        }
        free(set->buckets);
    }
    free(set);
}

int des_set_empty(const DesSet *set) {
    return set->size == 0;
}

static unsigned int des_set_hash(const DesSet *set, const char *key) {
    unsigned int sum = 0;
    for (size_t i = 0; i < strlen(key); i++) {
        sum += (unsigned int) key[i];
    }
    return sum % set->capacity;
}

DesSetEntry *des_set_insert(DesSet *set, const char *key) {

    DesSetEntry *entry = des_set_entry_new(key);
    if (!entry) {
        return NULL;
    }

    unsigned int hash_value = des_set_hash(set, key);
    if (set->buckets[hash_value] == NULL) {
        set->buckets[hash_value] = entry;
        ++set->size;
    }
    else {
        entry->next = set->buckets[hash_value];
        set->buckets[hash_value] = entry;
        ++set->size;
    }

    return entry;
}

static void des_print_list(const DesSetEntry *nlist) {
    while (nlist) {
        printf("{%s", nlist->key);
        printf("%s", "} => ");
        nlist = nlist->next;
    }
    puts("NULL");
}

void des_set_print(const DesSet *set) {
    assert(set != NULL);
    for (size_t i = 0; i < set->capacity; i++) {
        if (set->buckets[i] != NULL) {
            printf("buckets[%zu] => ", i);
            des_print_list(set->buckets[i]);
        }
        else {
            printf("buckets[%zu] => NULL\n", i);
        }
    }
}

