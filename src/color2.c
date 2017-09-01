typedef struct {
	unsigned int pin;
	unsigned int value;
	unsigned int delay_ns;
} color2;

void color2_init(color2* c);
void color2_set(color2* c, unsigned int value);
void color2_set_pin(color2* c, unsigned int value);
void color2_set_delay(color2* c, unsigned int value);

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
