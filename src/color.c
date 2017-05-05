#include <stdlib.h>
#include <stdio.h>

#include "color.h"

void color_init(color* c)
{
	color_set_red(c, 0);
	color_set_green(c, 0);
	color_set_blue(c, 0);
}
void color_set_red(color* c, unsigned int value)
{
	c->red = value;
}
void color_set_green(color* c, unsigned int value)
{
	c->green = value;
}
void color_set_blue(color* c, unsigned int value)
{
	c->blue = value;
}
void color_set_brightness(color* c, unsigned int value)
{
	c->brightness = value;
}
void color_set_delay(color* c, unsigned int value)
{
	color->delay = value;
}
void colorlist_init(node** head)
{
	head* = (node*)malloc(sizeof(node));
}
void colorlist_add(node** head, node** tail, color n)
{
	//TODO
}
void colorlist_free(node** head)
{
	//TODO
}