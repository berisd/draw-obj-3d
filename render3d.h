//
// Created by bernd on 16.01.21.
//

#ifndef DRAW_OBJ_3D_RENDER3D_H
#define DRAW_OBJ_3D_RENDER3D_H

#include <stdint.h>
#include "color.h"
#include <math.h>

#define MAX_POLYGONS 16
#define MAX_VERTICES 16

#define DEG_TO_RAD(deg) (M_PI*deg/180)
#define RAD_TO_DEG(rad) (180*rad/M_PI)

typedef struct _BRS_Point3D {
    int32_t x;
    int32_t y;
    int32_t z;

} BRS_Point3D;

typedef struct _BRS_Line3D {
    BRS_Point3D *p1;
    BRS_Point3D *p2;
} BRS_Line3D;

typedef struct _BRS_Vertex {
    BRS_Point3D *position;
    BRS_Color *color;
} BRS_Vertex;

typedef struct _BRS_Polygon {
    uint8_t numVertices;
    BRS_Vertex *vertices[MAX_VERTICES];
} BRS_Polygon;

typedef struct _BRS_Object3D {
    uint8_t numPolygons;
    BRS_Polygon *polygons[MAX_POLYGONS];
} BRS_Object3D;

typedef struct _BRS_Transformation3D {
    uint16_t rotateDegrees;
} BRS_Transformation3D;

void BRS_Render3D_drawObject(BRS_VideoContext *context, BRS_Object3D *object3d, BRS_Transformation3D *transformation3D);

BRS_Vertex *BRS_Render3D_createVertex(int32_t x, int32_t y, int32_t z);

void BRS_Render3D_destroyVertex(BRS_Vertex *vertex);

BRS_Transformation3D *BRS_Render3D_createTransformation();

void BRS_Render3D_destroyTransformation(BRS_Transformation3D *transformation3D);

#endif //DRAW_OBJ_3D_RENDER3D_H
