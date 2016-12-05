#include <stdio.h>
#include "htable.h"

int main(int argc, char **argv)
{
    int ret = 0;
	htable_t *table = ht_ctor(513);
    
    ret = fill_htable(table, "./test_text.txt");
    if (ret != 0) {
        fprintf(stderr, "Error in fill_htable function ret = %d\n", ret);
        ht_dtor(table);
        
        return -1;
    }

    ht_dump(table);
    system("gnuplot gp_script.txt");
    
    ret = sum_hashes(table);
    if (ret == -1) {
        fprintf(stderr, "Error in sum_hashes function ret = %d\n", ret);
        ht_dtor(table);
        
        return -1;
    }
    printf("Sum hashes = %u\n", ret);
    
    ht_dtor(table);
    
	return 0;
}

