#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "video.h"
#include "color.h"
#include "render.h"
#include "render3d.h"

typedef struct ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} ApplicationConfig;

typedef struct _ApplicationState {
    BRS_VideoContext *videoContext;
    BRS_Render2D_Context *render2DContext;
    BRS_Object2D *object2D;
    BRS_Point3D *origin3D;
    BRS_Object3D *object3D;
    BRS_Transformation3D *transformation3D;
    BRS_Transformation2D *transformation2D;
    bool quit;
} ApplicationState;

static ApplicationState *applicationState = NULL;

static ApplicationConfig *createConfig() {
    ApplicationConfig *config = malloc(sizeof(ApplicationConfig));
    config->screenWidth = 800;
    config->screenHeight = 600;
    return config;
}

static void destroyConfig(ApplicationConfig *config) {
    free(config);
}

static ApplicationState *createApplicationState() {
    ApplicationState *applicationState = malloc(sizeof(ApplicationState));
    return applicationState;
}

static void destroyApplicationState(ApplicationState *applicationState) {
    BRS_Render2D_destroyObject(applicationState->object2D);
    //TODO destroy object3D
    //TODO destroy transformation2D
    //TODO destroy transformation3D
    BRS_Render2D_destroyContext(applicationState->render2DContext);
    free(applicationState);
}

static BRS_Object2D *createRect(const BRS_Render2D_Context *renderContext) {
    BRS_Object2D *obj2d = malloc(sizeof(BRS_Object2D));
    obj2d->sideLength = 50;

    int32_t originX = renderContext->origin->x - (obj2d->sideLength/2);
    int32_t originY = renderContext->origin->y - (obj2d->sideLength/2);
    BRS_Polygon *polygon = malloc(sizeof(BRS_Polygon));
    polygon->numVertices = 4;
    polygon->vertices[0] = BRS_Render_createVertex(originX, originY);
    polygon->vertices[1] = BRS_Render_createVertex(originX, originY+obj2d->sideLength);
    polygon->vertices[2] = BRS_Render_createVertex(originX+obj2d->sideLength, originY+obj2d->sideLength);
    polygon->vertices[3] = BRS_Render_createVertex(originX+obj2d->sideLength, originY);

    obj2d->polygon = polygon;
    return obj2d;
}

static BRS_Object3D *createCube() {
    int32_t sideLen = 10;

    BRS_Object3D *obj3d = malloc(sizeof(BRS_Object3D));

    BRS_Polygon3D *frontFace = malloc(sizeof(BRS_Polygon3D));
    frontFace->numVertices = 4;
    frontFace->vertices[0] = BRS_Render3D_createVertex(0, 0, sideLen);
    frontFace->vertices[1] = BRS_Render3D_createVertex(sideLen, 0, sideLen);
    frontFace->vertices[2] = BRS_Render3D_createVertex(sideLen, sideLen, sideLen);
    frontFace->vertices[3] = BRS_Render3D_createVertex(0, sideLen, sideLen);

    BRS_Polygon3D *backFace = malloc(sizeof(BRS_Polygon3D));
    backFace->numVertices = 4;
    backFace->vertices[0] = BRS_Render3D_createVertex(0, 0, 0);
    backFace->vertices[1] = BRS_Render3D_createVertex(sideLen, 0, 0);
    backFace->vertices[2] = BRS_Render3D_createVertex(sideLen, sideLen, 0);
    backFace->vertices[3] = BRS_Render3D_createVertex(0, sideLen, 0);

    BRS_Polygon3D *bottomFace = malloc(sizeof(BRS_Polygon3D));
    bottomFace->numVertices = 4;
    bottomFace->vertices[0] = BRS_Render3D_createVertex(0, 0, sideLen);
    bottomFace->vertices[1] = BRS_Render3D_createVertex(sideLen, 0, sideLen);
    bottomFace->vertices[2] = BRS_Render3D_createVertex(sideLen, 0, 0);
    bottomFace->vertices[3] = BRS_Render3D_createVertex(0, 0, 0);

    BRS_Polygon3D *topFace = malloc(sizeof(BRS_Polygon3D));
    topFace->numVertices = 4;
    topFace->vertices[0] = BRS_Render3D_createVertex(0, sideLen, sideLen);
    topFace->vertices[1] = BRS_Render3D_createVertex(sideLen, sideLen, sideLen);
    topFace->vertices[2] = BRS_Render3D_createVertex(sideLen, sideLen, 0);
    topFace->vertices[3] = BRS_Render3D_createVertex(0, sideLen, 0);

    BRS_Polygon3D *leftFace = malloc(sizeof(BRS_Polygon3D));
    leftFace->numVertices = 4;
    leftFace->vertices[0] = BRS_Render3D_createVertex(0, 0, sideLen);
    leftFace->vertices[1] = BRS_Render3D_createVertex(0, 0, 0);
    leftFace->vertices[2] = BRS_Render3D_createVertex(0, sideLen, 0);
    leftFace->vertices[3] = BRS_Render3D_createVertex(0, sideLen, sideLen);

    BRS_Polygon3D *rightFace = malloc(sizeof(BRS_Polygon3D));
    rightFace->numVertices = 4;
    rightFace->vertices[0] = BRS_Render3D_createVertex(sideLen, 0, sideLen);
    rightFace->vertices[1] = BRS_Render3D_createVertex(sideLen, 0, 0);
    rightFace->vertices[2] = BRS_Render3D_createVertex(sideLen, sideLen, 0);
    rightFace->vertices[3] = BRS_Render3D_createVertex(sideLen, sideLen, sideLen);

    obj3d->numPolygons = 1;
    obj3d->polygons[0] = frontFace;
//    obj3d->polygons[1] = backFace;
//    obj3d->polygons[2] = topFace;
//    obj3d->polygons[3] = bottomFace;
//    obj3d->polygons[4] = leftFace;
//    obj3d->polygons[5] = rightFace;

//    obj3d->numPolygons = 2;
//    obj3d->polygons[0] = frontFace;
//    obj3d->polygons[1] = backFace;
//    obj3d->polygons[2] = topFace;
//    obj3d->polygons[3] = bottomFace;
//    obj3d->polygons[4] = leftFace;
//    obj3d->polygons[5] = rightFace;


    return obj3d;
}

