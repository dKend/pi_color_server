#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "color.h"

#define MAX_COLOR 255
#define PI 3.14
#define MAX_SER_NODES 1024

#define CERR_NOERROR 												0
#define CERR_GenSinCurveColorList_BadWavelength_Zero 				1
#define CERR_GenSinCurveColorList_BadDelay_Zero 					2
#define CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength	3
#define CERR_GenSinCurveColorList_BadColorList_NULL 				4
#define CERR_GenSinCurveColorList_BadColorList_AlreadyInit			5
#define CERR_SinColorCycle_BadEndpoints_StartEqualsEnd 				6
#define CERR_SinColorCycle_BadWavelength_Zero 						7
#define CERR_PrintColorList_BadColorList 							8
#define CERR_FreeColorList_BadSelf_NullPointer						9
#define CERR_FreeColorList_BadSelf_NullPointerPointer				10
#define CERR_AddColorList_BadSelf_NULLPointer						11
#define CERR_AddColorList_BadColor_NULLPointer						12
#define CERR_InitColorList_BadSelf_NULLPointer						13
#define CERR_InitColorList_BadSelf_NULLPointerPointer				14
#define CERR_FreeColor_BadPointer									15
#define CERR_GetCERRMessage_BadPointer								16
#define CERR_GetCERRMessage_BadPointer_NonNULL						17
#define CERR_GetCERRName_BadE										18
#define CERR_GetCERRName_BadPointer									19
#define CERR_GetCERRName_BadPointer_NonNULL							20
#define CERR_DuplicateColor_BadSrcPointer_NULL						21
#define CERR_DuplicateColor_BadDestPtr_NULL							22
#define CERR_DuplicateColor_BadDestPtr_NonNULL						23

static unsigned int cerr = CERR_NOERROR;
static unsigned int CERR_MAX = 20;
static const char* const CERR_Strings[] = {
	"CERR_NOERROR",
	"CERR_GenSinCurveColorList_BadWavelength_Zero",
	"CERR_GenSinCurveColorList_BadDelay_Zero",
	"CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength",
	"CERR_GenSinCurveColorList_BadColorList_NULL",
	"CERR_GenSinCurveColorList_BadColorList_AlreadyInit",
	"CERR_SinColorCycle_BadEndpoints_StartEqualsEnd",
	"CERR_SinColorCycle_BadWavelength_Zero",
	"CERR_PrintColorList_BadColorList",
	"CERR_FreeColorList_BadSelf_NullPointer",
	"CERR_FreeColorList_BadSelf_NullPointerPointer",
	"CERR_AddColorList_BadSelf_NULLPointer",
	"CERR_AddColorList_BadColor_NULLPointer",
	"CERR_InitColorList_BadSelf_NULLPointer",
	"CERR_InitColorList_BadSelf_NULLPointerPointer",
	"CERR_FreeColor_BadPointer",
	"CERR_GetCERRMessage_BadPointer",
	"CERR_GetCERRMessage_BadPointer_NonNULL",
	"CERR_GetCERRName_BadE",
	"CERR_GetCERRName_BadPointer",
	"CERR_GetCERRName_BadPointer_NonNULL",
	"CERR_DuplicateColor_BadSrcPointer_NULL",
	"CERR_DuplicateColor_BadDestPtr_NULL",
	"CERR_DuplicateColor_BadDestPtr_NonNULL",
	
	
};
static const char *CERR_Messages[] = {
	"No errors have been caught.",
	"GenSirCurveColorList received a bad wavelength value, wavelength must be greater than zero.",
	"GenSirCurveColorList received a bad delay value, delay must be greater than zero.",
	"GenSinCurveColorList received a bad delay value, delay must be less than wavelength.",
	"GenSinCurveColorList received a bad pointer, self cannot be NULL.",
	"GenSinCurveColorList received a bad pointer, self cannot be initialized.",
	"SinColorCycle received bad start and end values, start cannot equal end.",
	"SinColorCycle received a bad wavelength value, wavelength cannot be zero.",
	"PrintColorList received a bad pointer, self cannot be NULL.",
	"FreeColorList received a bad pointer, self cannot be NULL.",
	"FreeColorList received a bad pointer, self cannot point to NULL.",
	"AddColorList received a bad pointer, self cannot be NULL.",
	"AddColorList received a bad pointer, n cannot be NULL.",
	"InitColorList received a bad pointer, self cannot be NULL.",
	"InitColorList received a bad pointer, self* cannot be previously initialized.",
	"FreeColor received a bad pointer, self cannot be NULL.",
	"GetCERRMessage received a bad pointer, str cannot be NULL.",
	"GetCERRMessage received a bad pointer, *str must be NULL.",
	"GetCERRName received a bad e value, e is not a valid cerror number.",
	"GetCERRName received a bad pointer, str cannot be NULL.",
	"GetCERRName received a bad pointer, *str must be NULL.",
	"DuplicateColor received a bad pointer, src cannot be NULL.",
	"DuplicateColor received a bad pointer, dest cannot be NULL.",
	"DuplicateColor received a bad pointer, *dest must be NULL."
	
};

