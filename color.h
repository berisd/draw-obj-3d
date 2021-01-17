//
// Created by bernd on 16.01.21.
//

#ifndef DRAW_OBJ_3D_COLOR_H
#define DRAW_OBJ_3D_COLOR_H

#include <stdint.h>
#include <stdlib.h>

struct _BRS_Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
typedef struct _BRS_Color BRS_Color;

const BRS_Color COLOR_RED;
const BRS_Color COLOR_BLUE;
const BRS_Color COLOR_YELLOW;
const BRS_Color COLOR_WHITE;
const BRS_Color COLOR_BLACK;
const BRS_Color COLOR_DARK_GRAY;
const BRS_Color COLOR_LIGHT_GRAY;

BRS_Color *BRS_copyColor(const BRS_Color *color);

#endif //DRAW_OBJ_3D_COLOR_H
