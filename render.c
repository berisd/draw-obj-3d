//
// Created by bernd on 17.01.21.
//

#include "render.h"

BRS_Render2D_Context *BRS_Render2D_createContext(const BRS_VideoContext *videoContext, const BRS_Point *origin) {
    BRS_Render2D_Context *renderContext = malloc(sizeof(BRS_Render2D_Context));
    renderContext->videoContext = (BRS_VideoContext *) videoContext;
    renderContext->origin = BRS_copyPoint((BRS_Point *) origin);
    return renderContext;
}

void BRS_Render2D_destroyContext(BRS_Render2D_Context *renderContext) {
    free(renderContext->origin);
    free(renderContext);
}

BRS_Object2D *BRS_Render2D_createObject(int32_t sideLength, BRS_Polygon *polygon) {
    BRS_Object2D *obj2d = malloc(sizeof(BRS_Object2D));
    obj2d->sideLength = sideLength;
    obj2d->polygon = polygon;
    return obj2d;
}

static void BRS_Render2D_destroyVertex(BRS_Vertex *vertex) {
    free(vertex->position);
    free(vertex);
}

static void BRS_Render2D_destroyPolygon(BRS_Polygon *polygon) {
    for (uint16_t i = 0; i < polygon->numVertices; i++) {
        BRS_Render2D_destroyVertex(polygon->vertices[i]);
    }
    free(polygon);
}

void BRS_Render2D_destroyObject(BRS_Object2D *object2D) {
    BRS_Render2D_destroyPolygon(object2D->polygon);
    free(object2D);
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
calcScreenPoint(BRS_Render2D_Context *renderContext, const BRS_Point *point2D, BRS_Point *screenPoint,
                BRS_Transformation2D *transformation2D, int32_t sideLength) {
    int32_t originX = renderContext->origin->x - sideLength / 2;
    int32_t originY = renderContext->origin->y - sideLength / 2;

    BRS_Point newPoint = {.x = point2D->x - originX, .y = point2D->y - originY};

    double rad = DEG_TO_RAD(transformation2D->rotateDegrees);
    screenPoint->x = newPoint.x * cos(rad) - newPoint.y * sin(rad);
    screenPoint->y = newPoint.y * cos(rad) + newPoint.x * sin(rad);

    screenPoint->x += originX;
    screenPoint->y += originY;
}

static void BRS_Render_drawPolygon(BRS_Render2D_Context *renderContext, BRS_Object2D *object2D,
                                   BRS_Transformation2D *transformation2D) {
    const BRS_Color *color = &COLOR_YELLOW;
    BRS_Polygon *polygon = object2D->polygon;
    BRS_setColor(renderContext->videoContext, color);
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
            calcScreenPoint(renderContext, lastVertex->position, line.p1, transformation2D, object2D->sideLength);
            calcScreenPoint(renderContext, polygon->vertices[verticesIndex]->position, line.p2, transformation2D,
                            object2D->sideLength);
            BRS_drawline(renderContext->videoContext, &line);
        }
        lastVertex = polygon->vertices[verticesIndex];
    }
    calcScreenPoint(renderContext, lastVertex->position, line.p1, transformation2D, object2D->sideLength);
    calcScreenPoint(renderContext, firstVertex->position, line.p2, transformation2D, object2D->sideLength);
    BRS_drawline(renderContext->videoContext, &line);
}

BRS_Transformation2D *BRS_Render2D_createTransformation() {
    BRS_Transformation2D *transformation2D = malloc(sizeof(BRS_Transformation2D));
    transformation2D->rotateDegrees = 0;
    return transformation2D;
}

void BRS_Render2D_drawObject(BRS_Render2D_Context *renderContext, BRS_Object2D *object2d,
                             BRS_Transformation2D *transformation2D) {
    BRS_Render_drawPolygon(renderContext, object2d, transformation2D);
}
