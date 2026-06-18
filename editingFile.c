#include "gapbuffer.h"
#include <stdio.h>
#include <stdbool.h>

void saveFile(editorBuffer* buffer)
{
    FILE* file = fopen("layout_save.bin", "wb");
    if (file == NULL) {
        printf("Error opening file for saving!\n");
        return;
    }

    if (buffer->gap_start > 0) {
        fwrite(buffer->buffer, sizeof(char), buffer->gap_start, file);
    }

    int post_gap_size = buffer->size - buffer->gap_end ;
    if (post_gap_size > 0) {
        fwrite(&buffer->buffer[buffer->gap_end], sizeof(char), post_gap_size, file);
    }

    fclose(file);
    printf("Document saved successfully!\n");
}

bool readFile(editorBuffer* Buffer)
{
    FILE* file = fopen("layout_save.bin", "rb"); 
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        size_t element_size = sizeof(Buffer->buffer[0]);
        
        printf("here");
        bool same = false;
        int old_size = Buffer->size;
        int new_size = file_size * 2;
        int old_gend = Buffer->gap_end;
        char* new_buffer = reallocate(Buffer->buffer, new_size, &same);
        if (new_buffer != Buffer->buffer || same == true) {
            Buffer->buffer = new_buffer;
            fread(Buffer->buffer, sizeof(Buffer->buffer[0]), file_size, file);
            fclose(file);
            Buffer->size = new_size;
            Buffer->gap_end = new_size - 1;
            Buffer->gap_start = file_size;
            Buffer->cursor = Buffer->gap_start;
            return true;
        }
        else
        {
            return;
        }
    }
    return;
}