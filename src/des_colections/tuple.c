#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>

#define MAX_BUCKETS 1000

typedef struct DNode_ DNode;
typedef struct DTuple_ DTuple;
typedef struct DList_ DList;
typedef enum {DSTR, DLIST, DTUPLE} DType;
typedef enum {DSTATUS_OK, DSTATUS_ER} DSTATUS;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// Util ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static char *d_str_dup(const char *str) {
  char *tmp = malloc(strlen(str));
  if (!tmp) {
    return NULL;
  }
  strcpy(tmp, str);
  return tmp;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// DNode ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct DNode_ {
  char* key;
  union {
    char* str;
    DList* list;
    DTuple* tuple;
  } value;

  DType type;
  DNode* next;
};

DNode* d_node_new_tuple(const char* key, DTuple* t);
DSTATUS d_node_destroy(DNode* n);

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// DTuple ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct DTuple_ {
  DNode* buckets[MAX_BUCKETS];
  int size;
};

DTuple* d_tuple_new();
DSTATUS d_tuple_destroy(DTuple* tuple);
DNode* d_tuple_insert_tuple(DTuple* tuple, char* key, DTuple* item);

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// DList ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

struct DList_ {
  DNode* head;
  unsigned int size;
};

DList* d_list_new();
DSTATUS d_list_destroy(DList* list);

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// DNode ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DNode* d_node_new_tuple(const char* key, DTuple* t) {
  DNode* node = (DNode*) malloc(sizeof(DNode));
  node->key = d_str_dup(key);
  node->value.tuple = t;
  node->next = NULL;
  node->type = DTUPLE;

  return node;
}

DSTATUS d_node_destroy(DNode* n){
  assert(n != NULL);
  free(n->key);
  switch (n->type) {
    case DTUPLE:
    d_tuple_destroy(n->value.tuple);
    break;
    case DLIST:
    d_list_destroy(n->value.list);
    break;
    case DSTR:
    free(n->value.str);
    break;
  }
  free(n);
  return DSTATUS_OK;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// DTuple ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DTuple* d_tuple_new() {
  DTuple* t = (DTuple*) malloc(sizeof(DTuple));
  t->size = 0;
  return t;
}

static void d_tuple_destroy_bucket(DNode* n) {
  while (n != NULL) {
    DNode* tmp = n;
    n = n->next;
    d_node_destroy(tmp);
  }
}

DSTATUS d_tuple_destroy(DTuple* tuple) {
  if (tuple->size == 0) {
    return DSTATUS_OK;
  }

  for (size_t i = 0; i < MAX_BUCKETS; ++i) {
    if (tuple->buckets[i] != NULL) {
      d_tuple_destroy_bucket(tuple->buckets[i]);
    }
  }
  free(tuple);

  return DSTATUS_OK;
}

static unsigned int d_tuple_hash(const char *key) {
  unsigned int sum = 0;
  for (int i = 0; i < strlen(key); i++) {
    sum += (unsigned int) key[i];
  }
  return sum % MAX_BUCKETS;
}

DNode* d_tuple_insert_tuple(DTuple* tuple, char* key, void* item, DType type) {

  DNode* n = d_node_new_tuple(key, item);
  if (n == NULL) {
    return NULL;
  }

  unsigned int hash_value = d_tuple_hash(key);

  if (tuple->buckets[hash_value] == NULL) {
    tuple->buckets[hash_value] = n;
    ++tuple->size;
  } else {
    n->next = tuple->buckets[hash_value];
    tuple->buckets[hash_value] = n;
    ++tuple->size;
  }

  return n;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////// DLIST ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


DList* d_list_new() {
  DList* list = (DList*) malloc(sizeof(DList));
  list->head = NULL;
  list->size = 0;
  return list;
}

DSTATUS d_list_destroy(DList* list) {
  assert(list != NULL);

  if (list->size == 0) {
    return DSTATUS_OK;
  }

  d_tuple_destroy_bucket(list->head);
  free(list);

  return DSTATUS_OK;
}

int main(int argc, char** argv) {
}
