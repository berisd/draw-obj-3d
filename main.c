#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "video.h"
#include "color.h"

typedef struct ApplicationConfig {
    uint32_t screenWidth;
    uint32_t screenHeight;
} ApplicationConfig;

typedef struct _ApplicationState {
    BRS_VideoContext *videoContext;
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

static ApplicationState *initApplication(const ApplicationConfig *config) {
    ApplicationState *applicationState = createApplicationState();
    BRS_VideoContext *videoContext = BRS_initVideo(config->screenWidth, config->screenHeight);
    if (videoContext == NULL) {
        return NULL;
    }
    applicationState->videoContext = videoContext;
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

    BRS_updateVideo(applicationState->videoContext);
}

static void processEvent(SDL_Event *event, ApplicationState *applicationState) {

}

static void runApplication(ApplicationState *applicationState) {
    SDL_Event event;
    while (!applicationState->quit) {
        if (SDL_PollEvent(&event) != 0) {
            applicationState->quit = checkQuitApplication(&event);
            processEvent(&event, applicationState);
        }
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
