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