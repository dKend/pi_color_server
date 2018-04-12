#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "Color.h"
#include "ColorErrorDef.h"

#define MAX_COLOR 255
#define PI 3.14
#define MAX_SER_NODES 1024

static unsigned int cerr = CERR_NOERROR;
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
	"CERR_GenFadeColorList_WavelengthZero",
	"CERR_GenFadeColorList_DelayZero",
	"CERR_GenFadeColorList_DelayGtWavelength",
	"CERR_GenFadeColorList_SelfEqNULL",
	"CERR_GenFadeColorList_SelfInit",
	"CERR_ColorFade_TimeGtWavelength",
	"CERR_ColorFade_StartEqEnd",
	"CERR_ColorFade_WavelengthEqZero",
	"CERR_SaveColorList_AccessDenied",
	"CERR_SaveColorList_OutOfSpace",
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
	"DuplicateColor received a bad pointer, *dest must be NULL.",
	"[PH]CERR_GenFadeColorList_WavelengthZero",
	"[PH]CERR_GenFadeColorList_DelayZero",
	"[PH]CERR_GenFadeColorList_DelayGtWavelength",
	"[PH]CERR_GenFadeColorList_SelfEqNULL",
	"[PH]CERR_GenFadeColorList_SelfInit",
	"[PH]CERR_ColorFade_TimeGtWavelength",
	"[PH]CERR_ColorFade_StartEqEnd",
	"[PH]CERR_ColorFade_WavelengthEqZero",
	"[PH]CERR_SaveColorList_AccessDenied",
	"[PH]CERR_SaveColorList_OutOfSpace",
};

/**
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
**/
unsigned int getCERR(){
	return cerr;
}

/**
*	::int getCERRName()::
*
*		- Allocates the passed pointer and copies the name string associated 
*		with e into it.
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
**/
int getCERRName(char** str){
	int ret = -1;
	if(str != NULL){
		if(*str != NULL){
			*str = strdup(CERR_Strings[getCERR()]);
			ret = 0;
		}else
			cerr = CERR_GetCERRName_BadPointer_NonNULL;
	}else
		cerr = CERR_GetCERRName_BadPointer;
	return ret;
}

/**
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
**/
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

/**
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
**/
int freeColor(void* c){
	int ret = -1;
	if(c != NULL){
		free((color*)c);
	}else
		cerr = CERR_FreeColor_BadPointer;
	return ret;	
}

/**
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
**/
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

/**
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
**/
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
			if(ret!=-1){
				self->nodeCount++;
			}
		}else
			cerr = CERR_AddColorList_BadColor_NULLPointer;
	}else
		cerr = CERR_AddColorList_BadSelf_NULLPointer;
	return ret;
}

/**
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
**/
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

/**
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
**/
int printColorList(colorList* self){
	int ret = -1;
	if(self!=NULL){
		node* current = self->head;
		printf("\n");
		while(current != NULL){
			color* data = (color*)current->data;
			printf("{red: %d, green: %d, blue: %d, bright: %d, delay: %d}\n", data->red, data->green, data->blue, data->brightness, data->delay);
			current = current->next;
			
		}
		printf("\n");
		fflush(stdout);
		ret = 0;
	}else
		cerr = CERR_PrintColorList_BadColorList;
	return ret;
}

/**
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
**/
int sin_color_cycle(float time, float wavelength, int start, int end){
	//time - current time
	//wavelength - length in time units of 1 wave
	//offset - relative to sine function max range[0, 1]
	int ret = -1;
	if(wavelength != 0){
		if(start != end){
			float midpoint = (start + end)/2;
			if(end > start){
				if(time == 0){
					ret = (int)start;
				}else if(time == wavelength){
					ret = (int)start;
				}else if(time == (wavelength/2.0)){
					ret = (int)end;
				}else{
					// Old implementation using sin curve
					float amplitude = (end - start)/2;
					float offset = (midpoint/amplitude);
					float rad = (((2*PI)*time)/wavelength)+((3*PI)/2);
					//printf("%f\n", rad);
					ret = (int)amplitude*((offset)+sin(rad));
				}
			}else{
				if(time == 0){
					ret = (int)start;
				}else if(time == wavelength){
					ret = (int)start;
				}else if(time == (wavelength/2.0)){
					ret = (int)end;
				}else{
					// Old implementation using sin curve
					float amplitude = (start - end)/2;
					float offset = (midpoint/amplitude);
					float rad = (((2*PI)*time)/wavelength)+(PI/2);
					//printf("%f\n", rad);
					ret = (int)amplitude*((offset)+sin(rad));
				}
				
			}
			
		}else
			cerr = CERR_SinColorCycle_BadEndpoints_StartEqualsEnd;
	}else
		cerr = CERR_SinColorCycle_BadWavelength_Zero;

	
	return ret;
}

