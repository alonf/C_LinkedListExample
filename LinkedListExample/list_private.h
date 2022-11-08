#pragma once

typedef struct _node_t
{
    void* data_ptr;
    struct _node_t* next_ptr;
    struct _node_t* prev_ptr;
} *node_ptr_t;

typedef struct _list_t
{
    node_ptr_t head_ptr;
    int size;
} *list_ptr_t;

typedef node_ptr_t move_next_t(node_ptr_t);

static node_ptr_t list_get_head(list_ptr_t list);
static node_ptr_t list_get_tail(list_ptr_t list);

static list_ptr_t to_list(void* list_ptr);
static node_ptr_t to_node(void* list_ptr);

static node_ptr_t create_node(void* data_ptr, node_ptr_t prev_ptr, node_ptr_t next_ptr);

static node_ptr_t move_next(node_ptr_t node_ptr);

static node_ptr_t move_prev(node_ptr_t node_ptr);

static void* s_data_ptr;
static int data_ptr_predicate(void* data_ptr);