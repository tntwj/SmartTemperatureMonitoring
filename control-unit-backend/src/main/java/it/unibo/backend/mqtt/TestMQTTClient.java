package it.unibo.backend.mqtt;

import io.vertx.core.json.JsonObject;

/**
 * Unrelated to the backend. Just a MQTT test class.
 */
public class TestMQTTClient {

    public static void main(String[] args) throws InterruptedException {
        MQTTClient client = new MQTTClient("34.154.239.184", 1883);
        client.start();
        Thread.sleep(2000);
        JsonObject data = new JsonObject();
        data.put("welcomeMessage", "ciao");
        client.publish("esiot", data);
    }
}
