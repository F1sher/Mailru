#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include "dlist.h"


dlist_t *dlist_ctor(d_type init_val)
{
    dlist_t *alloc_dlist = NULL;
    
    alloc_dlist = (dlist_t *)malloc(sizeof(dlist_t));
    if (alloc_dlist == NULL) {
        return NULL;
    }
    
    alloc_dlist->val = init_val;
    alloc_dlist->prev = NULL;
    alloc_dlist->next = NULL;
    
    return alloc_dlist;
}

void dlist_dtor(dlist_t *dlist_head)
{
    if (dlist_ok(dlist_head)) {
        dlist_t *el_cur = dlist_head;
        dlist_t *el_p = NULL;
        dlist_t *el_n = dlist_head->next;
        
        while (el_n != NULL) {
            el_p = el_n;
            el_n = el_n->next;

            free(el_p);
        }
        
        el_n = dlist_head->prev;
        while (el_n != NULL) {
            el_p = el_n;
            el_n = el_n->prev;
            
            free(el_p);
        }
    }
}

int dlist_ok(dlist_t *list)
{
	if (list == NULL) {
		return 0;
	}

	dlist_t *t_dlist = list;
	
	//find last
	while (t_dlist->next != NULL) {
		t_dlist = t_dlist->next;
	}
	
	while (t_dlist != list) {
		t_dlist = t_dlist->prev;
		
		if (t_dlist == NULL) {
			return 0;
		}
	}
	
	//find first
	while (t_dlist->prev != NULL) {
		t_dlist = t_dlist->prev;
	}
	
	while (t_dlist != list) {
		t_dlist = t_dlist->next;
		
		if (t_dlist == NULL) {
			return 0;
		}
	}
	
	return 1;
}

dlist_t *dlist_first(dlist_t *list)
{
    dlist_t *t_dlist = list;
    
    if (dlist_ok(list)) {
        while (t_dlist->prev != NULL) {
            t_dlist = t_dlist->prev;
        }
        
        return t_dlist;
    }
    
    return NULL;
}

dlist_t *dlist_last(dlist_t *list)
{
    dlist_t *t_dlist = list;
    
    if (!dlist_ok(list)) {
        return NULL;
    }
    
    while (t_dlist->next != NULL) {
        t_dlist = t_dlist->next;
    }
    
    return t_dlist;
}

dlist_t *dlist_prepend(dlist_t *list, d_type val)
{
    dlist_t *new_dlist = dlist_ctor(val);
    dlist_t *first_in_list = NULL;
    
    if (dlist_ok(new_dlist) && dlist_ok(list)) {
        first_in_list = dlist_first(list);
        new_dlist->next = first_in_list;
        
        list->prev = new_dlist;
        
        return new_dlist;
    }
    
    dlist_dtor(new_dlist);
    return NULL;
}

dlist_t *dlist_append(dlist_t *list, d_type val)
{
    dlist_t *new_dlist = dlist_ctor(val);
    dlist_t *last_in_list = NULL;
    
    if (dlist_ok(new_dlist) && dlist_ok(list)) {
        last_in_list = dlist_last(list);
        
        last_in_list->next = new_dlist;
        new_dlist->prev = last_in_list;
        
        return new_dlist;
    }
    
    dlist_dtor(new_dlist);
    return NULL;
}

dlist_t *dlist_next(dlist_t *list)
{
    if (!dlist_ok(list)) {
        return NULL;
    }

    return list->next;
}

dlist_t *dlist_prev(dlist_t *list)
{
    if (!dlist_ok(list)) {
        return NULL;
    }

    return list->prev;
}

dlist_t *dlist_insert_before(dlist_t *list, d_type val)
{
    dlist_t *new_dlist = dlist_ctor(val);
    
    if (dlist_ok(new_dlist) && dlist_ok(list)) {
        new_dlist->prev = list->next;
        new_dlist->next = list;
        
        if (list->prev != NULL) {
            (list->prev)->next = new_dlist; 
        }
        list->prev = new_dlist;
        
        return new_dlist;
    }
    
    dlist_dtor(new_dlist);
    return NULL;
}

dlist_t *dlist_insert_after(dlist_t *list, d_type val)
{
    dlist_t *new_dlist = dlist_ctor(val);
    
    if (dlist_ok(new_dlist) && dlist_ok(list)) {
        new_dlist->prev = list;
        new_dlist->next = list->next;
       
        if (list->next != NULL) {
            (list->next)->prev = new_dlist;
        }
        list->next = new_dlist;
        
        return new_dlist;
    }
    
    dlist_dtor(new_dlist);
    return NULL;
}

