#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gapbuffer.h"
#include "textureCache.h"

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

	if (nHeap == NULL) {
		return lastHeapAd;
	}
	if (nHeap == lastHeapAd)
	{
		*same_ad = true;
	}
	return nHeap;
}

void Buffer_Full_Check(char* string, editorBuffer* orgBuffer)
{
	if (orgBuffer->gap_end == orgBuffer->gap_start)
	{
		bool same = false;
		int old_size = orgBuffer->size;
		int new_size = orgBuffer->size * 2;
		int old_gend = orgBuffer->gap_end;
		int copy_length = old_size - orgBuffer->gap_end - 1;
		char* new_buffer = reallocate(orgBuffer->buffer, new_size, &same);
		if (new_buffer != orgBuffer->buffer || same == true) {
			orgBuffer->buffer = new_buffer;
			orgBuffer->size = new_size;
			orgBuffer->gap_end = new_size - copy_length - 1;
			memmove(orgBuffer->buffer + orgBuffer->gap_end + 1, orgBuffer->buffer + old_gend + 1, copy_length);
			printf("%d", orgBuffer->size);
		}
	}
}

void Buffer_AddChar(char* string, editorBuffer* orgBuffer) {

	Buffer_Full_Check(string,orgBuffer);

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
		orgBuffer->gap_start = orgBuffer->gap_start - 1;
		orgBuffer->cursor--;
	}
}

void Buffer_NewLine(editorBuffer* orgBuffer)
{
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
		orgBuffer->cursor--;
		orgBuffer->gap_end--;
		char overlap = orgBuffer->buffer[orgBuffer->gap_start];
		orgBuffer->buffer[(orgBuffer->gap_end) + 1] = overlap;
	}
}
/*void Buffer_navigate_cursor_y(char* key, editorBuffer* orgBuffer, SDL_Window* window)
{
	cursor n_cus;
	int lines;
	char* text = wrap_the_ffing_lines(orgBuffer, window, &n_cus, &lines);
	if (strcmp(key, "up") == 0 && n_cus.cursor_line > 0 && n_cus.cursor_line <= lines)
	{
		n_cus.cursor_line--;

	}
}
*/

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


