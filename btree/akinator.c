#include "btree2.h"


const char *akinator_filename = "./akinator_QA.txt";


int akinator(btree_t *tree)
{
	int ret = 0;
	int ans_key = -1;
	char answer[1024];
	
	printf("Type Yes or No and I try to your animal\n\n");
	
	while ((tree->type) != E_ANS) {
		printf("%s\n", tree->val);
		
		ans_key = 0;
		do {
			ret = scanf("%s", answer);
			if (ret == EOF) {
				return ERR_INPUT;
			}
			
			if (!strncmp(answer, "Yes", 3) || !strncmp(answer, "YES", 3) || !strncmp(answer, "yes", 2) || !strncmp(answer, "y", 1)) {
				tree = tree->left;
				
				ans_key = 1;
			}
			else if (!strncmp(answer, "No", 2) || !strncmp(answer, "NO", 2) || !strncmp(answer, "no", 2) || !strncmp(answer, "n", 1)) {
				tree = tree->right;
				
				ans_key = 1;
			}
			else {
				printf("Please, type Yes or No.\n");
			}
		}
		while (ans_key != 1);
	}
	
	printf("Hm, I think it is a %s\n", tree->val);
	
	return 0;
}



int main(int argc, char **argv)
{
	int ret = 0;
	
	char *btree_str = NULL;
	
	ret = wr_str_to_file(0, akinator_filename, &btree_str);
	if (ret != 0) {
		fprintf(stderr, "!!! %s !!!\n", errors[-ret].msg);
	
		free(btree_str);
	
		return -1;
	}
	
	btree_t *bt2 = btree_deserialization(btree_str, strlen(btree_str));
	
	//btree_t *bt2 = btree_deserialization("'1' '2' # # '3' # #", strlen("'1' '2' # # '3' # #"));
	btree_dumpdot(bt2);
	
	
	ret = akinator(bt2);
	if (ret != 0) {
		fprintf(stderr, "!!! %s !!!\n", errors[-ret].msg);
		
		free(btree_str);
		btree_dtor(bt2);
	
		return -1;
	}
	
    free(btree_str);
	btree_dtor(bt2);
    
    return 0;
}

