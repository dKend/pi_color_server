typedef struct threadArgs{
	int pi;
	int sock;
} threadArgs;

void initThreadArgs(threadArgs** self, int pi, int sock);
void freeThreadArgs(threadArgs** self);
int getPi(struct threadArgs* self);
int getSock(struct threadArgs* self);
void closeSock(struct threadArgs** self);