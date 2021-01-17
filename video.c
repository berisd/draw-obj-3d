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

BRS_Vertex *BRS_Render_createVertex(int32_t x, int32_t y) {
    BRS_Vertex *vertex = malloc(sizeof(BRS_Vertex));
    BRS_Point *position = malloc(sizeof(BRS_Point));
    position->x = x;
    position->y = y;
    vertex->position = position;
    vertex->color = BRS_copyColor(&COLOR_YELLOW);
    return vertex;
}

static void
calcScreenPoint(const BRS_Point *point2D, BRS_Point *screenPoint, BRS_Transformation2D *transformation2D) {
    int32_t sidelen = 50;
    int32_t originX = (800-sidelen) / 2;
    int32_t originY = (600-sidelen) / 2;

    BRS_Point newPoint = {.x = point2D->x - originX, .y = point2D->y - originY};

    double rad = DEG_TO_RAD(transformation2D->rotateDegrees);
    screenPoint->x = newPoint.x * cos(rad) - newPoint.y * sin(rad);
    screenPoint->y = newPoint.y * cos(rad) + newPoint.x * sin(rad);

    screenPoint->x += originX;
    screenPoint->y += originY;
}

void BRS_Render_drawPolygon(BRS_VideoContext *context, BRS_Polygon *polygon, BRS_Transformation2D *transformation2D) {
    const BRS_Color *color = &COLOR_YELLOW;
    BRS_setColor(context, color);
    BRS_Vertex *lastVertex = NULL;
    BRS_Vertex *firstVertex = NULL;
    BRS_Point p1;
    BRS_Point p2;
    BRS_Line line = {.p1 = &p1, .p2 = &p2};

    for (uint8_t verticesIndex = 0; verticesIndex < polygon->numVertices; verticesIndex++) {
        if (firstVertex == NULL) {
            firstVertex = polygon->vertices[verticesIndex];
        }
        if (lastVertex != NULL) {
            calcScreenPoint(lastVertex->position, line.p1, transformation2D);
            calcScreenPoint(polygon->vertices[verticesIndex]->position, line.p2, transformation2D);
            BRS_drawline(context, &line);
        }
        lastVertex = polygon->vertices[verticesIndex];
    }
    calcScreenPoint(lastVertex->position, line.p1, transformation2D);
    calcScreenPoint(firstVertex->position, line.p2, transformation2D);
    BRS_drawline(context, &line);
}

BRS_Transformation2D *BRS_Render2D_createTransformation() {
    BRS_Transformation2D *transformation2D = malloc(sizeof(BRS_Transformation2D));
    transformation2D->rotateDegrees = 0;
    return transformation2D;
}
