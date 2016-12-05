#include "btree2.h"


btree_t *btree_ctor(b_type val)
{
	btree_t *alloc_btree = NULL;
	
	alloc_btree = (btree_t *)malloc(sizeof(btree_t));
	if (alloc_btree == NULL) {
		return NULL;
	}
	
	alloc_btree->val = val;
	if (val[strlen(val)-1] == '?') {
		alloc_btree->type = E_QUE;
	}
	else {
		alloc_btree->type = E_ANS;
	}
	alloc_btree->left = NULL;
	alloc_btree->right = NULL;
	
	return alloc_btree;
}

void btree_dtor(btree_t *tree)
{
	if (!btree_ok(tree)) {
		return ;
	}

	btree_dtor(tree->left);
	btree_dtor(tree->right);
	
	free(tree->val);
	free(tree);
	tree = NULL;
}

int btree_ok(btree_t *tree)
{
	if (tree == NULL) {
		return 0;
	}
	
	return 1;
}

btree_t *btree_postorder(btree_t *tree) 
{
	if (!btree_ok(tree)) {
		return NULL;
	}

	btree_postorder(tree->left);
	btree_postorder(tree->right);
	
	return tree;
}

void btree_dump_normal_tree(btree_t *tree, FILE *fout_dump)
{
	unsigned static int counter = 0;
	
	if (!btree_ok(tree)) {
		return ;
	}
	
	if (tree->left != NULL) {
		fprintf(fout_dump, "\t\"%s\" -> \"%s\";\n", tree->val, tree->left->val);
		btree_dump_normal_tree(tree->left, fout_dump);
	}
	else {
		fprintf(fout_dump, "\tnull%u [shape=point];\n", counter);
		fprintf(fout_dump, "\t\"%s\" -> null%u;\n", tree->val, counter);
		
		counter++;
	}
	
	if (tree->right != NULL) {
		fprintf(fout_dump, "\t\"%s\" -> \"%s\";\n", tree->val, tree->right->val);
		btree_dump_normal_tree(tree->right, fout_dump);
	}
	else {
		fprintf(fout_dump, "\tnull%u [shape=point];\n", counter);
		fprintf(fout_dump, "\t\"%s\" -> null%u;\n", tree->val, counter);
		
		counter++;
	}
}
 
void btree_dumpdot(btree_t *tree)
{
	if (!btree_ok(tree)) {
		return ;
	}
	
	FILE *fout_dump = fopen("./btree_dump.dot", "w+");
	if (fout_dump == NULL) {
		return ;
	}
	
	fprintf(fout_dump, "digraph BTREE {\n");
	fprintf(fout_dump, "\tnode [fontname=\"Arial\"];\n");
	
	if ((tree->left == NULL) && (tree->right == NULL)) {
		fprintf(fout_dump, "\t\"%s\";\n", tree->val);
	}
	else {
		btree_dump_normal_tree(tree, fout_dump);
	}
	
	fprintf(fout_dump, "}");
	
	fclose(fout_dump);
	
	system("dot -Tpng ./btree_dump.dot -o ./btree_dump.png");
    system("xviewer ./btree_dump.png");
}

unsigned int get_val_len(char *btree_str, unsigned int *index)
{
	if (btree_str[*index] == '#') {
		return 1;
	}
	else {
		unsigned int i = 0;
		
		if (btree_str[*index] == '\'') {
			(*index)++;
		}
		
		i = *index;
		while (btree_str[*index] != '\'') {
			(*index)++;
		}
		
		unsigned int ret = *index - i;
		//(*index) += 2;
		
		return ret;
	}
}

btree_t *btree_deserialization(char *btree_str, ssize_t btree_str_len)
{
	static unsigned int index = 1;
	
	if ((index >= btree_str_len) || (btree_str[index] == '#')) {
		return NULL;
	}
	
	unsigned int val_len = 0;
	char *s1 = calloc(1024, sizeof(char));
	if (s1 == NULL) {
		return NULL;
	}
	
	val_len = get_val_len(btree_str, &index);
	//printf("val_len = %d, s1 = %p, btree_str + index - val_len = %s\n", val_len, s1, btree_str + index - val_len);
	strncpy(s1, btree_str + index - val_len, val_len);
	btree_t *leaf = btree_ctor(s1);
	
	index += 2;
	leaf->left = btree_deserialization(btree_str, btree_str_len);
	
	val_len = get_val_len(btree_str, &index);
	index += 2;
	leaf->right = btree_deserialization(btree_str, btree_str_len);
	
	return leaf;
}