/*
*	::getCERR::
*
*		- returns the current integer value of cerr.
*
*	::returns::
*
*		- The value of cerr.
*
*	::error conditions::
*		- No errors possible.	
*/
unsigned int getCERR(){
	return cerr;
}

/*
*	::int getCERRName()::
*
*		- Allocates the passed pointer and copies the name string associated with e into it.
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*		
*		[CERR_GetCERRName_BadPointer]
*			- Str is NULL.
*
*		[CERR_GetCERRName_BadPointer_NonNULL]
*			- *Str was not NULL.
*
*		[CERR_GetCERRName_BadE]
*			- E is not a valid cerror number.
*
*/
int getCERRName(unsigned int e, char** str){
	int ret = -1;
	if(e <= CERR_MAX){
		if(str != NULL){
			if(*str != NULL){
				*str = strdup(CERR_Strings[e]);
				ret = 0;
			}else
				cerr = CERR_GetCERRName_BadPointer_NonNULL;
		}else
			cerr = CERR_GetCERRName_BadPointer;
	}else
		cerr = CERR_GetCERRName_BadE;
	
	return ret;
}

/*
*	::int getCERRMesage(char** str)::
*
*		- Allocates and places a copy of the error name (cooresponding to the value stores in cerr) into the passed pointer.
*		- Pointers passed to this function need to be freed.
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_GetCERRMessage_BadPointer]
*			- Str is NULL.
*
*		[CERR_GetCERRMessage_BadPointer_NonNULL]
*			- *Str was not NULL.
*
*/
int getCERRMessage(char** str){
	int ret = -1;
	if(str != NULL){
		if(*str == NULL){
			*str = strdup(CERR_Messages[cerr]);
			if(*str != NULL)
				ret = 0;
			else
				perror("strdup failed in getCERRMessage");
		}else
			cerr = CERR_GetCERRMessage_BadPointer_NonNULL;
	}else
		cerr = CERR_GetCERRMessage_BadPointer;
	return ret;
}

/*
*	::int freeColor(void* c)::
*
*		- Frees a color pointer.
*		- A pointer to this function is passed to the node initialization function for nodes defined as part of a colorList 
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_FreeColor_BadPointer]
*			- Self is NULL;
*
*/
int freeColor(void* c){
	int ret = -1;
	if(c != NULL){
		free((color*)c);
	}else
		cerr = CERR_FreeColor_BadPointer;
	return ret;	
}

/*
*	::int initColorList(colorList** self)::
*
*		- Initializes the color list struct. This must be done before using any other functions.
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_InitColorList_BadSelf_NULLPointer]
*			- Self is NULL;
*
*		[CERR_InitColorList_BadSelf_NULLPointerPointer]
*			- Self* is not NULL.
*
*/
int initColorList(colorList** self){
	int ret = -1;
	if(self != NULL){
		if(*self == NULL){
			(*self) = (colorList*)malloc(sizeof(colorList));
			(*self)->head = NULL;
			(*self)->tail = NULL;
			(*self)->nodeCount = 0;
			ret = 0;
		}else
			cerr = CERR_InitColorList_BadSelf_NULLPointerPointer;
	}else
		cerr = CERR_InitColorList_BadSelf_NULLPointer;
	
	return ret;
}

