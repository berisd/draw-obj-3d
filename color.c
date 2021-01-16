//
// Created by bernd on 16.01.21.
//

#include "color.h"

const BRS_Color COLOR_RED = {.r = 255, .g = 0, .b = 0, .a = 255};
const BRS_Color COLOR_BLUE = {.r = 0, .g = 0, .b = 255, .a = 255};
const BRS_Color COLOR_YELLOW = {.r = 255, .g = 255, .b = 0, .a = 255};
const BRS_Color COLOR_WHITE = {.r = 255, .g = 255, .b = 255, .a = 255};
const BRS_Color COLOR_BLACK = {.r = 0, .g = 0, .b = 0, .a = 0};
const BRS_Color COLOR_DARK_GRAY = {.r = 135, .g = 134, .b = 131, .a = 255};
const BRS_Color COLOR_LIGHT_GRAY = {.r = 180, .g = 179, .b = 177, .a = 255};

BRS_Color *BRS_copyColor(const BRS_Color *color) {
    BRS_Color  *copy = malloc(sizeof(BRS_Color));
    copy->r = color->r;
    copy->g = color->g;
    copy->b = color->b;
    copy->a = color->a;
    return copy;
}