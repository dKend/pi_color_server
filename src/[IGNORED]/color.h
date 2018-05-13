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
void colorlist_print(struct node* head, struct node* tail);


typedef struct {
	unsigned int pin;
	unsigned int value;
	unsigned int delay_ns;
} color2;


void color2_init(color2* c);
void color2_set(color2* c, unsigned int value);
void color2_set_pin(color2* c, unsigned int value);
void color2_set_delay(color2* c, unsigned int value);

int assert_(bool statement, const char* error);
int assert_false(bool statement, const char* error);
int test_();

int sin_cycle(float time, float wavelength);
int sin_color_cycle(float time, float wavelength, int start, int end);
int cos_color_cycle(float time, float wavelength, int start, int end);
void generate_sin_cycle_list(struct node** head, struct node** tail, float wavelength, int start, int end, int delay_ns);
void generate_fade_to_color(struct node** tail, float wavelength, int end);
void save_cycle_list(struct node** head, struct node** tail);

typedef struct trans{
	int id;
	struct trans* next;
	struct node* head;
} trans;

int create_trans(struct trans** t);
int generate_list_trans(struct trans** t, struct trans* tail, color* c);
int free_trans(struct trans** t);
int add_trans(struct trans** t, color* c);				//	7
int remove_trans(struct trans** t);						//	8	
int get_trans(struct trans** t, int id);

int save_cycle(struct trans* t, const char* filename);	//	9
int load_cycle(struct trans** t, const char* filename);	//	10
int insert_trans(struct trans** t, int id, color* c);	//	11



