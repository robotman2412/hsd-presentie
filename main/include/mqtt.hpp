
#pragma once

#include <functional>
#include <string>
#include <vector>
#include <map>

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

class MQTTClient {
	protected:
		// Current connection status.
		bool isConnected;
		
		// Internal copy of broker address.
		std::string address;
		// Internal copy of broker port.
		int port;
		// ESP-MQTT configuration.
		esp_mqtt_client_config_t config;
		// ESP-MQTT client handle.
		esp_mqtt_client_handle_t handle;
		
		// Redirects MQTT events into their class instances.
		friend void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
		
		// Handles MQTT events.
		void eventHandler(esp_event_base_t base, int32_t event_id, void *event_data);
		
	public:
		// Message callback type.
		using Callback = std::function<void(std::string topic, std::string msg)>;
		
		// When QOS -1 is specified, this value is used instead.
		int defaultQos;
		// List of callbacks that respond to every topic.
		std::vector<Callback> callbacks;
		// Map of topic-specific callbacks.
		std::map<std::string, std::vector<Callback>> topicCallbacks;
		
		// Starts an MQTT client over TCP.
		MQTTClient(std::string address="127.0.0.1", int port=1883);
		// Disallow copy initalise.
		MQTTClient(MQTTClient &) = delete;
		MQTTClient(MQTTClient &&) = delete;
		MQTTClient& operator=(const MQTTClient&) = delete;
		// On destruct, the client will do a blocking close.
		~MQTTClient();
		
		// Close the client.
		void close();
		// Reconnect to a different server.
		void connect(std::string address="127.0.0.1", int port=1883);
		
		// Publish to a certain topic.
		void publish(std::string topic, std::string message, int qos=-1);
		// Subscribe to a certain topic.
		void subscribe(std::string topic, int qos=-1);
		// Subscribe to a certain with a topic-affine callback.
		void subscribe(std::string topic, Callback cb, int qos=-1);
		
		// Add a callback for all incoming messages.
		void addCallback(Callback cb);
		// Add a callback for messages on a specific topic.
		void addCallback(std::string topic, Callback cb);
};
