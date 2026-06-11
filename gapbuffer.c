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

editorBuffer Buffer_AddChar(char *string, editorBuffer orgBuffer) {

	if ((orgBuffer.gap_end - orgBuffer.gap_start) <= strlen(string))
	{
		int old_size = orgBuffer.size;
		orgBuffer.size = orgBuffer.size * 2;
		orgBuffer.buffer = reallocate(orgBuffer.buffer, orgBuffer.size);
		orgBuffer.gap_end = orgBuffer.gap_end + (orgBuffer.size - old_size);
	}

	int index = 0;
	int cursorpos = orgBuffer.cursor;

	while (string[index] != '\0')
	{
		orgBuffer.buffer[cursorpos] = string[index];
		index++;
		cursorpos++;
	}

	orgBuffer.cursor = cursorpos;
	orgBuffer.gap_start = cursorpos;


	return orgBuffer;
}

editorBuffer Buffer_Init()
{
	editorBuffer array;
	array.size = 100;
	array.gap_start = 0;
	array.gap_end = 100;
	array.cursor = 0;
	array.buffer = allocate(array.size);
	return array;
}