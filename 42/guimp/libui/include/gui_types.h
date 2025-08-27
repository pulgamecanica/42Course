#ifndef GUI_TYPES_H
#define GUI_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // for uint42_t
#include <stdbool.h> // for bool

typedef struct { float x, y, w, h; } GuiRect;
typedef struct { float r, g, b, a; } GuiColor;

typedef uint32_t GuiId;     // stable object id (not pointer)
typedef uint32_t GuiTypeId; // type tag for runtime checks

typedef enum {
    GUI_OK = 0,
    GUI_ERR_LIMIT,
    GUI_ERR_ARGS,
    GUI_ERR_OOM,
} GuiResult;

typedef enum {
    GUI_WINDOW_MOVABLE   = 1u<<0, // 0x00000001  0001
    GUI_WINDOW_RESIZABLE = 1u<<1, // 0x00000002  0010
    GUI_WINDOW_CLOSABLE  = 1u<<2, // 0x00000004  0100
    GUI_WINDOW_VISIBLE   = 1u<<3, // 0x00000008  1000
    GUI_WINDOW_INPUT     = 1u<<4, // receives input
    GUI_WINDOW_AUTO_POS  = 1u<<5, // manager decides initial pos/size
    GUI_WINDOW_AUTO_SIZE = 1u<<6, // manager decides size
} GuiWindowFlags;

#ifdef __cplusplus
} // extern "C" {
#endif

#endif // GUI_TYPES_H
