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

    videoContext->window = SDL_CreateWindow("3D Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                            videoContext->screenWidth,
                                            videoContext->screenHeight,
                                            SDL_WINDOW_SHOWN);

    if (videoContext->window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    videoContext->renderer = SDL_CreateRenderer(videoContext->window, -1, SDL_RENDERER_PRESENTVSYNC);

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

BRS_Dimension *BRS_copyDimension(BRS_Dimension *source) {
    struct BRS_Dimension *copy = malloc(sizeof(source));
    memcpy(copy, source, sizeof(source));
    return copy;
}
