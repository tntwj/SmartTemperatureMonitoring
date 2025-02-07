#include <Arduino.h>
#include "MonitoringTemperature.h"
#include "controllers/LedController.h"
#include "controllers/TemperatureController.h"
#include "states/Idle.h"
#include "states/State.h"
#include "headers/defines.h"

void monitoringTask(void* parameter) {
    State* currentState = new Idle();
    currentState->execute();
    while(1) {
        delay(TASKS_PERIOD / frequency);
        State* nextState = currentState->next();
        if (nextState != nullptr) {
            currentState = nextState;
            currentState->execute();
        }
    }
}