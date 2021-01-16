//
// Created by bernd on 16.01.21.
//

#include "render3d.h"

static const int32_t SCALE_FACTOR = 5;

BRS_Vertex *BRS_Render3D_createVertex(int32_t x, int32_t y, int32_t z) {
    BRS_Vertex *vertex = malloc(sizeof(BRS_Vertex));
    BRS_Point3D *position = malloc(sizeof(BRS_Point3D));
    position->x = x;
    position->y = y;
    position->z = z;
    vertex->position = position;
    vertex->color = BRS_copyColor(&COLOR_YELLOW);
    return vertex;
}

void BRS_Render3D_destroyVertex(BRS_Vertex *vertex) {
    free(vertex->color);
    free(vertex->position);
    free(vertex);
}

static void BRS_drawPoint3D(const BRS_VideoContext *context, const BRS_Point3D *point3D) {
    int32_t z = (point3D->z == 0 ? 1 : point3D->z);
    int32_t viewX = point3D->x / z;
    int32_t viewY = point3D->y / z;
    SDL_RenderDrawPoint(context->renderer, viewX, viewY);
}

static BRS_Point *calcScreenPoint(const BRS_Point3D *point3D, BRS_Point *screenPoint) {
    int32_t z = point3D->z == 0 ? 1 : point3D->z;
    int32_t viewingDistance = 5;
    int32_t offsetX = 400;
    int32_t offsetY = 300;
    screenPoint->x = offsetX + viewingDistance * point3D->x / z;
    screenPoint->y = offsetY + viewingDistance * point3D->y / z;
}

static void BRS_drawLine3D(const BRS_VideoContext *context, const BRS_Line3D *line3D) {
    BRS_Point point2d_1;
    calcScreenPoint(line3D->p1, &point2d_1);

    BRS_Point point2d_2;
    calcScreenPoint(line3D->p2, &point2d_2);

    SDL_RenderDrawLine(context->renderer, point2d_1.x, point2d_1.y, point2d_2.x, point2d_2.y);
}

static void BRS_drawVertex(BRS_VideoContext *context, BRS_Vertex *vertex) {
    BRS_setColor(context, vertex->color);
    BRS_drawPoint3D(context, vertex->position);
}

static void BRS_drawPolygon(BRS_VideoContext *context, BRS_Polygon *polygon) {
    const BRS_Color *color = &COLOR_YELLOW;
    BRS_setColor(context, color);
    BRS_Vertex *lastVertex = NULL;
    BRS_Vertex *firstVertex = NULL;
    BRS_Line3D line3D;
    for (uint8_t verticesIndex = 0; verticesIndex < polygon->numVertices; verticesIndex++) {
//        BRS_drawVertex(context, polygon->vertices[verticesIndex]);
        if (firstVertex == NULL) {
            firstVertex = polygon->vertices[verticesIndex];
        }
        if (lastVertex != NULL) {
            line3D.p1 = lastVertex->position;
            line3D.p2 = polygon->vertices[verticesIndex]->position;
            BRS_drawLine3D(context, &line3D);
        }
        lastVertex = polygon->vertices[verticesIndex];
    }
    line3D.p1 = lastVertex->position;
    line3D.p2 = firstVertex->position;
    BRS_drawLine3D(context, &line3D);
}

void BRS_Render3D_drawObject(BRS_VideoContext *context, BRS_Object3D *object3d) {
    for (uint8_t polygonIndex = 0; polygonIndex < object3d->numPolygons; polygonIndex++) {
        BRS_drawPolygon(context, object3d->polygons[polygonIndex]);
    }
}

void BRS_Render3D_rotateObject(BRS_Object3D *object3d) {

}
