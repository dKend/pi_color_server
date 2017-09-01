typedef struct node{
	color* data;
	struct node* next;
} node;

void colorlist_init(struct node** head, struct node** tail);
void colorlist_add(struct node** head, struct node** tail, struct node* n);
void colorlist_free(struct node** tail);
void colorlist_print(struct node* head, struct node* tail);

int sin_cycle(float time, float wavelength);
int sin_color_cycle(float time, float wavelength, int start, int end);
int cos_color_cycle(float time, float wavelength, int start, int end);
void generate_sin_cycle_list(struct node** head, struct node** tail, float wavelength, int start, int end, int delay_ns);
void generate_fade_to_color(struct node** tail, float wavelength, int end);
void save_cycle_list(struct node** head, struct node** tail);