/*
*	::int addColorList(colorList* self, color* n)::
*
*		- Adds the color n to a new node that is inserted at the end of the colorlist pointed to by self.
*		- Color is duplicated into a dynamically allocated color pointer. no user end free or allocation is necessary.
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_AddColorList_BadSelf_NULLPointer]
*			- Self is NULL (segmentation fault avoidance).
*
*		[CERR_AddColorList_BadColor_NULLPointer]
*			- N is NULL (segmentation fault avoidance).
*			
*
*/
int addColorList(colorList* self, color* n){
	int ret = -1;
	if(self != NULL){
		if(n != NULL){
			node* new = NULL;
			color* c = NULL;
			duplicateColor(n, &c);
			create_node(&new, (void*)c, NULL, NULL);
			self->tail = new;
			ret = add_end(&(self->head), new);
		}else
			cerr = CERR_AddColorList_BadColor_NULLPointer;
	}else
		cerr = CERR_AddColorList_BadSelf_NULLPointer;
	return ret;
}

/*
*	::int freeColorList(colorList** self)::
*
*		- frees the a color list pointed to by self, and sets the pointer to NULL
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_FreeColorList_BadSelf_NullPointer]
*			- Self is null (segmentation fault/double free avoidance).
*		[CERR_FreeColorList_BadSelf_NullPointerPointer]
*			- Self* is null (segmentation fault/double free avoidance).
*
*/
int freeColorList(colorList** self){
	int ret = -1;
	if(self!=NULL){
		if(*self!=NULL){
			free_list(&((*self)->head));
			(*self)->tail = NULL;
			free(*self);
			(*self) = NULL;
			ret = 0;
		}else
			cerr = CERR_FreeColorList_BadSelf_NullPointerPointer;
	}else
		cerr = CERR_FreeColorList_BadSelf_NullPointer;
	return ret;
}

/*
*	::int printColorList(colorList* self)::
*
*		- prints the colorlist pointed to by the self variable to stdout via printf
*
*	::returns::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_PrintColorList_BadColorList]
*			- Self is zero (segmentation fault avoidance).
*
*/
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
	}else
		cerr = CERR_PrintColorList_BadColorList;
	return ret;
}

/*
*	::int sin_color_cycle(float time, float wavelength, int start, int end)::
*	
*		- computes the value @ time (x) of the sine function where int start is the min y value and end is the max y value.
*	
*	::return::
*	
*		- value of the sine function with given parameters on success, -1 on failure.
*		
*	::error conditions::
*	
*		[CERR_SinColorCycle_BadEndpoints_StartEqualsEnd]
*			- Start is equal to end (division by zero avoidance).
*		
*		[CERR_SinColorCycle_BadWavelength_Zero]
*			- Wavelength is zero (division by zero avoidance).
*/
int sin_color_cycle(unsigned int time, unsigned int wavelength, int start, int end){
	//time - current time
	//wavelength - length in time units of 1 wave
	//offset - relative to sine function max range[0, 1]
	int ret = -1;
	if(start != end){
		if(wavelength != 0){
			float midpoint = (start + end)/2;
			float amplitude = (end - start)/2;
			float offset = (midpoint/amplitude);
			float rad = (((2*PI)*time)/wavelength)+((3*PI)/2);
			//printf("%f\n", rad);
			ret = (int)amplitude*((offset)+sin(rad));
		}else
			cerr = CERR_SinColorCycle_BadWavelength_Zero;
	}else
		cerr = CERR_SinColorCycle_BadEndpoints_StartEqualsEnd;

	
	return ret;
}

