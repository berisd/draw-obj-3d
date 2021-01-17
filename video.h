//
// Created by bernd on 02.01.21.
//

#ifndef DRAW_OBJ_3D_VIDEO_H
#define DRAW_OBJ_3D_VIDEO_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "color.h"

#define DEG_TO_RAD(deg) (M_PI*deg/180)
#define RAD_TO_DEG(rad) (180*rad/M_PI)

typedef struct BRS_VideoContext {
    uint32_t screenWidth;
    uint32_t screenHeight;
    SDL_Window *window;
    SDL_Renderer *renderer;
} BRS_VideoContext;

struct BRS_Dimension {
    int32_t width;
    int32_t height;
};
typedef struct BRS_Dimension BRS_Dimension;

BRS_VideoContext *BRS_initVideo(uint32_t screenWidth, uint32_t screenHeight);

void BRS_shutdownVideo(const BRS_VideoContext *context);

void BRS_clearVideo(const BRS_VideoContext *context);

void BRS_updateVideo(const BRS_VideoContext *context);

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color);

#endif //DRAW_OBJ_3D_VIDEO_H
