#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "color.h"

#define MAX_COLOR 255
#define PI 3.14
#define MAX_SER_NODES 1024
int freeColor(void* c){
	int ret = -1;
	printf("inFreeColor");
	fflush(stdout);
	if(c != NULL){
		free((color*)c);
	}
	return ret;	
}

int initColorList(colorList** self){
	int ret = -1;
	if(self != NULL){
		if(*self == NULL){
			(*self) = (colorList*)malloc(sizeof(colorList));
			(*self)->head = NULL;
			(*self)->tail = NULL;
			(*self)->nodeCount = 0;
			ret = 0;
		}
	}
	
	return ret;
}

int addColorList(colorList* self, color* n){
	int ret = -1;
	if(self != NULL){
		node* new = NULL;
		create_node(&new, (void*)n, NULL, NULL);
		self->tail = new;
		ret = add_end(&(self->head), new);
	}
	return ret;
}
int freeColorList(colorList** self){
	int ret = -1;
	if(self!=NULL){
		if(*self!=NULL){
			free_list(&((*self)->head));
			(*self)->tail = NULL;
			free(*self);
			(*self) = NULL;
			ret = 0;
		}
	}
	return ret;
}

int printColorList(colorList* self){
	int ret = -1;
	if(self!=NULL){
		node* current = self->head;
		while(current != NULL){
			color* data = (color*)current->data;
			printf("{red: %d, green: %d, blue: %d, bright: %d, delay: %d}\n", data->red, data->green, data->blue, data->brightness, data->delay);
			printf("\t\t|\n");
			printf("\t\tV\n");
			current = current->next;
			
		}
		ret = 0;
	}
	return ret;
}
int sin_cycle(float time, float wavelength){
	//time - current time
	//wavelength - length in time units of 1 wave
	float rad = ((2*PI)*time)/wavelength;
	return 128*(1+sin(rad));
}

int sin_color_cycle(float time, float wavelength, int start, int end){
	//time - current time
	//wavelength - length in time units of 1 wave
	//offset - relative to sine function max range[0, 1]
	float midpoint = (start + end)/2;
	float amplitude = (end - start)/2;
	
	if(amplitude == 0 || wavelength == 0){
		return -1;
	}
	
	float offset = (midpoint/amplitude);
	float rad = (((2*PI)*time)/wavelength)+((3*PI)/2);
	//printf("%f\n", rad);
	return (int)amplitude*((offset)+sin(rad));
}

int genSinCurveColorList(colorList* self, float wavelength, colorPair pair, int delay_ns){
	int ret = -1;
	//TODO
	if(self!=NULL){
		//make sure they were recently initialized
		if(self->head==NULL && self->tail==NULL){
			if(delay_ns != 0){
				float time_step = delay_ns / 1000000000.0;
				float time = 0.0;
				while(time < wavelength){
					color* n = (color*)malloc(sizeof(color));
					n->red = (unsigned int)sin_color_cycle(time, wavelength, pair.start.red, pair.end.red);
					n->green = (unsigned int)sin_color_cycle(time, wavelength, pair.start.green, pair.end.green);
					n->blue = (unsigned int)sin_color_cycle(time, wavelength, pair.start.blue, pair.end.blue);
					n->brightness = (unsigned int)sin_color_cycle(time, wavelength, pair.start.brightness, pair.end.brightness);
					n->delay = (unsigned int)delay_ns;
					addColorList(self, n);
					time+=time_step;
				}
				ret = 0;
			}
		}
	}
	return ret;
}