/*	
* 	::int genSinCurveColorList(colorList* self, unsigned int wavelength, colorPair pair, unsigned int delay_ns)::
*
*		- generates a colorList which is a fade from one color to another (specified in the colorPair variable).
*		- the generated list is placed in the head node of the self argument (tail is also set).
*	 
*	::return::
*
*		 - 0 on success and -1 on error, indicating that an error has occured.
*
*	::error conditions::
*
*		[CERR_GenSinCurveColorList_BadWavelength_Zero]
*			- Wavelength is zero.
*
*		[CERR_GenSinCurveColorList_BadDelay_Zero]
*			- Delay is zero.
*
*		[CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength]
*			- Wavelength was less than or equal to delay_ns/1000000000, decrease delay_ns or increase wavelength.
*
*		[CERR_GenSinCurveColorList_BadColorList_NULL]
*			- List pointer (self) was NULL.
*
*		[CERR_GenSinCurveColorList_BadColorList_AlreadyInit]
*			- List pointer's head pointer already pointed to an existing list.
*
*/
int genSinCurveColorList(colorList** self, unsigned int wavelength, colorPair pair, unsigned int delay_ns){
	int ret = -1;
	if(self!=NULL){
		if(initColorList(self) != -1){
			if(wavelength != 0){
				if(delay_ns != 0){
					float time_step = delay_ns / 1000000000.0;
					if(time_step < wavelength){
						int err = 0;
						float time = 0.0;
						while(time < wavelength){
							color* n = (color*)malloc(sizeof(color));
							n->red = (unsigned int)sin_color_cycle(time, wavelength, pair.start.red, pair.end.red);
							n->green = (unsigned int)sin_color_cycle(time, wavelength, pair.start.green, pair.end.green);
							n->blue = (unsigned int)sin_color_cycle(time, wavelength, pair.start.blue, pair.end.blue);
							n->brightness = (unsigned int)sin_color_cycle(time, wavelength, pair.start.brightness, pair.end.brightness);
							n->delay = (unsigned int)delay_ns;
							
							//check if any sin_color_cycle errors occurred
							if(n->red != -1 && n->green != -1 && n->blue != -1 && n->brightness != -1 && n->delay != -1){
								addColorList(*self, n);
								time+=time_step;
							}else{
								err = -1;
								break;
							}
						}
						
						if(err != -1){
							ret = 0;
						}
							
					}else
						cerr = CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength;
				}else
					cerr = CERR_GenSinCurveColorList_BadDelay_Zero;
			}else
				cerr = CERR_GenSinCurveColorList_BadWavelength_Zero;
		}else
			cerr = CERR_GenSinCurveColorList_BadColorList_AlreadyInit;
	}else
		cerr = CERR_GenSinCurveColorList_BadColorList_NULL;
	
	return ret;
}

/*
*	::int duplicateColor(color* self)::
*
*		- Allocate space into dest and duplicate the color pointed to by src into the newly allocated dest pointer.
*		- Needs to be freed either by freeColorList or by the user manually (NOT BOTH).
*
*	::return::
*
*		- 0 on success, -1 on failure.
*
*	::error conditions::
*
*		[CERR_DuplicateColor_BadSrcPointer_NULL]
*			- Src was NULL.
*
*		[CERR_DuplicateColor_BadDestPtr_NULL]
*			- Dest was NULL.
*
*		[CERR_DuplicateColor_BadDestPtr_NonNULL]
*			- *Dest was not NULL.
*
*/
int duplicateColor(color* src, color** dest){
	int ret = -1;
	if(src != NULL){
		if(dest != NULL){
			if(*dest == NULL){
				*dest = (color*)malloc(sizeof(color));
				(*dest)->red = src->red;
				(*dest)->green = src->green;
				(*dest)->blue = src->blue;
				(*dest)->brightness = src->brightness;
				(*dest)->delay = src->delay;
				ret = 0;
			}else
				cerr = CERR_DuplicateColor_BadDestPtr_NonNULL;
		}else
			cerr = CERR_DuplicateColor_BadDestPtr_NULL;
	}else
		cerr = CERR_DuplicateColor_BadSrcPointer_NULL;
	return ret;
}
