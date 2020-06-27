#ifndef DES_MAP_H
#define DES_MAP_H

#include "des_colections/des_util.h"
#include <stdio.h>

#define DES_MAP_TYPE_DESTROY(value) free(value)
#define DES_MAP_TYPE_COPY(value) des_str_dup(value)
#define DES_MAP_TYPE char*
#define DES_MAP_TYPE_TO_STR(out, value) fprintf(out, "%s", value)

typedef enum {
    des_map_found,
    des_map_not_found
} DesMapResult;

////////////////////////////////////////////////////////////
////////////////////// Map Entry ///////////////////////////
////////////////////////////////////////////////////////////

typedef struct des_map_entry DesMapEntry;
struct des_map_entry {
    char *key;
    DES_MAP_TYPE value;
    struct des_map_entry *next;
};

////////////////////////////////////////////////////////////
///////////////////////// Map //////////////////////////////
////////////////////////////////////////////////////////////

typedef struct des_map DesMap;
struct des_map {
    size_t size;
    size_t capacity;
    struct des_map_entry **buckets;
};

DesMap *des_map_new(size_t capacity);
void des_map_destroy(DesMap *map);
int des_map_empty(const DesMap *map);
DesMapEntry *des_map_insert(DesMap *map, const char *key, DES_MAP_TYPE value);
// DesMapResult des_map_lookup(const DesMap *map, const char *key); // ====
DesMapEntry* des_map_get(const DesMap *map, const char *key);
void des_map_print(const DesMap *map);

#endif /* ifndef DES_MAP */
