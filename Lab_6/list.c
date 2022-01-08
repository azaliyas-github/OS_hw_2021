#include "list.h"

list create_list(size_t initial_capacity) {
	list result;
	result.size = 0;
	result.capacity = initial_capacity;
	result.elements = malloc(sizeof(void*) * initial_capacity);
	return result;
}

size_t append_to_list(list* list, void* element) {
	if (list->size == list->capacity) {
		list->capacity *= 2;
		list->elements = realloc(list->elements, sizeof(void*) * list->capacity);
	}

	list->elements[list->size] = element;
	return ++list->size;
}

void free_list(list* list) {
	list->size = list->capacity = 0;
	free(list->elements);
	list->elements = NULL;
}
