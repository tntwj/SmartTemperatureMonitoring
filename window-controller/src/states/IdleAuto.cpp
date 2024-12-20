#include "IdleAuto.h"
#include "ActiveAuto.h"
#include "IdleManual.h"
#include "Context.h"
#include "Config.h"
#include "controllers/LcdController.h"

extern Context* context;
extern LcdController* lcdController;

IdleAuto::IdleAuto() {
    this->startTime = millis();
}

void IdleAuto::handle() {
    lcdController->printInfo(context->getLevel(), "AUTO", context->getTemperature());
}

State* IdleAuto::next() {
    if ((millis() - this->startTime) > IDLE_TIME) {
        return new ActiveAuto();
    } else if (context->getOperatingMode() == OperatingMode::MANUAL) {
        return new IdleManual();
    } else {
        return this;
    }
}
