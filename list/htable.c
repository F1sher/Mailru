#include "htable.h"

//hash_flag = 0..5 for choosing the hash function
const char hash_flag = 5;
const char *dump_filename = "./table_dump.txt";

htable_t *ht_ctor(unsigned int size)
{
	unsigned int i = 0, j = 0;
	
	if (size == 0) {
		return NULL;
	}
	
	htable_t *new_table = (htable_t *)malloc(sizeof(htable_t));
	if (new_table == NULL) {
		return NULL;
	}
	
	new_table->keys = (dlist_t **)calloc(sizeof(dlist_t *), size);
	if (new_table->keys == NULL) {
		free(new_table);
		
		return NULL;
	}
	
	for (i = 0; i <= size-1; i++) {
		new_table->keys[i] = dlist_ctor("");
	
		if (new_table->keys[i] == NULL) {
			for (j = 0; j <= i-1; j++) {
				dlist_dtor(new_table->keys[i]);
			}
			
			free(new_table->keys);
			free(new_table);
			
			return NULL;
		}
		//if new_table->keys[i] == NULL ...
	}
	
	new_table->tsize = size;
	
	return new_table;
}

void ht_dtor(htable_t *table) 
{
	unsigned int i = 0;
	
	for (i = 0; i <= table->tsize-1; i++) {
		dlist_dtor(table->keys[i]);
	}

	free(table->keys);
	table->keys = NULL;
	free(table);
	table = NULL;
}

int ht_ok(htable_t *table)
{
	unsigned int i = 0;
	int ret = 0;
	
	if ((table == NULL) || (table->keys == NULL)) {
		return 0;
	}
	
	for (i = 0; i <= table->tsize-1; i++) {
		ret = dlist_ok(table->keys[i]);
	
		if (ret != 1) {
				return 0;
		}
	}
	
	return 1;
}

unsigned int lrotl(unsigned int value, unsigned char rotation)
{
    const unsigned int cBitsInByte=8; // implementation-defined. It is 8 for most platforms.
    return (value<<rotation) | (value>>(sizeof(value)*cBitsInByte - rotation));
}

unsigned int sum_hashes(htable_t *table)
{
    if (!ht_ok(table)) {
        return -1;
    }
    
    unsigned int ret = 0, i = 0;
    
    for (i = 0; i <= table->tsize-1; i++) {
        ret += dlist_num_elements_in_list(table->keys[i]);
    }
    
    return ret;
}

int ht_hash(htable_t *table, const char *str, char f_flag)
{
    int ret = 0;
    int i = 0;
    int len = strlen(str);
    
    if (f_flag == 0) {
        return ret;
    }
    else if (f_flag == 1) {
        return (str[2] % table->tsize);  
    }
    else if (f_flag == 2) {
        return strlen(str) % table->tsize;
    }
    else if (f_flag == 3) {
        for (i = 0; i <= len-1; i++) {
            ret += str[i];
        }
        
        return ret % table->tsize;
    }
    else if (f_flag == 4) {
        unsigned int h_i_1 = 0;
        unsigned int h_i = 0;
        
        for (i = 1; i <= len-1; i++) {
            h_i = lrotl(h_i_1, 1) ^ str[i];
            h_i_1 = h_i;
            
            ret += h_i;
        }
    }
    else if (f_flag == 5) {
        static const unsigned int b = 378551;
        unsigned int a = 63689;
        unsigned int hash = 0;

        for(; *str; str++)
        {
            ret = ret * a + (unsigned char)(*str);
            a *= b;
        }
        
        return ret % table->tsize;
    }
}

int ht_insert(htable_t *table, d_type val)
{
    if ((ht_ok(table) != 1) || (val == NULL)) {
        return -1;
    }

    int hash = ht_hash(table, val, hash_flag);
    if ((hash < 0) || (hash >= table->tsize)) {
        return -1;
    }

    //printf("hash = %d, val = %s\n", hash, val);

    dlist_t *ret = dlist_append(table->keys[hash], val);

    //printf("dlist.val = %s\n", ret->val);

    if (ret != NULL) {
        return 0;
    }
    
    return -1;
}

dlist_t *ht_search_by_val(htable_t *table, d_type val)
{
    if ((ht_ok(table) != 1) || (val == NULL)) {
        return NULL;
    }
    
    int hash = ht_hash(table, val, hash_flag);
    if ((hash < 0) || (hash >= table->tsize)) {
        return NULL;
    }
    
    return dlist_search_by_val(table->keys[hash], val);
}

void ht_dump(htable_t *table)
{
    if (ht_ok(table) != 1) {
        return ;
    }
    
    unsigned int i = 0;
    FILE *fp_out = fopen(dump_filename, "w+");
    if (fp_out == NULL) {
        return ;
    }

    /*
    for (i = 0; i <= table->tsize-1; i++) {
        dlist_dump(table->keys[i], fp_out);
    }    
    fprintf(fp_out, "\n\n\n");
    */
    
    for (i = 0; i <= table->tsize-1; i++) {
        fprintf(fp_out, "%d %d\n", i, dlist_num_elements_in_list(table->keys[i]));
    }
    
    fclose(fp_out);
}

int fill_htable(htable_t *table, const char *filename)
{
    FILE *fp_in = fopen(filename, "r");
    ssize_t f_size = 0;
    char *text = NULL;
    if (fp_in == NULL) {
        return -1;
    }
    unsigned int lines = 0, i = 0;
    
    //fseek(fp_in, 0L, SEEK_END);
    //f_size = ftell(fp_in);
    //rewind(fp_in);
    
    while (!feof(fp_in)) {
        if (fgetc(fp_in) == '\n') {
            lines++;
        }
    }
    f_size = ftell(fp_in);
    rewind(fp_in);
    
    if (f_size == 0) {
        fclose(fp_in);
    
        return -2;
    }
    
    text = (char *)calloc(f_size, sizeof(char));
    if (text == NULL) {
        fclose(fp_in);
    
        return -3;
    }
    
    fread(text, sizeof(char), f_size, fp_in);
    
    char *word = strtok(text, "\n");
    while (word != NULL) {
        ht_insert(table, word);
    
        word = strtok(NULL, "\n");
        i++;
    }
    
    //free(text);
    fclose(fp_in);

    return 0;
}
