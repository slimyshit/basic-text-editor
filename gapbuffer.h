#pragma once
#ifndef GAPBUFFER_H
#define	GAPBUFFER_H
typedef struct {
	char* buffer;
	int size;
	int gap_start;
	int gap_end;
	int cursor;
}editorBuffer;

editorBuffer Buffer_Init();

editorBuffer Buffer_AddChar(char* string, editorBuffer orgBuffer);

#endif // 
