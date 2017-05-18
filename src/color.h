#include <stdbool.h>
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
	color* data;
	struct node* next;
} node;

void colorlist_init(struct node** head, struct node** tail);
void colorlist_add(struct node** head, struct node** tail, struct node* n);
void colorlist_free(struct node** tail);

int assert_(bool statement, const char* error);
int assert_false(bool statement, const char* error);
int test_();

int sin_cycle(float time, float wavelength);
int sin_color_cycle(float time, float wavelength, int start, int end);
void generate_sin_cycle_list(struct node** head, struct node** tail, float wavelength, int start, int end);
void save_cycle_list(struct node** head, struct node** tail);