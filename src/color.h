#include "dstruct/lists.h"
typedef struct  color{
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	unsigned int brightness;
	unsigned int delay;
}color;
typedef struct colorPair{
	color start;
	color end;
}colorPair;
typedef struct colorList{
	node* head;
	node* tail;
	unsigned int nodeCount;
}colorList;
typedef struct colorListQueueData{
	colorList* list;
	node* current;
	int timesRepeated;
	int msPassedSinceNodeChange;
	int repeat_f;
	int prior_f;
	int reque_f;
}colorListQueueData;
typedef struct colorListQueue{
	node* head;
	node* tail;
	unsigned int nodeCount;
}colorListQueue;

int freeColor(void* c);

int initColorList(colorList** self);
int addColorList(colorList* self, color* n);		
int freeColorList(colorList** self);
int printColorList(colorList* self);
//int sin_cycle(float time, float wavelength);
int genSinCurveColorList(colorList* self, int wavelength, colorPair pair, int delay_ns);

void genFadeColorList(colorList* self, float wavelength, colorPair pair);
void saveColorList(colorList* self);

int initColorListQueue(colorListQueue** self);
int queueColorListQueue(colorListQueue* self, colorList* data, int repeat, int priority, int reque);
int requeueColorListQueue(colorListQueue* self, node* node);
int dequeueColorListQueue(colorListQueue* self, colorList** val);
int freeColorListQueue(colorListQueue** self);

int initColorListQueueNode(node** self);
int updateColorListQueueNode(node* self);
int isComplete(node* self);

// error number globals
unsigned int getCERR();
int getCERRMessage(char** str);

