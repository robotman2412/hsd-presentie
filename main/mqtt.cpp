
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
void MQTTClient::eventHandler(esp_event_base_t base, int32_t event_id, void *event_data) {
	const char *TAG = "MQTT";
	esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
	esp_mqtt_client_handle_t client = event->client;
	int msg_id;
	
	std::string topic, data;
	switch ((esp_mqtt_event_id_t)event_id) {
		
		case MQTT_EVENT_CONNECTED:
			ESP_LOGI(TAG, "Connected");
			for (auto pair: subscriptions) {
				esp_mqtt_client_subscribe(handle, pair.first.c_str(), pair.second);
			}
			break;
			
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGI(TAG, "Disconnected");
			break;
			
		case MQTT_EVENT_DATA:
			topic = std::string(event->topic, event->topic_len);
			data  = std::string(event->data,  event->data_len);
			ESP_LOGI(TAG, "RX %s: %s", topic.c_str(), data.c_str());
			onData(topic, data);
			break;
			
		default:
			break;
	}
}

// Handles MQTT data events.
void MQTTClient::onData(std::string topic, std::string data) {
	for (auto cb: callbacks) cb(topic, data);
	for (auto cb: topicCallbacks[topic]) cb(topic, data);
}


// Starts an MQTT client over TCP.
MQTTClient::MQTTClient(std::string address, int port) {
	isConnected = false;
	defaultQos = 1;
	connect(address, port);
	clientID = "ESP32_MQTT";
}

// On destruct, the client will do a blocking close.
MQTTClient::~MQTTClient() {
	close();
}



// Close the client.
void MQTTClient::close() {
	esp_mqtt_client_disconnect(handle);
	esp_mqtt_client_destroy(handle);
	handle = NULL;
}

// Reconnect to a different server.
void MQTTClient::connect(std::string address, int port) {
	if (isConnected) close();
	
	this->port       = port;
	this->address    = address;
	// ESP-MQTT configuration.
	esp_mqtt_client_config_t config = {
		.host      = this->address.c_str(),
		.port      = (uint32_t) port,
	};
	
	handle = esp_mqtt_client_init(&config);
	if (!handle) abort();
	
	esp_mqtt_client_register_event(handle, (esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, mqtt_event_handler, this);
	esp_mqtt_client_start(handle);
}



// Publish to a certain topic.
void MQTTClient::publish(std::string topic, std::string message, int qos) {
	if (qos == -1) qos = defaultQos;
	
	if (!handle) return;
	ESP_LOGI("MQTT", "TX %s: %s", topic.c_str(), message.c_str());
	esp_mqtt_client_publish(handle, topic.c_str(), message.c_str(), 0, qos, 0);
}

// Subscribe to a certain topic.
void MQTTClient::subscribe(std::string topic, int qos) {
	if (qos == -1) qos = defaultQos;
	
	subscriptions.emplace(std::pair<std::string, int>(topic, qos));
	ESP_LOGI("MQTT", "Subscribing to %s", topic.c_str());
	if (!handle) return;
	esp_mqtt_client_subscribe(handle, topic.c_str(), qos);
}

// Subscribe to a certain with a topic-affine callback.
void MQTTClient::subscribe(std::string topic, Callback cb, int qos) {
	topicCallbacks[topic].push_back(cb);
	subscribe(topic, qos);
}



// Add a callback for all incoming messages.
void MQTTClient::addCallback(Callback cb) {
	callbacks.push_back(cb);
}

// Add a callback for messages on a specific topic.
void MQTTClient::addCallback(std::string topic, Callback cb) {
	topicCallbacks[topic].push_back(cb);
}

