#pragma once
#ifndef GAPBUFFER_H
#define	GAPBUFFER_H
typedef struct {
	char* buffer;
	int size;
	int gap_start;
	int gap_end;
	char mode[10];
}editorBuffer;

editorBuffer Buffer_Init();

void Buffer_AddChar(char* string, editorBuffer* orgBuffer);

void Buffer_DelChar(editorBuffer* orgBuffer);

void Buffer_NewLine(editorBuffer* orgBuffer);

#endif // 
