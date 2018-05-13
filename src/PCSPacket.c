#include <stdio.h>
#include <stdlib.h>
#include "PCSDefine.h"
static int PCSError = 0;
int setCommand(struct PCSPacket* self, unsigned int value){
	int ret = -1;
	if(self != NULL){
		self->command = value;
		ret = 0;
	}else
		PCSError = PCSNullPacketPointer;
	return ret;
}
int setRed(struct PCSPacket* self, unsigned int value){
	int ret = -1;
	if(self != NULL){
		self->red = value;
	}else
		PCSError = PCSNullPacketPointer;
	return ret;
}
int setGreen(struct PCSPacket* self, unsigned int value){
	int ret = -1;
	
	return ret;
}
int setBlue(struct PCSPacket* self, unsigned int value){
	int ret = -1;
	
	return ret;
}
int setBright(struct PCSPacket* self, unsigned int value){
	int ret = -1;
	
	return ret;
}
int setCommandChar(struct PCSPacket* self, unsigned char value){
	int ret = -1;
	
	return ret;
}
int setRedChar(struct PCSPacket* self, unsigned char value){
	int ret = -1;
	
	return ret;
}
int setGreenChar(struct PCSPacket* self, unsigned char value){
	int ret = -1;
	
	return ret;
}
int setBlueChar(struct PCSPacket* self, unsigned char value){
	int ret = -1;
	
	return ret;
}
int setBrightChar(struct PCSPacket* self, unsigned char value){
	int ret = -1;
	
	return ret;
}
int writePacket(struct PCSPacket* self, int sock){
	int ret = -1;
	
	return ret;
}
int readPacket(struct PCSPacket* self, int sock){
	int ret = -1;
	
	return ret;
}
