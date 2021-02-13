//
// Created by bernd on 16.01.21.
//

#include "render3d.h"

static const int32_t SCALE_FACTOR = 5;

const BRS_Point3D WORLD_ORIGIN = {.x = 0, .y=0, .z=0};

BRS_Vertex3D *BRS_Render3D_createVertex(int32_t x, int32_t y, int32_t z) {
    BRS_Vertex3D *vertex = malloc(sizeof(BRS_Vertex3D));
    BRS_Point3D *position = malloc(sizeof(BRS_Point3D));
    position->x = x;
    position->y = y;
    position->z = z;
    vertex->position = position;
    vertex->color = BRS_copyColor(&COLOR_YELLOW);
    return vertex;
}

void BRS_Render3D_destroyVertex(BRS_Vertex3D *vertex) {
    free(vertex->color);
    free(vertex->position);
    free(vertex);
}

BRS_Transformation3D *BRS_Render3D_createTransformation() {
    BRS_Transformation3D *transformation3D = malloc(sizeof(BRS_Transformation3D));
    transformation3D->rotateDegrees = 0;
    return transformation3D;
}

void BRS_Render3D_destroyTransformation(BRS_Transformation3D *transformation3D) {
    free(transformation3D);
}

static void BRS_drawPoint3D(const BRS_VideoContext *context, const BRS_Point3D *point3D) {
    int32_t z = (point3D->z == 0 ? 1 : point3D->z);
    int32_t viewX = point3D->x / z;
    int32_t viewY = point3D->y / z;
    SDL_RenderDrawPoint(context->renderer, viewX, viewY);
}

static void transformPoint3D(BRS_Point3D *point3D, BRS_Transformation3D *transformation3D) {
//    double rad = M_PI*transformation3D->rotateDegrees/180;
    double rad = DEG_TO_RAD(transformation3D->rotateDegrees);

    int32_t newX = point3D->x;
    int32_t newY = point3D->y;
    int32_t newZ = point3D->z;

    double cos_rad = cos(rad);
    double sin_rad = sin(rad);
    // rotate around z-axis
    newX = point3D->x * cos_rad - point3D->y * sin_rad;
    newY = point3D->y * cos_rad + point3D->x * sin_rad;

    // rotate around y-axis
//    newX = point3D->x * cos_rad + point3D->z * sin_rad;
//    newZ = - point3D->y * sin_rad + point3D->z * cos_rad;

    // rotate around x-axis
//    newY = point3D->y * cos(rad) - point3D->z * sin(rad);
//    newZ = - point3D->y * sin(rad) + point3D->z * cos(rad);

    point3D->x = newX;
    point3D->y = newY;
    point3D->z = newZ;
}

static int32_t oneIfZero(int32_t v) {
    return v == 0 ? 1 : v;
}

static void
calcScreenPoint(const BRS_Point3D *point3Din, BRS_Point *screenPoint, BRS_Transformation3D *transformation3D) {
    BRS_Point3D point3D;
    point3D.x = point3Din->x;
    point3D.y = point3Din->y;
    point3D.z = point3Din->z;

    transformPoint3D(&point3D, transformation3D);

    int32_t pc = 40; // if z_rotation choose sidelen from createCube for smooth rotation
    int32_t x_res = 640;
    int32_t y_res = 480;
    screenPoint->x = ((double)point3D.x / oneIfZero(point3D.z)) * pc + 0.5 * x_res + 0.5;
    screenPoint->y = ((double)point3D.y / oneIfZero(point3D.z)) * -pc + 0.5 * y_res + 0.5;
}

static void
BRS_drawLine3D(const BRS_VideoContext *context, const BRS_Line3D *line3D, BRS_Transformation3D *transformation3D) {
    BRS_Point point2d_1 = {.x = 0, .y = 0};
    calcScreenPoint(line3D->p1, &point2d_1, transformation3D);

    BRS_Point point2d_2 = {.x = 0, .y = 0};
    calcScreenPoint(line3D->p2, &point2d_2, transformation3D);

    SDL_RenderDrawLine(context->renderer, point2d_1.x, point2d_1.y, point2d_2.x, point2d_2.y);
}

static void BRS_Rander3D_drawVertex(BRS_VideoContext *context, BRS_Vertex3D *vertex) {
    BRS_setColor(context, vertex->color);
    BRS_drawPoint3D(context, vertex->position);
}

static void
BRS_Rander3D_drawPolygon(BRS_VideoContext *context, BRS_Polygon3D *polygon, BRS_Transformation3D *transformation3D) {
    BRS_setColor(context, &COLOR_YELLOW);
    BRS_Vertex3D *previousVertex = NULL;
    BRS_Vertex3D *firstVertex = NULL;
    BRS_Line3D line3D;
    for (uint8_t verticesIndex = 0; verticesIndex < polygon->numVertices; verticesIndex++) {
        if (firstVertex == NULL) {
            firstVertex = polygon->vertices[verticesIndex];
        }
        if (previousVertex != NULL) {
            line3D.p1 = previousVertex->position;
            line3D.p2 = polygon->vertices[verticesIndex]->position;
            BRS_drawLine3D(context, &line3D, transformation3D);
        }
        previousVertex = polygon->vertices[verticesIndex];
    }
    line3D.p1 = previousVertex->position;
    line3D.p2 = firstVertex->position;
    BRS_drawLine3D(context, &line3D, transformation3D);
}

void
BRS_Render3D_drawObject(BRS_VideoContext *context, BRS_Object3D *object3d, BRS_Transformation3D *transformation3D) {
    for (uint8_t polygonIndex = 0; polygonIndex < object3d->numPolygons; polygonIndex++) {
        BRS_Rander3D_drawPolygon(context, object3d->polygons[polygonIndex], transformation3D);
    }
}
