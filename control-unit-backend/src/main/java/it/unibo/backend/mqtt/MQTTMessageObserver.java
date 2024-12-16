package it.unibo.backend.mqtt;

import io.vertx.core.json.JsonObject;

public interface MQTTMessageObserver {
    void onMessageReceived(String topic, JsonObject jsonPayload);
}