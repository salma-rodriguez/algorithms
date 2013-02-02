struct list_head *head;
struct list_head *tail;

void init(void *data);
void list_pop_head();
void list_pop_tail();
void list_push_head(void *data);
void list_push_tail(void *data);
