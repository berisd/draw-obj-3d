//
// Created by bernd on 17.01.21.
//

#ifndef DRAW_OBJ_3D_RENDER_H
#define DRAW_OBJ_3D_RENDER_H

#include "video.h"

#define MAX_POLYGONS 16
#define MAX_VERTICES 16

typedef struct _BRS_Point {
    int32_t x;
    int32_t y;
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

typedef struct _BRS_Vertex {
    BRS_Point *position;
    BRS_Color *color;
} BRS_Vertex;

typedef struct _BRS_Polygon {
    uint8_t numVertices;
    BRS_Vertex *vertices[MAX_VERTICES];
} BRS_Polygon;

typedef struct _BRS_Object2D {
    int32_t sideLength;
    BRS_Polygon *polygon;
} BRS_Object2D;

typedef struct _BRS_Transformation2D {
    uint16_t rotateDegrees;
} BRS_Transformation2D;

typedef struct _BRS_Render2D_Context {
    BRS_VideoContext *videoContext;
    BRS_Point *origin;
} BRS_Render2D_Context;

BRS_Render2D_Context *BRS_Render2D_createContext(const BRS_VideoContext *videoContext, const BRS_Point *origin);

void BRS_Render2D_destroyContext(BRS_Render2D_Context *renderContext);

BRS_Object2D *BRS_Render2D_createObject(int32_t sideLength, BRS_Polygon *polygon);

void BRS_Render2D_destroyObject(BRS_Object2D *object2D);

void BRS_drawPoint(const BRS_VideoContext *context, const BRS_Point *point);

void BRS_drawline(const BRS_VideoContext *context, const BRS_Line *line);

void BRS_drawlRect(const BRS_VideoContext *context, const BRS_Rect *rect);

void BRS_drawlFillRect(const BRS_VideoContext *context, const BRS_Rect *rect);

bool BRS_PointInRect(BRS_Point *point, BRS_Rect *rect);

BRS_Point *BRS_copyPoint(BRS_Point *source);

BRS_Dimension *BRS_copyDimension(BRS_Dimension *source);

BRS_Vertex *BRS_Render_createVertex(int32_t x, int32_t y);

void BRS_Render2D_drawObject(BRS_Render2D_Context *renderContext, BRS_Object2D *object2d,
                             BRS_Transformation2D *transformation2D);

BRS_Transformation2D *BRS_Render2D_createTransformation();

#endif //DRAW_OBJ_3D_RENDER_H
