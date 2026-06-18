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

void Buffer_AddChar(char* string, editorBuffer* orgBuffer);

void Buffer_DelChar(editorBuffer* orgBuffer);

void Buffer_NewLine(editorBuffer* orgBuffer);

void Buffer_navigate_cursor_x(char* key, editorBuffer* orgBuffer);

void Buffer_navigate_cursor_y(editorBuffer* orgBuffer, char* key, int cursor_index);

#endif 
