// This is implementation of linked list by Alon Fliess
#pragma once

#define LIST_SUCCESS 1
#define LIST_FAILURE 0

typedef int BOOL;

typedef void* list_t;
typedef void* element_t;
typedef int predicate_t(void* data_ptr);
typedef void action_t(void* data_ptr);

list_t		list_create(void);

int			list_get_size(list_t list);
void		list_destroy(list_t list, action_t* free_function);

BOOL		list_add_head(list_t list, void* data_ptr);
BOOL		list_add_tail(list_t list, void* data_ptr);

void*		list_get_head_data(list_t list);
void*		list_get_tail_data(list_t list);

void		list_remove_head(list_t list);
void		list_remove_tail(list_t list);

BOOL		list_add_after(list_t list, element_t element, void* data_ptr);
BOOL		list_add_before(list_t list, element_t element, void* data_ptr);

void		list_remove_element(list_t list, element_t element);

void		list_for_each(list_t list, action_t* action);
void		list_for_each_reverse(list_t list, action_t* action);

element_t	list_find_element(list_t list, void* data_ptr);
element_t	list_find_element_if(list_t list, predicate_t* predicate);
