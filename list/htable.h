#ifndef HTABLE_H_
#define HTABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlist.h"

typedef struct htable_struct {
	unsigned int tsize;
	dlist_t **keys;
} htable_t;

htable_t *ht_ctor(unsigned int size);
void ht_dtor(htable_t *table);
int ht_ok(htable_t *table);
unsigned int lrotl(unsigned int value, unsigned char rotation);
unsigned int sum_hashes(htable_t *table);
int ht_hash(htable_t *table, const char *str, char f_flag);
int ht_insert(htable_t *table, d_type val);
dlist_t *ht_search_by_val(htable_t *table, d_type val);
void ht_dump(htable_t *table);
int fill_htable(htable_t *table, const char *filename);

#endif
