#include "dstruct/lists.h"
#include "ColorTypeDef.h"
int freeColor(void* c);
int initColorList(colorList** self);
int addColorList(colorList* self, color* n);		
int freeColorList(colorList** self);
int printColorList(colorList* self);
int sin_color_cycle(float time, float wavelength, int start, int end);
int genSinCurveColorList(colorList** self, unsigned int wavelength, colorPair pair, unsigned int delay_ns);
int color_fade(float time, float wavelength, unsigned int start, unsigned int end);
int genFadeColorList(colorList** self, unsigned int wavelength, colorPair pair, unsigned int delay_ns);
int saveColorList(colorList* self);

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
int initColorListQueue(colorListQueue** self);

/**
*	Adds a colorList to the queue and sets the given flags/data in the data 
*	struct.
*	
*	@param colorListQueue* self, the colorListQueue to perform the operaiton on.
*	@param colorList* data, the colorList to add to the queue.
*	@param int repeat, flag that indicates if and how many times the list will 
*	be 'executed' while it is at the front of the queue.
*	@param int priority, number that indicates the priority of the list, lower 
*	numbers indicate low priority, higher numbers indicate higher priority.
*	@param int reque, number that indicates if and how many times a list will 
*	be requeued. Negative numbers will be replaced with zero.
*	@error CERR_QueClrLstQueue_SelfNULL, self is NULL.
*	@error CERR_QueClrLstQueue_DataNull, data is NULL.
*	@return 0 on success, -1 on error.
**/
int queueColorListQueue(colorListQueue* self, colorList* data, int repeat, int priority, int reque);

/**
*	Removes the node pointed to by head and frees said node.
*
*	@param colorListQueue* self, list which is to have its head node removed.
*	@return	0 on success, -1 on error.
**/
int dequeueColorListQueue(colorListQueue* self);

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
int requeueColorListQueue(colorListQueue* self);

/**
*	Deallocates the entire queue and sets the self pointer argument to NULL.
*
*	@param colorListQueue** self, queue to be deallocated.
*	@return	0 on success, -1 on error.
**/
int freeColorListQueue(colorListQueue** self);

/**
*	Frees a colorListQueueData struct. Required to be passed to the list 
*	constructor.
**/
int freeColorListQueueData(void* arg);

//int initColorListQueueNode(node** self);
//int updateColorListQueueNode(node* self);
int isComplete(node* self);

// error number global accessors
unsigned int getCERR();
int getCERRMessage(char** str);
int getCERRName(char** str);

// utility function
int duplicateColor(color* src, color** dest);

/**
*	Initializes a colorListQueueData structure
*	
*	@param	colorListQueueData** self, structure to be initialized.
*	@param	colorList* data, colorlist to be stored in the structure.
*	@param	int repeat, number of times the colorList will repeat.
*	@param	int priority, determines position in queue.
*	@param	int requeue, number that indicates how many times the list will be 
*	requeued. Negative numbers will be replaced with zero.
*	@error
**/
int initColorListQueueData(colorListQueueData** self, colorList* data, int repeat, int priority, int requeue);