void btree_serialization(btree_t *tree, FILE *out_fp)
{
	if (!btree_ok(tree)) {
		fprintf(out_fp, "# ");
		
		return ;
	}
	
	fprintf(out_fp, "'%s' ", tree->val);
	btree_serialization(tree->left, out_fp);
	btree_serialization(tree->right, out_fp);
}

int wr_str_to_file(int read_write_flag, const char *filename, char **str)
{
	int ret = 0;
	FILE *fp_in = NULL;
	unsigned int file_size = 0;
	
	if (filename == NULL) {
		return ERR_FUNC_PARAMS;
	}
	
	if (read_write_flag == 0) {
		//READ
		if ((*str) != NULL) {
			return ERR_FUNC_PARAMS;
		}
		
		fp_in = fopen(filename, "r");
		if (fp_in == NULL) {
			return ERR_FOPENCLOSE;
		}
		
		fseek(fp_in, 0L, SEEK_END);
		file_size = ftell(fp_in)/sizeof(char);
		rewind(fp_in);
		
		(*str) = (char *)malloc((file_size+1)*sizeof(char));
		if ((*str) == NULL) {
			fclose(fp_in);
			
			return ERR_ALLOC;
		}
		
		//ret = fgets(*str, sizeof(char)*file_size, fp_in);
		if (fgets(*str, (file_size)*sizeof(char), fp_in) == NULL) {
			free(*str);
			fclose(fp_in);
		
			return ERR_FREAD;
		}
		
		fclose(fp_in);
	}
	else if (read_write_flag == 1) {
		//WRITE
		if ((*str) == NULL) {
			return ERR_FUNC_PARAMS;
		}
		
		FILE *fp_out = fopen(filename, "w+");
		if (fp_out == NULL) {
			return ERR_FOPENCLOSE;
		}
		
		ret = fwrite(*str, sizeof(char), strlen(*str), fp_out);
		if (ret < strlen(*str)) {
			fclose(fp_out);
			
			return ERR_FWRITE;
		}
		
		fclose(fp_out);
	}
	else {
		return ERR_FUNC_PARAMS;
	}
	
	
	return 0;
}


btree_t *btree_search_val(btree_t *tree, char *search_val)
{
	static btree_t *ret = NULL;
	
	if (!btree_ok(tree)) {
		return NULL;
	}

	if (strcmp(tree->val, search_val) == 0) {
		ret = tree;
	}

	btree_search_val(tree->left, search_val);
	btree_search_val(tree->right, search_val);
	
	return ret;
}

btree_t *btree_search_branch_tree(btree_t *tree, btree_t *search_tree)
{
	static btree_t *ret = NULL;
	
	if (!btree_ok(tree) || !btree_ok(search_tree)) {
		return NULL;
	}

	if ((tree->left == search_tree) || (tree->right == search_tree)) {
		ret = tree;
	}

	btree_search_branch_tree(tree->left, search_tree);
	btree_search_branch_tree(tree->right, search_tree);
	
	return ret;
}

int btree_print_props(btree_t *tree, char *search_val)
{
	char temp_str[1024] = {'\0'};
		
	if (!btree_ok(tree)) {
		return -1;
	}
	
	btree_t *bt = btree_search_val(tree, search_val);
	if (!btree_ok(bt)) {
		return -1;
	}
	
	printf("%s is ", bt->val);
	while (bt != tree) {
		bt = btree_search_branch_tree(tree, bt);
		
		if (!btree_ok(bt)) {
			return -1;
		}
		
		if (strlen(bt->val) > 1024) {
			return -1;
		}
		
		printf("%s", strncpy(temp_str, bt->val, strlen(bt->val)-1));
		memset(temp_str, '\0', 1024);
		
		if (bt != tree) {
			printf(" and ");
		} 
	}
	printf("\b\n");
	
	return 0;
}
