#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define NEXT_FILE "index/next.ind"

unsigned int generateAllIndicies();
unsigned int addFile(char* path);

int main(int argc, const char* argv[])
{
	generateAllIndicies();
	
	return 0;
}

unsigned int generateAllIndicies()
{
	DIR* d;
	struct dirent* dir;
	mkdir("index");
	
	
	unsigned int next_index = 0;
	
	d = opendir(".");
	
	while((dir = readdir(d))!=NULL)
	{
		char* fname = strdup(dir->d_name);
		char* path = (char*) malloc(1024);
		sprintf(path, "index/%u.ind", next_index);
		FILE* f = fopen(path, "w");
		fprintf(f, "%s", fname);
		fclose(f);
		next_index++;
		free(path);
	}
	FILE* n = fopen(NEXT_FILE, "w");
	fprintf(n, "%u", next_index);
	fclose(n)
	closedir(d);
	
	return next_index;
}

unsigned int addFile(char* path)
{
	unsigned int next_index = 0;
	FILE* ifile = fopen(NEXT_FILE, "r");
	if( ifile != NULL )
	{
		fread(&next_index, sizeof(unsigned int), 1, ifile);
		fclose(ifile);
		
		char* p = (char*) malloc(1024);
		sprintf(p, "index/%u.ind", next_index);
		FILE* jfile = fopen(p, "w");
		fprintf(jfile, "%s", path);
		fclose(jfile);
		next_index++;
		
		FILE* ifile = fopen(NEXT_FILE, "w");
		fprintf(ifile, "%u", next_index);
		fclose(ifile);
		
		free(p);
	}
	
	
	
	return next_index;
	
}