/**	
* 	::int genSinCurveColorList(colorList* self, unsigned int wavelength, colorPair pair, unsigned int delay_ns)::
*
*		- generates a colorList which is a fade from one color to another (specified in the colorPair variable).
*		- the generated list is placed in the head node of the self argument (tail is also set).
*	 	- wavelength is in SECONDS, delay_ns is in NANOSECONDS
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
**/
int genSinCurveColorList(colorList** self, unsigned int wavelength, colorPair pair, unsigned int delay_ns){
	int ret = -1;
	if(self!=NULL){
		if(initColorList(self) != -1){
			if(wavelength != 0){
				if(delay_ns != 0){
					float time_step = ((float)delay_ns) / 1000000000.0;
					float lambda = (float)wavelength;
					if(time_step < lambda){
						ret = 0;
						float time = 0.0;
						float nodes = 0;
						while(time <= lambda){
							color* n = (color*)malloc(sizeof(color));
							n->red = (unsigned int)sin_color_cycle(time, lambda, pair.start.red, pair.end.red);
							n->green = (unsigned int)sin_color_cycle(time, lambda, pair.start.green, pair.end.green);
							n->blue = (unsigned int)sin_color_cycle(time, lambda, pair.start.blue, pair.end.blue);
							n->brightness = (unsigned int)sin_color_cycle(time, lambda, pair.start.brightness, pair.end.brightness);
							n->delay = delay_ns;
							
							//check if any sin_color_cycle errors occurred
							if(n->red != -1 && n->green != -1 && n->blue != -1 && n->brightness != -1 && n->delay != -1){
								addColorList(*self, n);
								time+=time_step;
								nodes++;
							}else{
								ret = -1;
								freeColorList(self);
								*self=NULL;
								break;
							}
						}
						color* tmp = NULL;
						duplicateColor(&(pair.start), &tmp);
						tmp->delay = delay_ns;
						addColorList(*self, tmp);
					}else{
						cerr = CERR_GenSinCurveColorList_BadDelay_NotLessThanWavelength;
						freeColorList(self);
						*self=NULL;
					}
				}else{
					cerr = CERR_GenSinCurveColorList_BadDelay_Zero;
					freeColorList(self);
					*self=NULL;
				}
			}else{
				cerr = CERR_GenSinCurveColorList_BadWavelength_Zero;
				freeColorList(self);
				*self=NULL;
			}
				
		}else
			cerr = CERR_GenSinCurveColorList_BadColorList_AlreadyInit;
	}else
		cerr = CERR_GenSinCurveColorList_BadColorList_NULL;
	
	return ret;
}

/**
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
**/
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

/**
*	Calculates the value of a fade over time from the color specified by start 
*	to the color specified by end. The value is proportional to the value of 
*	time/wavelength.
*	There are two cases that can occur when valid input is received:
*	Case 1:	Start < end
*		Steps:
*		(1)	difference = end - start
*		(2) add = difference * (time / wavelength)
*		(3) return start + add
*	Case 2: Start > end
*		Steps:
*		(1) difference = start - end
*		(2) add = difference * (time / wavelength)
*		(3) return start - add
*	We can actually factor this down to only case 1 since in the case of start >
*	end, the value of difference will be a negative number and as a result add 
*	will also be negative. So when we return the value of "start + add", in the 
*	second case this is the same as "start - add" where difference is calculated 
*	using "start - end".
*	
*	@param time float, number between 0 and wavelength
*	@param wavelength
*	@param start
*	@param end
*	@return ret
*	@error CERR_ColorFade_TimeGtWavelength
*	@error CERR_ColorFade_StartEqEnd
*	@error CERR_ColorFade_WavelengthEqZero
**/
int color_fade(float time, float wavelength, unsigned int start, unsigned int end){
	int ret = -1;
	time = fabsf(time);
	if(time<=wavelength){
		if(wavelength!=0){
			if(start!=end){
				if(start < end){
					ret = start+((end-start)*(time/wavelength));
				}else{
					ret = start-((start-end)*(time/wavelength));
				}
			}else
				cerr = CERR_ColorFade_StartEqEnd;
		}else
			cerr = CERR_ColorFade_WavelengthEqZero;
	}else
		cerr = CERR_ColorFade_TimeGtWavelength;
	return ret;
}

/**
*	Creates a colorList using the fade_color function. Creates nodes in a loop 
*	and calls the color_fade function for each part of the colors passed in 
*	pair. The start color will occupy the first node and the end color will 
*	occupy the last node generated.
*	
*	@param self colorList**
*	@param wavelength unsigned int; In SECONDS 
*	@param colorPair pair
*	@param delay_ns unsigned int; In NANOSECONDS
*	@error CERR_GenFadeColorList_SelfEqNULL				1
*	@error CERR_GenFadeColorList_SelfInit				2
*	@error CERR_GenFadeColorList_WavelengthZero			3
*	@error CERR_GenFadeColorList_DelayZero				4
*	@error CERR_GenFadeColorList_DelayGtWavelength		5
**/
int genFadeColorList(colorList** self, unsigned int wavelength, colorPair pair, unsigned int delay_ns){
	int ret = -1;
	if(self != NULL){
		if((*self) == NULL){
			if(wavelength > 0){
				if(delay_ns > 0){
					float time_step = ((float)delay_ns) / 1000000000.0;
					float lambda = (float)wavelength;
					
					if(time_step < lambda){
						initColorList(self); // this should NEVER error 
						float time = 0.0;
						while(time <= lambda){
							printf("time: %f\tlambda: %f\ttime_step: %f\n", time, lambda, time_step);
							color* n = (color*)malloc(sizeof(color));
							n->red = (unsigned int)color_fade(time, lambda, pair.start.red, pair.end.red);
							n->green = (unsigned int)color_fade(time, lambda, pair.start.green, pair.end.green);
							n->blue = (unsigned int)color_fade(time, lambda, pair.start.blue, pair.end.blue);
							n->brightness = (unsigned int)color_fade(time, lambda, pair.start.brightness, pair.end.brightness);
							n->delay = delay_ns;
							addColorList(*self, n);
							time+=time_step;
						}
						ret = 0;
					}else
						cerr = CERR_GenFadeColorList_DelayGtWavelength;
				}else
					cerr = CERR_GenFadeColorList_DelayZero;
			}else
				cerr = CERR_GenFadeColorList_WavelengthZero;
		}else
			cerr = CERR_GenFadeColorList_SelfInit;
	}else
		cerr = CERR_GenFadeColorList_SelfEqNULL;
	return ret;
}

