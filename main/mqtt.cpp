
#include <mqtt.hpp>

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"



// Redirects MQTT events into their class instances.
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
	((MQTTClient *) handler_args)->eventHandler(base, event_id, event_data);
}

// Handles MQTT events.
void eventHandler(esp_event_base_t base, int32_t event_id, void *event_data) {
	
}


// Starts an MQTT client over TCP.
MQTTClient::MQTTClient(std::string address, int port) {
	isConnected = false;
	connect(address, port);
}

// On destruct, the client will do a blocking close.
MQTTClient::~MQTTClient() {
	close();
}



// Close the client.
void MQTTClient::close() {
	
}

// Reconnect to a different server.
void MQTTClient::connect(std::string address, int port) {
	if (isConnected) close();
	
	// config = (esp_mqtt_client_config_t) {
	// 	.broker.address.uri = this->address.c_str(),
	// };
}



// Publish to a certain topic.
void MQTTClient::publish(std::string topic, std::string message, int qos) {
	
}

// Subscribe to a certain topic.
void MQTTClient::subscribe(std::string topic, int qos) {
	
}

// Subscribe to a certain with a topic-affine callback.
void MQTTClient::subscribe(std::string topic, Callback cb, int qos) {
	
}



// Add a callback for all incoming messages.
void MQTTClient::addCallback(Callback cb) {
	callbacks.push_back(cb);
}

// Add a callback for messages on a specific topic.
void MQTTClient::addCallback(std::string topic, Callback cb) {
	topicCallbacks[topic].push_back(cb);
}

