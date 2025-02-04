#include "WindowController.h"
#include "config.h"

WindowController::WindowController(int pin) {
    this->servo = new ServoTimer2();
    this->servo->attach(pin);
    this->servo->write(WINDOW_CLOSED_WIDTH);
}

WindowController::~WindowController() {
    delete this->servo;
}

void WindowController::open() {
    this->servo->write(WINDOW_OPEN_WIDTH);
}

void WindowController::close() {
    this->servo->write(WINDOW_CLOSED_WIDTH);
}

void WindowController::setLevel(float level) {
    level = constrain(level, WINDOW_MIN_LEVEL, WINDOW_MAX_LEVEL);
    int mappedValue = WINDOW_CLOSED_WIDTH + (level - WINDOW_MIN_LEVEL) * 
                      (WINDOW_OPEN_WIDTH - WINDOW_CLOSED_WIDTH) / 
                      (WINDOW_MAX_LEVEL - WINDOW_MIN_LEVEL);
    this->servo->write(mappedValue);
}
