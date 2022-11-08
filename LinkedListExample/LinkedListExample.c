// LinkedListExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void print(void* data)
{
    printf("%d ", (int)data);
}

int main()
{
    list_t list = list_create();
    list_add_head(list, (void*)2);
    list_add_head(list, (void*)1);
    list_add_tail(list, (void*)3);
    list_add_tail(list, (void*)6);
    
    element_t element = list_find_element(list, (void*)3);
    list_add_after(list, element, (void*)4);
    element = list_find_element(list, (void*)6);
    list_add_before(list, element, (void*)5);
    
    list_for_each(list, print);
    printf("\n");
    
    list_remove_head(list);
    list_for_each(list, print);
    printf("\n");
    
    list_remove_tail(list);
    list_for_each(list, print);
    printf("\n");
    
    element = list_find_element(list, (void*)4);
    list_remove_element(list, element);
    list_for_each(list, print);
    printf("\n");
    
    list_destroy(list, NULL);
    
    return 0;
    
}
