#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gapbuffer.h"

char* allocate(int size)
{
	char* heap = malloc(size * sizeof(char));
	if (heap == NULL) {
		return NULL;
	}
	return heap;
}

char* reallocate(char* lastHeapAd,int size)
{
	char* nHeap = realloc(lastHeapAd, size * sizeof(char));

	if (nHeap == NULL) {
		return lastHeapAd;
	}
	return nHeap;
}

void Buffer_Full_Check(char* string, editorBuffer* orgBuffer)
{
	if ((orgBuffer->gap_end - orgBuffer->gap_start) <= strlen(string))
	{
		int old_size = orgBuffer->size;
		int new_size = orgBuffer->size * 2;
		char* new_buffer = reallocate(orgBuffer->buffer, new_size);
		if (new_buffer != orgBuffer->buffer) {
			orgBuffer->buffer = new_buffer;
			orgBuffer->size = new_size;
			orgBuffer->gap_end = orgBuffer->gap_end + (orgBuffer->size - old_size);
			printf("%d", orgBuffer->size);
		}

	}
}

void Buffer_AddChar(char* string, editorBuffer* orgBuffer) {

	Buffer_Full_Check(string,orgBuffer);

	int index = 0;
	int cursorpos = orgBuffer->gap_start;

	while (string[index] != '\0')
	{
		orgBuffer->buffer[cursorpos] = string[index];
		index++;
		cursorpos++;
	}

	orgBuffer->gap_start = cursorpos;
}

void Buffer_DelChar(editorBuffer* orgBuffer)
{
	if (orgBuffer->gap_start > 0)
	{
		orgBuffer->gap_start = orgBuffer->gap_start - 1;
	}
}

void Buffer_NewLine(editorBuffer* orgBuffer)
{
	orgBuffer->buffer[orgBuffer->gap_start] = '\n';
	orgBuffer->gap_start++;
}

editorBuffer Buffer_navigate_cursor(char* key, editorBuffer orgBuffer)
{
	if (key == "right")
	{
		orgBuffer.gap_start = orgBuffer.gap_start + 1;
		return orgBuffer;
	}
	if (key == "left")
	{
		orgBuffer.gap_start = orgBuffer.gap_start - 1;
		return orgBuffer;
	}
}

editorBuffer Buffer_Init()
{
	editorBuffer array;
	array.size = 100;
	array.gap_start = 0;
	array.gap_end = 100;
	array.buffer = allocate(array.size);
	return array;
}