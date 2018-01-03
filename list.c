#include <stdio.h>
#include <stdlib.h>
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
	struct cc_ops *tmp, *min;
	struct list_head *pos, *q;
	INIT_LIST_HEAD(&cc_ctx.list);
	for (i=0; i<10; i++) {
		tmp = malloc(sizeof(struct cc_ops));
		tmp->text = text[i];
		tmp->ref = 9-i;
		/*
		if (list_empty(&cc_ctx.list)) {
		    list_add_tail(&tmp->list, &cc_ctx.list);
		} else {
	        list_for_each(pos, &cc_ctx.list) {
		        max = list_entry(pos, struct cc_ops, list);
			    if (max->ref > tmp->ref) {
				   break;
			    }
	        }
		    list_add_tail(&tmp->list, &cc_ctx.list);
	}
		*/
		list_add_tail(&tmp->list, &cc_ctx.list);
	}
	list_for_each(pos, &cc_ctx.list) {
		tmp = list_entry(pos, struct cc_ops, list);
		printf("%d\n", tmp->ref);
	}
	list_for_each_safe(pos, q, &cc_ctx.list) {
		tmp = list_entry(pos, struct cc_ops, list);
		list_del(pos);
		free(tmp);
	}
    return 0;
}

