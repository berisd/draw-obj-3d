//
// Created by bernd on 16.01.21.
//

#ifndef DRAW_OBJ_3D_RENDER3D_H
#define DRAW_OBJ_3D_RENDER3D_H

#include <stdint.h>
#include <math.h>
#include "color.h"
#include "video.h"
#include "render.h"

typedef struct _BRS_Point3D {
    double x;
    double y;
    double z;

} BRS_Point3D;

typedef struct _BRS_Line3D {
    BRS_Point3D *p1;
    BRS_Point3D *p2;
} BRS_Line3D;

typedef struct _BRS_Vertex3D {
    BRS_Point3D *position;
    BRS_Color *color;
} BRS_Vertex3D;

typedef struct _BRS_Polygon3D {
    uint8_t numVertices;
    BRS_Vertex3D *vertices[MAX_VERTICES];
} BRS_Polygon3D;

typedef struct _BRS_Object3D {
    uint8_t numPolygons;
    BRS_Polygon3D *polygons[MAX_POLYGONS];
} BRS_Object3D;

typedef struct _BRS_Transformation3D {
    uint16_t rotateDegrees;
} BRS_Transformation3D;

const BRS_Point3D WORLD_ORIGIN;

void BRS_Render3D_drawObject(BRS_VideoContext *context, BRS_Object3D *object3d);

BRS_Vertex3D *BRS_Render3D_createVertex(int32_t x, int32_t y, int32_t z);

void BRS_Render3D_destroyVertex(BRS_Vertex3D *vertex);

BRS_Transformation3D *BRS_Render3D_createTransformation();

void BRS_Render3D_destroyTransformation(BRS_Transformation3D *transformation3D);

void BRS_Render3D_calcObject(BRS_VideoContext *context, BRS_Object3D *object3d, BRS_Object3D *object3d_transformed, BRS_Transformation3D *transformation3D);

#endif //DRAW_OBJ_3D_RENDER3D_H
