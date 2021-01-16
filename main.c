#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "video.h"
#include "color.h"
#include "render3d.h"

typedef struct ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} ApplicationConfig;

typedef struct _ApplicationState {
    BRS_VideoContext *videoContext;
    BRS_Object3D *object3D;
    BRS_Transformation3D *transformation3D;
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
    free(applicationState);
}

static BRS_Object3D *createCube3D() {
    BRS_Object3D *obj3d = malloc(sizeof(BRS_Object3D));

    BRS_Polygon *frontFace = malloc(sizeof(BRS_Polygon));
    frontFace->numVertices = 4;
    frontFace->vertices[0] = BRS_Render3D_createVertex(0, 0, 10);
    frontFace->vertices[1] = BRS_Render3D_createVertex(10, 0, 10);
    frontFace->vertices[2] = BRS_Render3D_createVertex(10, 10, 10);
    frontFace->vertices[3] = BRS_Render3D_createVertex(0, 10, 10);

    BRS_Polygon *backFace = malloc(sizeof(BRS_Polygon));
    backFace->numVertices = 4;
    backFace->vertices[0] = BRS_Render3D_createVertex(0, 0, 0);
    backFace->vertices[1] = BRS_Render3D_createVertex(10, 0, 0);
    backFace->vertices[2] = BRS_Render3D_createVertex(10, 10, 0);
    backFace->vertices[3] = BRS_Render3D_createVertex(0, 10, 0);

    BRS_Polygon *bottomFace = malloc(sizeof(BRS_Polygon));
    bottomFace->numVertices = 4;
    bottomFace->vertices[0] = BRS_Render3D_createVertex(0, 0, 10);
    bottomFace->vertices[1] = BRS_Render3D_createVertex(10, 0, 10);
    bottomFace->vertices[2] = BRS_Render3D_createVertex(10, 0, 0);
    bottomFace->vertices[3] = BRS_Render3D_createVertex(0, 0, 0);

    BRS_Polygon *topFace = malloc(sizeof(BRS_Polygon));
    topFace->numVertices = 4;
    topFace->vertices[0] = BRS_Render3D_createVertex(0, 10, 10);
    topFace->vertices[1] = BRS_Render3D_createVertex(10, 10, 10);
    topFace->vertices[2] = BRS_Render3D_createVertex(10, 10, 0);
    topFace->vertices[3] = BRS_Render3D_createVertex(0, 10, 0);

    BRS_Polygon *leftFace = malloc(sizeof(BRS_Polygon));
    leftFace->numVertices = 4;
    leftFace->vertices[0] = BRS_Render3D_createVertex(0, 0, 10);
    leftFace->vertices[1] = BRS_Render3D_createVertex(0, 0, 0);
    leftFace->vertices[2] = BRS_Render3D_createVertex(0, 10, 0);
    leftFace->vertices[3] = BRS_Render3D_createVertex(0, 10, 10);

    BRS_Polygon *rightFace = malloc(sizeof(BRS_Polygon));
    rightFace->numVertices = 4;
    rightFace->vertices[0] = BRS_Render3D_createVertex(10, 0, 10);
    rightFace->vertices[1] = BRS_Render3D_createVertex(10, 0, 0);
    rightFace->vertices[2] = BRS_Render3D_createVertex(10, 10, 0);
    rightFace->vertices[3] = BRS_Render3D_createVertex(10, 10, 10);

    obj3d->numPolygons = 6;
    obj3d->polygons[0] = frontFace;
    obj3d->polygons[1] = backFace;
    obj3d->polygons[2] = topFace;
    obj3d->polygons[3] = bottomFace;
    obj3d->polygons[4] = leftFace;
    obj3d->polygons[5] = rightFace;

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
    applicationState->videoContext = videoContext;
    applicationState->object3D = createCube3D();
    applicationState->transformation3D = BRS_Render3D_createTransformation();
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
    BRS_updateVideo(applicationState->videoContext);
}

static void processEvent(SDL_Event *event, ApplicationState *applicationState) {

}

static void rotateObject(ApplicationState *applicationState) {
    applicationState->transformation3D->rotateDegrees++;
    if (applicationState->transformation3D->rotateDegrees >= 360) {
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

        rotateObject(applicationState);

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