static ApplicationState *initApplication(const ApplicationConfig *config) {
    ApplicationState *applicationState = createApplicationState();
    BRS_VideoContext *videoContext = BRS_initVideo(config->screenWidth, config->screenHeight);
    if (videoContext == NULL) {
        return NULL;
    }
    BRS_Point origin2D = { .x = videoContext->screenWidth / 2, .y = videoContext->screenHeight / 2};

    applicationState->videoContext = videoContext;
    applicationState->render2DContext = BRS_Render2D_createContext(videoContext, &origin2D);
    applicationState->object3D = createCube();
    applicationState->object2D = createRect(applicationState->render2DContext);
    applicationState->transformation3D = BRS_Render3D_createTransformation();
    applicationState->transformation2D = BRS_Render2D_createTransformation();
    atexit(SDL_Quit);
    return applicationState;
}

static bool checkQuitApplication(SDL_Event *event) {
    switch (event->type) {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            switch (event->key.keysym.sym) {
                case SDLK_ESCAPE:
                    return true;
            }
    }
    return false;
}

static void handleVideo(ApplicationState *applicationState) {
    BRS_setColor(applicationState->videoContext, &COLOR_BLACK);
    BRS_clearVideo(applicationState->videoContext);
    BRS_Render3D_drawObject(applicationState->videoContext, applicationState->object3D,
                            applicationState->transformation3D);
//    BRS_Render2D_drawObject(applicationState->render2DContext, applicationState->object2D, applicationState->transformation2D);
    BRS_updateVideo(applicationState->videoContext);
}

static void processEvent(SDL_Event *event, ApplicationState *applicationState) {

}

static void rotateObject2D(ApplicationState *applicationState) {
    if (applicationState->transformation2D->rotateDegrees < 360) {
        applicationState->transformation2D->rotateDegrees++;
    } else {
        applicationState->transformation2D->rotateDegrees = 1;
    }
}

static void rotateObject3D(ApplicationState *applicationState) {
//    applicationState->transformation3D->rotateDegrees++;
//    if (applicationState->transformation3D->rotateDegrees >= 360) {
//        applicationState->transformation3D->rotateDegrees = 1;
//    }
    if (applicationState->transformation3D->rotateDegrees < 360) {
        applicationState->transformation3D->rotateDegrees++;
    } else {
        applicationState->transformation3D->rotateDegrees = 1;
    }
}

static void runApplication(ApplicationState *applicationState) {
    SDL_Event event;

    while (!applicationState->quit) {
        if (SDL_PollEvent(&event) != 0) {
            applicationState->quit = checkQuitApplication(&event);
            processEvent(&event, applicationState);
        }

//        rotateObject2D(applicationState);
        rotateObject3D(applicationState);

        handleVideo(applicationState);
    }
}

static void shutdownApplication(ApplicationState *applicationState) {
    BRS_shutdownVideo(applicationState->videoContext);
    destroyApplicationState(applicationState);
}

int main() {
    ApplicationConfig *config = createConfig();
    applicationState = initApplication(config);
    if (applicationState != NULL) {
        runApplication(applicationState);
    }
    shutdownApplication(applicationState);
    destroyConfig(config);
    return EXIT_SUCCESS;
}
