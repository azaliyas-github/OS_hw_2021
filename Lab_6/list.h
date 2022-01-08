#ifndef HW6_LIST_H
#define HW6_LIST_H

#include <stdlib.h>

typedef struct list {
	void** elements;
	size_t size;
	size_t capacity;
} list;

list create_list(size_t initial_capacity);
size_t append_to_list(list* list, void* element);
void free_list(list* list);

#endif //HW6_LIST_H
