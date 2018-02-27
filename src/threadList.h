#include <sys/types.h>
typedef struct threadListNode {
	pthread_t data;
	unsigned int f_joined;
	unsigned int f_removed;
	struct threadListNode* next;
}threadListNode;
typedef struct threadList {
	struct threadListNode* head;
	struct threadListNode* tail;
	unsigned int nodeCount;
}threadList;
int initThreadList(struct threadList** self);
int freeThreadList(struct threadList** self);
int addThreadList(struct threadList* self, pthread_t data);
int removeThreadList(struct threadList* self, unsigned int index);
int getThreadList(struct threadList* self, unsigned int index, pthread_t* var);
int removeHeadThreadList(struct threadList* self);
int removeTailThreadList(struct threadList* self);
int isEmptyThreadList(struct threadList* self);
int waitForThreadsThreadList(struct threadList* self);
