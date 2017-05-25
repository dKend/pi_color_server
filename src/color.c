#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "color.h"

#define MAX_COLOR 255
#define PI 3.14
#define MAX_SER_NODES 1024

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
		color color1 = {100, 200, 10, 0, 100};
		color color2 = {100, 200, 10, 0, 100};
		color color3 = {100, 200, 10, 0, 100};
		struct node n1 = {&color1, NULL};
		struct node n2 = {&color2, NULL};
		struct node n3 = {&color3, NULL};
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
	void test_generate_sin_cycle_list()
	{
		struct node* head;
		struct node* tail;
		colorlist_init(&head, &tail);
		float wavelength = 10.0;
		int start = 0;
		int end = 255;
		int delay_ns = 50000000;
		generate_sin_cycle_list(&head, &tail, wavelength, start, end, delay_ns);
		
		assert_(head!=NULL, "head is still NULL.");
		assert_(tail!=NULL, "tail is still NULL.");
		assert_(head!=tail, "head is tail.");
		
		colorlist_print(head, tail);
		
		if(head!=NULL && tail!=NULL)
			colorlist_free(&tail);
		
	}
	test_color_init();
	test_set_red();
	test_set_green();
	test_set_blue();
	test_set_brightness();
	test_set_delay();
	test_colorlist_init();
	test_colorlist_add();
	test_generate_sin_cycle_list();
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
			free((*c)->data);
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

int sin_cycle(float time, float wavelength)
{
	//time - current time
	//wavelength - length in time units of 1 wave
	float rad = ((2*PI)*time)/wavelength;
	return 128*(1+sin(rad));
}

int sin_color_cycle(float time, float wavelength, int start, int end)
{
	//time - current time
	//wavelength - length in time units of 1 wave
	//offset - relative to sine function max range[0, 1]
	float midpoint = (start + end)/2;
	float amplitude = (end - start)/2;
	
	if(amplitude == 0)
	{
		return -1;
	}
	
	float offset = (midpoint/amplitude) - 1.0;
	float rad = ((2*PI)*time)/wavelength;
	
	return (int)amplitude*((1+offset)+sin(rad));
}

void generate_sin_cycle_list(struct node** head, struct node** tail, float wavelength, int start, int end, int delay_ns)
{
	
	//TODO
	if(head!=NULL && tail!=NULL)
	{
		//make sure they were recently initialized
		if(*head==NULL && *tail==NULL)
		{
			float time_step = delay_ns / 1000000000.0;
			float time = 0.0;
			while (time < wavelength)
			{
				struct node* n = (node*)malloc(sizeof(node));
				n->data = (color*)malloc(sizeof(color));
				n->next = NULL;
				
				color_init(n->data);
				color_set_green(n->data, sin_color_cycle(time, wavelength, start, end));
				
				colorlist_add(head, tail, n);
				
				time+=time_step;
				
			}
		}
	}
}

void colorlist_print(struct node* head, struct node* tail)
{
	if(head!=NULL && tail!=NULL)
	{
		tail->next = NULL;
		struct node* current = head;
		
		while(current != NULL)
		{
			printf("{red: %d, green: %d, blue: %d, bright: %d, delay: %d}\n", (current->data)->red, (current->data)->green, (current->data)->blue, (current->data)->brightness, (current->data)->delay);
			printf("\t\t|\n");
			printf("\t\tV\n");
			current = current->next;
		}
	}	
	tail->next = head;
}

void color2_init(color2* c)
{
	color2_set(c, 0);
	color2_set_delay(c, 1000);
}
void color2_set(color2* c, unsigned int v)
{
	if(v <= MAX_COLOR)
		c->value = v;
}
void color2_set_delay(color2* c, unsigned int v)
{
	c->delay_ns = v;
}
void color2_set_pin(color2* c, unsigned int v)
{
	c->pin = v;
}