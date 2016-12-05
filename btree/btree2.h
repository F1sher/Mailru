#ifndef BTREE_H_
#define BTREE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "errors_btree.h"

enum {
	E_NUL = 0,
	E_QUE = 1,
	E_ANS = 2,
};

typedef char * b_type;

typedef struct btree_struct {
    b_type val;
    unsigned char type;
    struct btree_struct *left;
    struct btree_struct *right;
} btree_t;

btree_t *btree_ctor(b_type val);
void btree_dtor(btree_t *tree);
int btree_ok(btree_t *tree);
btree_t *btree_postorder(btree_t *tree);
void btree_dump_normal_tree(btree_t *tree, FILE *fout_dump);
void btree_dumpdot(btree_t *tree);
unsigned int get_val_len(char *btree_str, unsigned int *index);
btree_t *btree_deserialization(char *btree_str, ssize_t btree_str_len);
void btree_serialization(btree_t *tree, FILE *out_fp);
int wr_str_to_file(int read_write_flag, const char *filename, char **str);
btree_t *btree_search_branch_tree(btree_t *tree, btree_t *search_tree);
int btree_print_props(btree_t *tree, char *search_val);


#endif
