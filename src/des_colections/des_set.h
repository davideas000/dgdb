#ifndef DES_COLECTIONS
#define DES_COLECTIONS

#include <stdio.h>

typedef enum {
  des_colections_found,
  des_colections_not_found
} DesColectionsResult;

////////////////////////////////////////////////////////////
////////////////////// Set Entry ///////////////////////////
////////////////////////////////////////////////////////////

typedef struct des_set_entry DesSetEntry;
struct des_set_entry {
  char *key;
  struct des_set_entry *next;
};

////////////////////////////////////////////////////////////
///////////////////////// Set //////////////////////////////
////////////////////////////////////////////////////////////

typedef struct des_set DesSet;
struct des_set {
  size_t size;
  size_t capacity;
  struct des_set_entry **buckets;
};

DesSet *des_set_new(size_t capacity);
void des_set_destroy(DesSet *set);
int des_set_empty(const DesSet *set);
DesSetEntry *des_set_insert(DesSet *set, const char *key);
DesColectionsResult des_set_lookup(const DesSet *set, const char *key);
void des_set_print(const DesSet *set);

////////////////////////////////////////////////////////////
///////////////////////// Util /////////////////////////////
////////////////////////////////////////////////////////////

char* des_str_dup(const char* str);

#endif