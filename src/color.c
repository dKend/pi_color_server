#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "color.h"

#define MAX_COLOR 255

int test_(){
	void test_color_init()
	{
		color c;
		color_init(&c);
		
		assert_(c.red == 0, "red isnt zero.");
		assert_(c.green == 0, "green isnt zero.");
		assert_(c.blue == 0, "blue isnt zero.");
		assert_(c.brightness == 255, "brightness isnt 255.");
		assert_(c.delay == 1000, "delay isnt 1000ns.");
	}
	void test_set_red()
	{
		color c = {100, 200, 255, 255, 1000};
		color_set_red(&c, 1);
		assert_(c.red == 1, "red isnt changed to 1.");
		color_set_red(&c, 2000);
		assert_(c.red == 1, "red is changed when invalid integer is given (+).");
		color_set_red(&c, -1);
		assert_(c.red == 1, "red is changed when invalid integer is given (-).");
	}
	void test_set_green()
	{
		color c = {100, 200, 255, 255, 1000};
		color_set_green(&c, 1);
		assert_(c.green == 1, "green isnt changed to 1.");
		color_set_green(&c, 2000);
		assert_(c.green == 1, "green is changed when invalid integer is given (+).");
		color_set_green(&c, -1);
		assert_(c.green == 1, "green is changed when invalid integer is given (-).");
	}
	void test_set_blue()
	{
		color c = {100, 200, 255, 255, 1000};
		color_set_blue(&c, 1);
		assert_(c.blue == 1, "blue isnt changed to 1.");
		color_set_blue(&c, 2000);
		assert_(c.blue == 1, "blue is changed when invalid integer is given (+).");
		color_set_blue(&c, -1);
		assert_(c.blue == 1, "blue is changed when invalid integer is given (-).");
	}
	void test_set_brightness()
	{
		color c = {100, 200, 255, 255, 1000};
		color_set_brightness(&c, 1);
		assert_(c.brightness == 1, "brightness isnt changed to 1.");
		color_set_brightness(&c, 2000);
		assert_(c.brightness == 1, "brightness is changed when invalid integer is given (+).");
		color_set_brightness(&c, -1);
		assert_(c.brightness == 1, "brightness is changed when invalid integer is given (-).");
	}
	void test_set_delay()
	{
		color c = {100, 200, 255, 255, 1000};
		color_set_delay(&c, 1);
		assert_(c.delay == 1, "delay isnt changed to 1.");
	}
	void test_colorlist_init()
	{
		struct node* head;
		struct node* tail;
		colorlist_init(&head, &tail);
		assert_(head == NULL, "head isnt NULL.");
		assert_(tail == NULL, "tail isnt NULL.");
	}
	void test_colorlist_add()
	{
		struct node* head;
		struct node* tail;
		struct node n1 = {{100, 200, 10, 0, 100}, NULL};
		struct node n2 = {{100, 200, 10, 0, 100}, NULL};
		struct node n3 = {{100, 200, 10, 0, 100}, NULL};
		colorlist_init(&head, &tail);
		
		colorlist_add(&head, &tail, &n1);
		assert_(head !=NULL, ".");
		assert_(tail !=NULL, ".");
		assert_(head == &n1, "head not set.");
		assert_(head == tail, "color not set.");
		
		colorlist_add(&head, &tail, &n2);
		assert_(head != NULL, ".");
		assert_(tail != NULL, ".");
		assert_(head == &n1, "head changed.");
		assert_(tail == &n2, "tail didnt change");
		assert_(tail->next == &n1, "tail->next isnt head.");
		
		colorlist_add(&head, &tail, &n3);
		assert_(head != NULL, ".");
		assert_(tail != NULL, ".");
		assert_(head == &n1, "head changed.");
		assert_(tail == &n3, "tail didnt change");
		assert_(tail->next == &n1, "tail->next isnt head.");
		
		
	}
	test_color_init();
	test_set_red();
	test_set_green();
	test_set_blue();
	test_set_brightness();
	test_set_delay();
	test_colorlist_init();
	test_colorlist_add();
	return 0;
}
void color_init(color* c)
{
	color_set_red(c, 0);
	color_set_green(c, 0);
	color_set_blue(c, 0);
	color_set_brightness(c, 255);
	color_set_delay(c, 1000);
}
void color_set_red(color* c, unsigned int value)
{
	if(value <= MAX_COLOR)
		c->red = value;
		
}
void color_set_green(color* c, unsigned int value)
{
	if(value <= MAX_COLOR)
		c->green = value;
}
void color_set_blue(color* c, unsigned int value)
{
	if(value <= MAX_COLOR)
		c->blue = value;
}
void color_set_brightness(color* c, unsigned int value)
{
	if(value <= MAX_COLOR)
		c->brightness = value;
}
void color_set_delay(color* c, unsigned int value)
{
	c->delay = value;
}
void colorlist_init(struct node** head, struct node** tail)
{
	*head = NULL;
	*tail = NULL;
}
void colorlist_add(struct node** head, struct node** tail, struct node* n)
{
	if((*tail)!=NULL)
	{
		(*tail)->next = n;
		(*tail) = n;
		
		//make it cyclic
		(*tail)->next = (*head);
		
	}else
	{
		(*head) = n;
		(*tail) = n;
		
	}
}
void colorlist_free(struct node** tail)
{
	void freehelp(struct node** c)
	{
		if(*c!=NULL)
		{
			colorlist_free( &((*c)->next) );
			free(*c);
			*c = NULL;
		}
	}
	
	if(*tail!=NULL)
	{
		//converts cyclic list into a normal linked list so we can use the normal linked list free algorithm
		struct node** thing = &((*tail)->next);
		(*tail)->next = NULL;
		freehelp(thing);
	}
}

int assert_(bool statement, const char* error)
{
	if(statement)
	{
		printf("AssertPass\n");
		
		return 1;
	}
	printf("AssertFail: %s\n", error);
	
	return 0;
}

int assert_false(bool statement, const char* error)
{
	return assert_(!statement, error);
}