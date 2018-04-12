#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "List.h"

//create a node
int free_list_helper(node** head);
int create_node(node** self, void* data, node* next, int (*free_data)(void*)){
	int ret = -1;
	if(self != NULL && data != NULL){
		if(*self == NULL){
			*self =(node*)malloc(sizeof(node));
			(*self)->data = data;
			(*self)->next = next;
			if(free_data != NULL){
				(*self)->free_data = free_data;
			}else{
				(*self)->free_data = NULL;		//	Fix for bug#02, create_node didnt set new->free_data to NULL when free_data was NULL. So the free data wasnt guaranteed to be initialized to NULL.
			}
			ret = 0;
		}
	}
	return ret;
}
// add a new node to the front of the list
int add_front(node** head, node* new_node){
	int ret = -1;
	if(head){
		if(new_node){
			new_node -> next = (*head);
			(*head) = new_node;
			ret = 0;
		}
		
	}
	return ret;
}
// add a new node to the end of the list
int add_end(node** head, node* new_node){
	int ret = -1;
	if(head){
		if((*head)){
			if(new_node){
				node* tail = (*head);
				while(tail->next)
					tail = tail->next;
				new_node -> next = tail -> next; //set new_node's next pointer to NULL
				tail -> next = new_node;
				ret = 0;
			}
		}else{
			ret = add_front(head, new_node);
		}
	}
	return ret;
}
// remove and return the node at the front of the list or NULL if empty
node* rem_front(node** head){
	//store the next node somewhere else
	node* ret = NULL;
	if(head){
		if(*head){
			ret = *head;
			(*head)=(*head) -> next;
		}
	}
	return ret;
	
}
// remove and return the node at the end of the list or NULL if empty
node* rem_end(node** head){
	node* ret = NULL;
	if(head){
		if(*head){
			if((*head)->next){
				while((*head)->next){
					head=&((*head)->next);
				}
				
			}
			ret = &(*(*head)); //hopefully make new pointer to last node
			*head = NULL; //set 2nd last node's next to null'
		}
	}
	
	return ret;
}
// return the number of nodes in the list
int list_len(const node* head){
	if(head != NULL){
		return list_len(head->next)+1;
	}
	
	return 0;
}
// print the data values of all the nodes in the list (from start to end)
int print_list(const node* head){
	int ret = -1;
	
	return ret;
}
// free a single node
int free_node(node** n){
	int ret = -1;
	if(n!=NULL){
		if((*n) != NULL){
			if((*n)->free_data != NULL){
				((*n)->free_data)((*n)->data);
			}else{
				if((*n)->data != NULL){
					free((*n)->data);
				}
			}
			free(*n);
			*n = NULL;
			ret = 0;
		}
	}
	return ret;
}

// free the entire list and set *node = NULL

int free_list(node** head){
	int ret = -1;
	breakCyclicList(head);
	ret = free_list_helper(head);
	return ret;
}
int free_list_helper(node** head){
	int ret = -1;
	if(head != NULL){
		if(*head != NULL){
			free_list_helper(&((*head)->next));
			free_node(head);
			*head=NULL;
			ret = 0;
		}
	}
	return ret;
}
int breakCyclicList(node** head){
	//	O(n) time for this method.
	int ret = -1;
	if(head != NULL){
		if(*head != NULL){
			node* current = *head;
			while(current->next != NULL && current->next != *head){
				current = current->next;
			}
			if(current->next == *head){
				ret = 0;
			}
			current->next = NULL;
		}
	}
	
	return ret;
}