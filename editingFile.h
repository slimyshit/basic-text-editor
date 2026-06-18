#pragma once
#ifndef EDITINGFILE_H
#define EDITINGFILE_H
#include "gapbuffer.h"

void saveFile(editorBuffer* buffer);

bool readFile(editorBuffer* Buffer);

#endif