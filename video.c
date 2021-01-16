//
// Created by bernd on 02.01.21.
//
#include <stdbool.h>
#include "video.h"

static const uint8_t PIXELS_BETWEEN_CHARS = 1;

static BRS_VideoContext *createVideoContext(int32_t screenWidth, int32_t screenHeight) {
    BRS_VideoContext *context = malloc(sizeof(BRS_VideoContext));
    context->window = NULL;
    context->renderer = NULL;
    context->screenWidth = screenWidth;
    context->screenHeight = screenHeight;
    return context;
}

static bool initSdl(BRS_VideoContext *videoContext) {
    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using SDL version %d.%d.%d", sdlVersion.major, sdlVersion.minor,
                sdlVersion.patch);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    videoContext->window = SDL_CreateWindow("Fontedit", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            videoContext->screenWidth,
                                            videoContext->screenHeight,
                                            SDL_WINDOW_SHOWN);

    if (videoContext->window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    videoContext->renderer = SDL_CreateRenderer(videoContext->window, -1, 0);

    if (videoContext->renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    return true;
}

BRS_VideoContext *BRS_initVideo(uint32_t screenWidth, uint32_t screenHeight) {
    BRS_VideoContext *context = createVideoContext(screenWidth, screenHeight);
    if (!initSdl(context)) {
        free(context);
        return NULL;
    }
    return context;
}

void BRS_shutdownVideo(const BRS_VideoContext *context) {
    SDL_DestroyRenderer(context->renderer);
    SDL_DestroyWindow(context->window);
    free((BRS_VideoContext *) context);
}

void BRS_clearVideo(const BRS_VideoContext *context) {
    SDL_RenderClear(context->renderer);
}

void BRS_setColor(const BRS_VideoContext *context, const BRS_Color *color) {
    SDL_SetRenderDrawColor(context->renderer, color->r, color->g, color->b, color->a);
}

void BRS_updateVideo(const BRS_VideoContext *context) {
    SDL_RenderPresent(context->renderer);
}

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point) {
    SDL_RenderDrawPoint(context->renderer, point->x, point->y);
}

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line) {
    SDL_RenderDrawLine(context->renderer, line->p1->x, line->p1->y, line->p2->x, line->p2->y);
}

void BRS_drawlRect(const BRS_VideoContext *context, const BRS_Rect *rect) {
    SDL_Rect sdlRect = {.x = rect->x, .y = rect->y, .w = rect->width, .h = rect->height};
    SDL_RenderDrawRect(context->renderer, &sdlRect);
}

void BRS_drawlFillRect(const BRS_VideoContext *context, const BRS_Rect *rect) {
    SDL_Rect sdlRect = {.x = rect->x, .y = rect->y, .w = rect->width, .h = rect->height};
    SDL_RenderFillRect(context->renderer, &sdlRect);
}

bool BRS_PointInRect(BRS_Point *point, BRS_Rect *rect) {
    bool intersect = (point->x >= rect->x && point->x <= rect->x + rect->width &&
                      point->y >= rect->y && point->y <= rect->y + rect->height);
    return intersect;
}

BRS_Point *BRS_copyPoint(BRS_Point *source) {
    BRS_Point *copy = malloc(sizeof(source));
    memcpy(copy, source, sizeof(source));
    return copy;
}

BRS_Dimension *BRS_copyDimension(BRS_Dimension *source) {
    struct BRS_Dimension *copy = malloc(sizeof(source));
    memcpy(copy, source, sizeof(source));
    return copy;
}
