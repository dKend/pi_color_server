// a recursive data type definition
// serves as the nodes of a list
typedef struct node{
	void* data;
	struct node* next;
	int (*free_data)(void*);
}node;
//create a node
int create_node(node** self, void* data, node* next, int (*free_data)(void*));
// add a new node to the front of the list
int add_front(node** head, node* new_node);
// add a new node to the end of the list
int add_end(node** head, node* new_node);
// remove and return the node at the front of the list or NULL if empty
node* rem_front(node** head);
// remove and return the node at the end of the list or NULL if empty
node* rem_end(node** head);
// return the number of nodes in the list
int list_len(const node* head);
// print the data values of all the nodes in the list (from start to end)
int print_list(const node* head);
// free a single node
int free_node(node** n);
// free the entire list and set *node = NULL
int free_list(node** head);


