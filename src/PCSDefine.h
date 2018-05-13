/**
*	General defines
*/
#define	VERSION_MAJOR	0
#define	VERSION_MINOR	1
#define BUILD_NUM 		19218
#define RED_PIN 17
#define GREEN_PIN 22
#define BLUE_PIN 24
#define MAX 255
#define LOG_PATH "/var/log/pcs-server"
#define LOG_NAME "pcs-server.log"
#define DIR_PATH "/tmp/pcs"
#define BUFF_LEN 1024
#define SAV_NAME "pcs-server.color"
#define HELP_LEN 7

/**
*	PCS Error numbers.
*/
#define PCSNoError 				0
#define PCSNullPacketPointer 	1

/**
*	Command numbers
*/
#define SETCOLOR	0
#define GETCOLOR	1
#define SETRED		2
#define	GETRED		3
#define SETGREEN	4
#define GETGREEN	5
#define SETBLUE		6
#define	GETBLUE		7
#define SETBRIGHT	8
#define GETBRIGHT	9
#define KILL		10

int getPCSErrorValue();
void clearPCSErrorValue();
struct PCSPacket{
	unsigned char command;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char bright;
};
int setCommand(struct PCSPacket* self, unsigned int value);
int setRed(struct PCSPacket* self, unsigned int value);
int setGreen(struct PCSPacket* self, unsigned int value);
int setBlue(struct PCSPacket* self, unsigned int value);
int setBright(struct PCSPacket* self, unsigned int value);
int setCommandChar(struct PCSPacket* self, unsigned char value);
int setRedChar(struct PCSPacket* self, unsigned char value);
int setGreenChar(struct PCSPacket* self, unsigned char value);
int setBlueChar(struct PCSPacket* self, unsigned char value);
int setBrightChar(struct PCSPacket* self, unsigned char value);
int writePacket(struct PCSPacket* self, int sock);
int readPacket(struct PCSPacket* self, int sock);
