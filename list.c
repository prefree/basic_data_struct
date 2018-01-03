#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "list.h"

struct cc_ops {
	struct list_head  list;
    int               ref;
	unsigned char     text;
};

struct cc_ops cc_ctx;
int main()
{
	int i;
	char text[10] = "dlrowolleh";
	struct cc_ops *tmp, *max=NULL;
	struct list_head *pos, *q;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srandom(tv.tv_sec*1000+tv.tv_usec/1000);
	INIT_LIST_HEAD(&cc_ctx.list);
	for (i=0; i<20; i++) {
		tmp = malloc(sizeof(struct cc_ops));
		tmp->ref = random()%100;
		printf("input->%d\n", tmp->ref);
		if (list_empty(&cc_ctx.list)) {
		    list_add_tail(&tmp->list, &cc_ctx.list);
		} else {
	        list_for_each(pos, &cc_ctx.list) {
		        max = list_entry(pos, struct cc_ops, list);
			    if (max->ref > tmp->ref) {
		           list_add_tail(&tmp->list, &max->list);
				   break;
			    }
	        }
			if (max->ref <= tmp->ref)
		        list_add_tail(&tmp->list, &cc_ctx.list);
	    }
	}
	printf("==============================================\n");
	list_for_each(pos, &cc_ctx.list) {
		tmp = list_entry(pos, struct cc_ops, list);
		printf("result->%d\n", tmp->ref);
	}
	list_for_each_safe(pos, q, &cc_ctx.list) {
		tmp = list_entry(pos, struct cc_ops, list);
		list_del(pos);
		free(tmp);
	}
    return 0;
}

