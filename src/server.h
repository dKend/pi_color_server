typedef struct s_flags {	//start flags
	int daemon;
	int no_log;
	int version;
	int multi_thread;
	int help;
} s_flags;

typedef struct Server {
	s_flags* sFlags;
	color* sColor;
	int sColorChanged; //not sure if we still need this
	sem_t sCCLock;	//or this
	int sListen;
	sem_t sLLock;
	FILE* sLog;
	int sPi;
	int sSock;
	colorListQueue* sRunning;	//Points to one of the below queues
	colorListQueue* sLow;		//queue for normal priority Lists
	colorListQueue* sHigh;		//queue for high priority Lists
} Server;

void* cycle(void* args);
int createServer(Server** self);
int freeServer(Server** self);
int initServer(Server* self);	// initialize server fields to defaults
int runServer(Server* self);	// only returns on halt, should be called by main after server data is initialized
int getColorChanged(Server* self);
void setColorChanged(Server* self);
void unsetColorChanged(Server* self);
void setColor(Server* self, unsigned int r, unsigned int g, unsigned int b, unsigned int br);
void colorChangedActions(Server* self);
int listenLoop(Server* self);
int processRequest(Server* self);
void server_tests();
void apply_brightness(int* n, int bright);
int setListen(Server* self);
int unsetListen(Server* self);
int log_output(Server* self, const char * str);
void* threadHandleConnection(void* args);
