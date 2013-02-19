#ifndef __BAG_H
#define __BAG_H

typedef any_t (*bpeek_t)(struct bag *);
typedef void (*bpush_t)(any_t, struct bag *);

struct bag {
	struct dequeue *dec;
	bpush_t list_push_bag;
	bpeek_t list_peek_bag;
};

struct bag *create_bag(struct bag *);
struct bag *destroy_bag(struct bag *);

#endif /* __BAG_H */
