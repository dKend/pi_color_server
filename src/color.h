typedef struct {
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	unsigned int brightness;
	unsigned int delay;
} color;

void color_init(color* c);
void color_set_red(color* c, unsigned int value);
void color_set_green(color* c, unsigned int value);
void color_set_blue(color* c, unsigned int value);
void color_set_brightness(color* c, unsigned int value);
void color_set_delay(color* c, unsigned int value);

typedef struct node{
	color data;
	node* next;
} node;

void colorlist_init(node** head);
void colorlist_add(node** head, node** tail, color n);
void colorlist_free(node** head);