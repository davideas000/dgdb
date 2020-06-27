#include "des_colections/des_map.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

////////////////////////////////////////////////////////////
////////////////////// Map Entry ///////////////////////////
////////////////////////////////////////////////////////////

static DesMapEntry *des_map_entry_new(const char *key, DES_MAP_TYPE value) {
  DesMapEntry *entry = malloc(sizeof(DesMapEntry));
  if (!entry) {
    return NULL;
  }

  entry->key = des_str_dup(key);
  if (!entry->key) {
    return NULL;
  }

  #ifdef DES_MAP_TYPE_COPY
  entry->value = DES_MAP_TYPE_COPY(value);
  #else
  entry->value = value; // FIXME
  #endif /* ifndef DES_MAP_TYPE_COPY */

  entry->next = NULL;

  return entry;
}

static void des_map_entry_destroy(DesMapEntry *entry) {
  assert(entry != NULL);
  //if (entry) { // ====
  free(entry->key);

  #ifdef DES_MAP_TYPE_DESTROY
  DES_MAP_TYPE_DESTROY(entry->value); // FIXME
  #endif

  free(entry);
  //} // ====
}

////////////////////////////////////////////////////////////
///////////////////////// Map //////////////////////////////
////////////////////////////////////////////////////////////

DesMap *des_map_new(size_t capacity) {
  DesMap *map = malloc(sizeof(DesMap));
  if (!map) {
    return NULL;
  }

  map->size = 0;
  map->capacity = capacity;
  map->buckets = malloc(map->capacity * sizeof(DesMapEntry*));
  if (!map->buckets) {
    free(map);
    return NULL;
  }
  memset(map->buckets, 0, capacity * sizeof(DesMapEntry*));
  //testb
  for (size_t i = 0; i < map->capacity; i++) {
    assert(map->buckets[i] == NULL);
  }
  //teste
  return map;
}

static void des_map_free_bucket(DesMapEntry *bucket) {
  while (bucket != NULL) {
    DesMapEntry *tmp = bucket;
    bucket = bucket->next;
    des_map_entry_destroy(tmp);
  }
}

void des_map_destroy(DesMap *map) {
  assert(map != NULL);
  /*if (map) {*/ // ====
  if (!des_map_empty(map)) {
    assert(map->buckets != NULL); // DEBUG
    assert(map->size > 0); // ====
    for (size_t i = 0; i < map->capacity; i++) {
      if (map->buckets[i] != NULL) {
        des_map_free_bucket(map->buckets[i]);
      }
    }
    free(map->buckets);
  }
  free(map);
  map = NULL; // ===
  /*}*/ // ====
}

int des_map_empty(const DesMap *map) {
  return map->size == 0;
}

static unsigned int des_map_hash(const DesMap *map, const char *key) {
  unsigned int sum = 0;
  for (size_t i = 0; i < strlen(key); i++) {
    sum += (unsigned int) key[i];
  }
  return sum % map->capacity;
}

DesMapEntry *des_map_insert(DesMap *map, const char *key, DES_MAP_TYPE value) {

  DesMapEntry *entry = des_map_entry_new(key, value);
  if (!entry) {
    return NULL;
  }

  unsigned int hash_value = des_map_hash(map, key);
  if (map->buckets[hash_value] == NULL) {
    map->buckets[hash_value] = entry;
    ++map->size;
  }
  else {
    //assert(map->buckets[hash_value] != NULL); // ====
    entry->next = map->buckets[hash_value];
    map->buckets[hash_value] = entry;
    ++map->size;
  }

  return entry;
}

// DesMapResult des_map_lookup(const DesMap *map, const char *key) {
//   unsigned int hash_value = des_map_hash(map, key);
//   if (map->buckets[hash_value]) {
//     DesMapEntry *cur = map->buckets[hash_value];
//     //while(!strcmp(cur->key, key) && cur) {
//     while(cur) {
//       if (strcmp(cur->key, key))
//       return des_map_found;
//       cur = cur->next;
//     }
//     //return cur->value;
//   }
//   return des_map_not_found;
// }

DesMapEntry* des_map_get(const DesMap *map, const char *key) {
  unsigned int hash_value = des_map_hash(map, key);
  if (map->buckets[hash_value]) {
    DesMapEntry *cur = map->buckets[hash_value];

    while(cur) {
      if (strcmp(cur->key, key) == 0) {
        return cur;
      }
      cur = cur->next;
    }

  }

  return NULL;
}

// #ifndef DES_MAP_TYPE_TO_STR
// #define DES_MAP_TYPE_TO_STR(out, value) double_to_s(out, value)
// //#define DES_MAP_TYPE_TO_STR(out, value) s_to_s(out, value)
//
// static void double_to_s(FILE *out, double value) {
//   //static void s_to_s(FILE *out, const char *value) {
//   //fprintf(out, "%s", value);
//   fprintf(out, "%.2f", value);
// }
//
// #endif

static void des_print_list(const DesMapEntry *nlist) {
  while (nlist) {
    printf("{%s, ", nlist->key);
    DES_MAP_TYPE_TO_STR(stdout, nlist->value);
    printf("%s", "} => ");
    nlist = nlist->next;
  }
  puts("NULL");
}

void des_map_print(const DesMap *map) {
  assert(map != NULL);
  assert(map->size > 0);
  for (size_t i = 0; i < map->capacity; i++) {
    if (map->buckets[i] != NULL) {
      printf("buckets[%zu] => ", i);
      des_print_list(map->buckets[i]);
    }
    else {
      printf("buckets[%zu] => NULL\n", i);
    }
  }
}
