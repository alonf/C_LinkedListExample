#include "list_private.h"
#include "list.h"
#include <stdlib.h>

list_ptr_t to_list(void* list_ptr)
{
    return (list_ptr_t)list_ptr;
}

node_ptr_t to_node(void* node_ptr)
{
    return (node_ptr_t)node_ptr;
}

node_ptr_t list_get_head(list_ptr_t list)
{
    if (list->size == 0)
    {
        return (node_ptr_t)NULL;
    }
    return list->head_ptr;
}

node_ptr_t list_get_tail(list_ptr_t list)
{
    if (list->size == 0)
    {
        return (node_ptr_t)NULL;
    }
    node_ptr_t tail = list->head_ptr->prev_ptr;
    return tail;
}

static node_ptr_t create_node(void* data_ptr, node_ptr_t prev_ptr, node_ptr_t next_ptr)
{
    node_ptr_t node_ptr = (node_ptr_t)malloc(sizeof(*(node_ptr_t)0));
    if (node_ptr == NULL)
        return (node_ptr_t)NULL;
    
    node_ptr->data_ptr = data_ptr;
    node_ptr->prev_ptr = prev_ptr;
    node_ptr->next_ptr = next_ptr;
    return node_ptr;
}

list_t list_create(void)
{
    list_ptr_t list = (list_ptr_t)malloc(sizeof(*(list_ptr_t)0));
    if (list == NULL)
    {
        return NULL;
    }
    list->head_ptr = NULL;
    list->size = 0;
    return list;
}

int list_get_size(list_t list)
{
    return to_list(list)->size;
}

void list_destroy(list_t list, action_t* free_function)
{
    list_ptr_t list_info = to_list(list);
    while (list_info->size > 0)
    {
        if (free_function != NULL)
        {
            free_function(list_info->head_ptr->data_ptr);
        }
        list_remove_head(list);
    }
    
    free(list_info);
}

list_operation_result_t list_add_head(list_t list, void* data_ptr)
{
    list_ptr_t list_info = to_list(list);
    node_ptr_t node = create_node(data_ptr, 
        list_get_tail(list_info), 
            list_get_head(list_info));
    
    if (node == NULL)
        return LIST_FAILURE;
    
    list_info->size++;
    if (list_info->size == 1)
    {
        node->next_ptr = node;
        node->prev_ptr = node;
    }
    else
    {
        node_ptr_t tail = list_get_tail(list_info);
        list_get_head(list_info)->prev_ptr = node;
        tail->next_ptr = node;
    }
    list_info->head_ptr = node;
    return LIST_SUCCESS;
}

list_operation_result_t list_add_tail(list_t list, void* data_ptr)
{
    list_operation_result_t result =
        list_add_head(list, data_ptr);
    
    if (result == LIST_FAILURE)
        return LIST_FAILURE;
    
    to_list(list)->head_ptr = list_get_head(to_list(list))->next_ptr;
    return LIST_SUCCESS;
}

void list_remove_head(list_t list)
{
    list_ptr_t list_info = to_list(list);
    if (list_info->size == 0)
        return;

    node_ptr_t head = list_get_head(list_info);
    node_ptr_t tail = list_get_tail(list_info);
    list_info->head_ptr = head->next_ptr;
    tail->next_ptr = head->next_ptr;
    head->next_ptr->prev_ptr = tail;
    free(head);
    list_info->size--;
}

void list_remove_tail(list_t list)
{
    list_ptr_t list_info = to_list(list);
    if (list_info->size == 0)
        return;
    
    list_info->head_ptr = list_get_head(list_info)->prev_ptr;
    list_remove_head(list);
}

list_operation_result_t list_add_after(list_t list, element_t element, void* data_ptr)
{
    list_ptr_t list_info = to_list(list);
    if( element == list_info->head_ptr)
    {
        return list_add_head(list, data_ptr);
    }
    
    if (element == list_get_tail(list_info))
    {
        return list_add_tail(list, data_ptr);
    }
    
    //else
    node_ptr_t node = create_node(data_ptr,
        to_node(element),
        to_node(element)->next_ptr);

    if (node == NULL)
        return LIST_FAILURE;

    list_info->size++;
    to_node(element)->next_ptr->prev_ptr = node;
    to_node(element)->next_ptr = node;
    return LIST_SUCCESS;
}

list_operation_result_t list_add_before(list_t list, element_t element, void* data_ptr)
{
    node_ptr_t prev = to_node(element)->prev_ptr;
    return list_add_after(list, prev, data_ptr);
}

void list_remove_element(list_t list, element_t element)
{
    list_ptr_t list_info = to_list(list);
    if (element == list_info->head_ptr)
    {
        list_remove_head(list);
        return;
    }

    if (element == list_get_tail(list_info))
    {
        list_remove_tail(list);
        return;
    }
    
    node_ptr_t node = to_node(element);
    node->prev_ptr->next_ptr = node->next_ptr;
    node->next_ptr->prev_ptr = node->prev_ptr;
    free(node);
    list_info->size--;
    
}

static node_ptr_t move_next(node_ptr_t node_ptr)
{
    return node_ptr->next_ptr;
}

static node_ptr_t move_prev(node_ptr_t node_ptr)
{
    return node_ptr->prev_ptr;
}

void list_for_each_direction(list_t list, action_t* action, move_next_t* next_func)
{
    list_ptr_t list_info = to_list(list);
    node_ptr_t node = list_get_head(list_info);
    for (int i = 0; i < list_info->size; ++i)
    {
        action(node->data_ptr);
        node = next_func(node);
    }
}

void list_for_each(list_t list, action_t * action)
{
    list_for_each_direction(list, action, move_next);
}

void list_for_each_reverse(list_t list, action_t* action)
{
    list_for_each_direction(list, action, move_prev);
}

element_t list_find_element_if(list_t list, predicate_t* predicate)
{
    list_ptr_t list_info = to_list(list);
    node_ptr_t node = list_get_head(list_info);
    for (int i = 0; i < list_info->size; ++i)
    {
        if (predicate(node->data_ptr))
            return node;
        node = move_next(node);
    }
    return NULL;
}

static int data_ptr_predicate(void* data_ptr)
{
    return data_ptr == s_data_ptr;
}
element_t list_find_element(list_t list, void* data_ptr)
{
    s_data_ptr = data_ptr;
    return list_find_element_if(list, data_ptr_predicate);
}
