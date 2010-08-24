/*
 * Copyright (C) cozybit, Inc. 2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 */

#include "mdns_private.h"

int mdns_list_empty(struct mdns_list *list)
{
	return (list->head == NULL) && (list->tail == NULL);
}

struct mdns_list_item *mdns_list_pop(struct mdns_list *list)
{
	struct mdns_list_item *ret;

	if (mdns_list_empty(list))
		return NULL;

	if (list->head == NULL || list->tail == NULL) {
		LOG("error: bad list state on pop\n");
		return NULL;
	}

	ret = list->head;
	if (ret->next == NULL) {
		list->head = NULL;
		list->tail = NULL;
	} else {
		list->head = ret->next;
		list->head->prev = NULL;
	}
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}

struct mdns_list_item *mdns_list_next(struct mdns_list *list,
									  struct mdns_list_item *item)
{
	if (mdns_list_empty(list))
		return NULL;

	if (item == NULL)
		return list->head;

	if (item == list->tail)
		return NULL;

	return item->next;
}

void mdns_list_push(struct mdns_list *list, struct mdns_list_item *item)
{
	item->next = NULL;
	item->prev = NULL;
	if (mdns_list_empty(list)) {
		list->tail = item;
		list->head = item;
	} else {
		if (list->head == NULL || list->tail == NULL) {
			LOG("error: bad list state on push\n");
		}
		list->tail->next = item;
		item->prev = list->tail;
		list->tail = item;
	}

	if (list->head == NULL || list->tail == NULL) {
		LOG("error: failed to push\n");
	}
}

void mdns_list_remove(struct mdns_list *list, struct mdns_list_item *item)
{
	if (mdns_list_empty(list))
		return;

	if (list->head == item && list->tail == item) {
		list->head = NULL;
		list->tail = NULL;

	} else if (list->head == item) {
		list->head = item->next;

	} else if (list->tail == item) {
		list->tail = item->prev;

	} else {
		item->prev->next = item->next;
		item->next->prev = item->prev;
	}

	item->next = NULL;
	item->prev = NULL;
}

#ifdef MDNS_TESTS
void list_tests(void)
{
	struct mdns_list l;
	struct mdns_list_item items[4], *cursor;

	test_title("list tests");
	LIST_INIT(&l);
	FAIL_UNLESS(mdns_list_empty(&l), "New list not empty");
	items[0].data = &items[0];
	items[1].data = &items[1];
	items[2].data = &items[2];
	items[3].data = &items[3];
	mdns_list_push(&l, &items[0]);
	FAIL_UNLESS(l.head == &items[0], "Failed to push item");
	cursor = mdns_list_pop(&l);
	FAIL_UNLESS(cursor == &items[0], "Failed to pop item");
	FAIL_UNLESS(mdns_list_empty(&l), "popped list is not empty");
}
#endif
