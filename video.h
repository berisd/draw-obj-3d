//
// Created by bernd on 02.01.21.
//

#ifndef DRAW_OBJ_3D_VIDEO_H
#define DRAW_OBJ_3D_VIDEO_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct BRS_VideoContext {
    uint32_t screenWidth;
    uint32_t screenHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
} BRS_VideoContext;

struct _BRS_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
typedef struct _BRS_Color BRS_Color;

typedef struct _BRS_Point {
    int x;
    int y;
} BRS_Point;
typedef struct _BRS_Point BRS_Point;

struct BRS_Line {
    BRS_Point *p1;
    BRS_Point *p2;
};
typedef struct BRS_Line BRS_Line;

typedef struct _BRS_Rect {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
} BRS_Rect;

struct BRS_Dimension {
    int32_t width;
    int32_t height;
};
typedef struct BRS_Dimension BRS_Dimension;

BRS_VideoContext *BRS_initVideo(uint32_t screenWidth, uint32_t screenHeight);

void BRS_shutdownVideo(const BRS_VideoContext *context);

void BRS_clearVideo(const BRS_VideoContext *context);

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color);

void BRS_updateVideo(const BRS_VideoContext *context);

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point);

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line);

void BRS_drawlRect(const BRS_VideoContext *context, const BRS_Rect *rect);

void BRS_drawlFillRect(const BRS_VideoContext *context, const BRS_Rect *rect);

bool BRS_PointInRect(BRS_Point *point, BRS_Rect *rect);

BRS_Point *BRS_copyPoint(BRS_Point *source);

BRS_Dimension *BRS_copyDimension(BRS_Dimension *source);

#endif //DRAW_OBJ_3D_VIDEO_H
