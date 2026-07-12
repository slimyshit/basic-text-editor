# basic-text-editor

A basic GUI text editor written in C, using a gap buffer for efficient text storage and editing.

## What it does

A windowed text editor with cursor movement, text editing, and file I/O, built from scratch without a text-editing framework. Text is stored in a gap buffer for efficient insertion/deletion near the cursor, and rendering uses a texture cache to avoid re-rendering unchanged text every frame.

## Features

- Gap buffer for efficient text insertion and deletion
- Cursor movement and positioning
- File loading and saving
- Windowed GUI with event handling (keyboard input, etc.)
- Texture caching for rendering performance

## Build

Requires SDL3 and SDL3_ttf.

Open `text editor.slnx` in Visual Studio and build, or via the command line:

Make sure SDL3 and SDL3_ttf are linked in the project settings (include/lib paths configured in Visual Studio).

## Run

Run the built executable from Visual Studio, or launch it directly after building.

## Architecture

- `gapbuffer.c/h` — core text storage data structure
- `cursor.c/h` — cursor position and movement logic
- `eventHandling.c/h` — keyboard/input event processing
- `editingFile.c/h` — file loading and saving
- `layout.c/h` — text layout and positioning for rendering
- `textureCache.c/h` — caches rendered text as textures for performance
