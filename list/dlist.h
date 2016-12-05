typedef char * d_type;

typedef struct dlist_struct {
    d_type val;
    struct dlist_struct *prev;
    struct dlist_struct *next;
} dlist_t;


dlist_t *dlist_ctor(d_type init_val);
void dlist_dtor(dlist_t *dlist_head);
int dlist_ok(dlist_t *dlist_head);
dlist_t *dlist_first(dlist_t *list);
dlist_t *dlist_last(dlist_t *list);
dlist_t *dlist_prepend(dlist_t *list, d_type val);
dlist_t *dlist_append(dlist_t *list, d_type val);
dlist_t *dlist_next(dlist_t *list);
dlist_t *dlist_prev(dlist_t *list);
dlist_t *dlist_insert_before(dlist_t *list, d_type val);
dlist_t *dlist_insert_after(dlist_t *list, d_type val);
dlist_t *dlist_remove(dlist_t *list);
dlist_t *dlist_search_by_val(dlist_t *list, d_type val);
dlist_t *dlist_search_by_index(dlist_t *list, int index);
void dlist_dump(dlist_t *list, FILE *stream);
void dlist_dumpdot(dlist_t *list);
unsigned int dlist_num_elements_in_list(dlist_t *list);