/**
*	Saves the colorList stored in self to a file somewhere in storage so it can 
*	be used again at a later time.
*	
*	@param self colorList*
*	@error CERR_SaveColorList_AccessDenied
*	@error CERR_SaveColorList_OutOfSpace
**/
int saveColorList(colorList* self){
	int ret = -1;
	
	return ret;
}

/**
*	Initializes the colorListQueue by allocating space for the pointer on the 
*	heap and setting the node count to zero.
*
*	@param	self colorListQueue, Object to be initialized.
*
*	@error	CERR_InitColorListQueue_NULL,	self is NULL
*	@error	CERR_InitColorListQueue_NotNULL	self is already initialized.
*	
*	@return	0 on success, -1 on error.
**/
int initColorListQueue(colorListQueue** self){
	int ret = -1;
	
	return ret;
}

/**
*	Adds a colorList to the queue and sets the given flags/data in the data 
*	struct.
*	
*	@param 	colorListQueue* self, the colorListQueue to perform the operaiton 
*	on.
*	@param	colorList* data, the colorList to add to the queue.
*	@param	int repeat, flag that indicates if and how many times the list will 
*	be 'executed' while it is at the front of the queue.
*	@param	int priority, number that indicates the priority of the list, lower 
*	numbers indicate low priority, higher numbers indicate higher priority.
*	@param	int reque, number that indicates if and how many times a list will 
*	be requeued. Negative numbers will be replaced with zero.
*	@error	CERR_QueClrLstQueue_SelfNULL,	self is NULL.
*	@error	CERR_QueClrLstQueue_DataNull,	data is NULL.
*	@return	0 on success, -1 on error.
**/
int queueColorListQueue(colorListQueue* self, colorList* data, int repeat, int priority, int reque){
	int ret = -1;
	
	return ret;
}

/**
*	Pops a node from the front of the list and moves it to the back of the list 
*	if certain conditions are met.
*	Operations:
*	<p>
*		1)
*			(Condition)
*				If the requeue value of the head node is greater then 0 OR equal
*				to -1.
*			(Operation)
*				The head node is 'popped' from the top of the queue and  
*				queued again to the back of the queue regardless of the data's 
*				priority value.
*
*		2)
*			(Condition)
*				If and only if the value of the head node is greater than 0.
*			(Operation)
*				The node's requeue value is decremented by one.
*	</p>
*	<p>
*		(NOTE) 
*			If the list only contains one node and the condition for operation 2 
*			is met, then the node's requeue field is decremented and no further  
*			actions occur.
*	</p>
*	<p>
*		The value of a node's requeue field indicates how many times that node  
*		is allowed to reach the head node position in the queue. If requeue is  
*		called on a node which has a requeue value of 0, then the node is 
*		removed from the list and deallocated (AKA 'dequeue' is called).
*	</p>
*
*	@param	colorListQueue* self, list on which the operation is performed.
*	@error	CERR_RequeClrLstQueue_NULL, self is NULL.
*	@return	0 on success, -1 on error.
**/
int requeueColorListQueue(colorListQueue* self){
	int ret = -1;
	
	return ret;
}

/**
*	Removes the node at the front of the queue from the list and frees said 
*	node.
*
*	@param colorListQueue* self, list which is to have its head node removed.
*	@return	0 on success, -1 on error.
**/
int dequeueColorListQueue(colorListQueue* self){
	int ret = -1;
	
	return ret;
}

/**
*	Deallocates the entire queue and sets the self pointer argument to NULL.
*
*	@param colorListQueue** self, queue to be deallocated.
*	@return	0 on success, -1 on error.
**/
int freeColorListQueue(colorListQueue** self){
	int ret = -1;
	
	return ret;
}

int freeColorListQueueData(void* arg){
	int ret = -1;
	colorListQueueData* tmp = (colorListQueueData*) arg;
	if(arg != NULL){
		freeColorList(&(tmp->list));
		free(tmp);
		ret = 0;
	}
	return ret;
}

int isComplete(node* self){
	int ret = -1;
	
	return ret;
}
