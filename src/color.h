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

int initColorListQueue(colorListQueue** self);
int queueColorListQueue(colorListQueue* self, colorList* data, int repeat, int priority, int reque);
int requeueColorListQueue(colorListQueue* self, node* node);
int dequeueColorListQueue(colorListQueue* self, colorList** val);
int freeColorListQueue(colorListQueue** self);

int initColorListQueueNode(node** self);
int updateColorListQueueNode(node* self);
int isComplete(node* self);

// error number global accessors
unsigned int getCERR();
int getCERRMessage(char** str);
int getCERRName(char** str);

// utility function
int duplicateColor(color* src, color** dest);