dlist_t *dlist_remove(dlist_t *list)
{
    if (dlist_ok(list)) {
        if (list->prev != NULL) {
            (list->prev)->next = list->next;
        }
        
        if (list->next != NULL) {
            (list->next)->prev = list->prev;
        }
        
        free(list);
    }

    return NULL;
}

dlist_t *dlist_search_by_val(dlist_t *list, d_type val)
{
    dlist_t *t_list = NULL;
    
    if (dlist_ok(list)) {
        t_list = list;
        while (t_list != NULL) {
            if ( strncmp(t_list->val, val, strlen(t_list->val)) ) {
                return t_list;
            }
            
            t_list = t_list->next;
        }
        
        
        t_list = list;
        while (t_list != NULL) {
            if ( strncmp(t_list->val, val, strlen(t_list->val)) ) {
                return t_list;
            }
            
            t_list = t_list->prev;
        }
    }
    
    return NULL;
}

dlist_t *dlist_search_by_index(dlist_t *list, int index)
{
    int i = 0;
    dlist_t *t_list = NULL;

    if (dlist_ok(list)) {
        t_list = dlist_first(list);
        
        while ((i != index) && (t_list != NULL)) {
            i++;
            t_list = t_list->next;
        }
    }
    
    return t_list;
}

void dlist_dump(dlist_t *list, FILE *stream)
{
    if ((!dlist_ok(list)) || stream == NULL) {
        return ;
    }
    
    dlist_t *t_list = dlist_first(list);
    
    while (t_list != NULL) {
        fprintf(stream, "%s -> ", t_list->val);
        t_list = t_list->next;
    }
    fprintf(stream, "\n");
}

void dlist_dumpdot(dlist_t *list)
{
	if (!dlist_ok(list)) {
        return ;
    }
    
    dlist_t *t_list = dlist_first(list);
    FILE *fout_dot = fopen("./graphviz_code.dot", "w+");

	if (fout_dot == NULL) {
		fprintf(stderr, "fout_dot in NULL in %s function", __FUNCTION__);
		
		return ;
	}
    
    fprintf(fout_dot, "digraph {\n");
    fprintf(fout_dot, "\t\trankdir=LR;\n");
    if ((t_list->next == NULL) && (t_list->prev == NULL)) {
		fprintf(fout_dot, "\t\t%s;\n", t_list->val);
	}
    
    while (t_list->next != NULL) {
        fprintf(fout_dot, "\t\t%s -> %s;\n", t_list->val, t_list->next->val);
        
        t_list = t_list->next;
    }
    while (t_list->prev != NULL) {
		fprintf(fout_dot, "\t\t%s -> %s;\n", t_list->val, t_list->prev->val);
    
		t_list = t_list->prev;
	}
    
    fprintf(fout_dot, "}\n");
    
    fclose(fout_dot);
    
    system("dot -Tpng ./graphviz_code.dot -o ./graphviz.png");
    system("xviewer graphviz.png");
}

unsigned int dlist_num_elements_in_list(dlist_t *list)
{
    unsigned int num = 0;
    dlist_t *t_list = list;
    
    if (!dlist_ok(list)) {
        return 0;
    }
    
    while (t_list->prev != NULL) {
        t_list = t_list->prev;
    }
    
    while (t_list != NULL) {
        num++;
        t_list = t_list->next;
    }
    
    return num;
}

/*
int main(int argc, char **argv)
{
    dlist_t *num_dlist = NULL;
    dlist_t *temp_dlist = NULL;
    
    num_dlist = dlist_ctor("-1");
    dlist_prepend(num_dlist, "-2");
    temp_dlist = dlist_append(num_dlist, "10");
    dlist_insert_before(temp_dlist, "c");
    temp_dlist = dlist_insert_after(temp_dlist, "b");
    dlist_remove(temp_dlist);
    
    printf(">= [0]:\n");
    temp_dlist = num_dlist;
    while (temp_dlist != NULL) {
        printf("val = %s\n", temp_dlist->val);
        
        temp_dlist = temp_dlist->next;
    }
    
    printf("< [0]:\n");
    temp_dlist = num_dlist->prev;
    while (temp_dlist != NULL) {
        printf("val = %s\n", temp_dlist->val);
        
        temp_dlist = temp_dlist->prev;
    }
    /*
    temp_dlist = dlist_search_by_val(num_dlist, 10);
    printf("ptr = %p, val = %d\n", temp_dlist, temp_dlist->val);
    
    temp_dlist = dlist_search_by_index(num_dlist, 4);
    printf("ptr = %p, val = %d\n", temp_dlist, temp_dlist->val);
    
    /*
    temp_dlist = dlist_last(num_dlist);
    printf("last val = %d\n", temp_dlist->val);
    */
/*   
	return 0;
}
*/
