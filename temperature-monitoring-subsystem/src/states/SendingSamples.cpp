#include "SendingSamples.h"
#include "ProblemDetected.h"
#include "Idle.h"


SendingSamples::SendingSamples() {
}

void SendingSamples::execute() {
    Serial.println("Sending State");
    String jsonMessage = "{\"temperature\":" + String(tempController->getTemp()) + "}";
    client.publish(topic_samples, jsonMessage.c_str());
}

State* SendingSamples::next() {
    if (!isNetworkConnected) {
        return new ProblemDetected();
    }
    return new Idle();
}
