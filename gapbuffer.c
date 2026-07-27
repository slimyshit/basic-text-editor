#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gapbuffer.h"


char* allocate(int size)
{
	char* heap = malloc(size * sizeof(char));
	if (heap == NULL) {
		return NULL;
	}
	return heap;
}

char* reallocate(char* lastHeapAd,int size, bool* same_ad)
{
	char* nHeap = realloc(lastHeapAd, size * sizeof(char));
	*same_ad = (nHeap != NULL);
	if (nHeap == NULL) {
		return lastHeapAd;
	}
	return nHeap;
}

void Buffer_Full_Check(editorBuffer* orgBuffer)
{
	if (orgBuffer->gap_end <= orgBuffer->gap_start + 1)
	{
		bool same = false;
		int old_size = orgBuffer->size;
		int new_size = orgBuffer->size * 2;
		int old_gend = orgBuffer->gap_end;	
		int copy_length = old_size - orgBuffer->gap_end - 1;

		char* new_buffer = reallocate(orgBuffer->buffer, new_size, &same);
			
		if (!same) {
			return;
		}
		orgBuffer->buffer = new_buffer;
		orgBuffer->size = new_size;
		orgBuffer->gap_end = new_size - copy_length - 1;
		memmove(orgBuffer->buffer + orgBuffer->gap_end + 1, orgBuffer->buffer + old_gend + 1, copy_length);
	}
}

void Buffer_AddChar(char* string, editorBuffer* orgBuffer) {

	Buffer_Full_Check(orgBuffer);

	int index = 0;

	while (string[index] != '\0')
	{
		orgBuffer->buffer[orgBuffer->gap_start] = string[index];
		index++;
		orgBuffer->gap_start++;
		orgBuffer->cursor++;
	}
}

void Buffer_DelChar(editorBuffer* orgBuffer)
{
	if (orgBuffer->gap_start > 0)
	{
		orgBuffer->gap_start--;
		orgBuffer->cursor--;
	}
}

void Buffer_NewLine(editorBuffer* orgBuffer)
{
	Buffer_Full_Check(orgBuffer);
	orgBuffer->buffer[orgBuffer->gap_start] = '\n';
	orgBuffer->gap_start++;
	orgBuffer->cursor++;
}

void Buffer_navigate_cursor_x(char*key, editorBuffer* orgBuffer)
{
	if (strcmp(key, "right") == 0 && orgBuffer->gap_end < (orgBuffer->size - 1))
	{
		
		char overlap = orgBuffer->buffer[orgBuffer->gap_end + 1];
		orgBuffer->buffer[(orgBuffer->gap_start)] = overlap;
		orgBuffer->gap_end++;
		orgBuffer->gap_start++;
		orgBuffer->cursor++;
	}
	if (strcmp(key, "left") == 0 && orgBuffer->gap_start > 0)
	{
		orgBuffer->gap_start--;
		orgBuffer->gap_end--;
		char overlap = orgBuffer->buffer[orgBuffer->gap_start];
		orgBuffer->buffer[(orgBuffer->gap_end) + 1] = overlap;
		orgBuffer->cursor--;
	}
}

void Buffer_navigate_cursor_y(editorBuffer* orgBuffer, char* key, int cursor_index)
{
	int gap_size = orgBuffer->gap_end - orgBuffer->gap_start + 1;

	if (strcmp(key, "up") == 0)
	{
		int diff = orgBuffer->gap_start - cursor_index;
		printf("gap start = %d    diff = %d\n", orgBuffer->gap_start, diff);
		memmove(orgBuffer->buffer + cursor_index + gap_size, orgBuffer->buffer + cursor_index, diff);
		orgBuffer->cursor = cursor_index;
		orgBuffer->gap_start = cursor_index;
		orgBuffer->gap_end = cursor_index + gap_size - 1;
	}
	else if (strcmp(key, "down") == 0)
	{
		int diff = cursor_index - orgBuffer->gap_start;

		memmove(orgBuffer->buffer + orgBuffer->gap_start, orgBuffer->buffer + orgBuffer->gap_end + 1, diff);

		orgBuffer->cursor = cursor_index;
		orgBuffer->gap_start = cursor_index;
		orgBuffer->gap_end = cursor_index + gap_size - 1;
	}
}

editorBuffer Buffer_Init()
{
	editorBuffer array;
	array.size = 100;
	array.gap_start = 0;
	array.cursor = 0;
	array.gap_end = 99;
	array.buffer = allocate(array.size);
	return array;
}


