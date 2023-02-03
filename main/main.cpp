/*
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

// This file contains a simple Hello World app which you can base you own
// native Badge apps on.

#include "main.h"
#include "graphics.hpp"

#include "mqtt.hpp"

// Last card read time.
int64_t cardReadTime = 0;
// Last card read name.
std::string cardReadName = "Mr. Name";

pax_buf_t buf;
xQueueHandle buttonQueue;

#include <esp_log.h>
static const char *TAG = "mch2022-demo-app";

// Updates the screen with the latest buffer.
void disp_flush() {
	ili9341_write(get_ili9341(), buf.buf_8bpp);
}

// Exits the app, returning to the launcher.
void exit_to_launcher() {
	REG_WRITE(RTC_CNTL_STORE0_REG, 0);
	esp_restart();
}

extern "C" void app_main() {
	ESP_LOGI(TAG, "Welcome to the template app!");
	
	// Initialize the screen, the I2C and the SPI busses.
	bsp_init();
	
	// Initialize the RP2040 (responsible for buttons, etc).
	bsp_rp2040_init();
	
	// This queue is used to receive button presses.
	buttonQueue = get_rp2040()->queue;
	
	// Initialize graphics for the screen.
	pax_buf_init(&buf, NULL, 320, 240, PAX_BUF_16_565RGB);
	currentScreen = Screen::DISCONNECTED;
	updateGraphics();
	
	// Initialize NVS.
	nvs_flash_init();
	
	wifi_init();
	wifi_connect_to_stored();
	
	// Connect to MQTT?
	MQTTClient client("10.0.0.145", 1883);
	currentScreen = Screen::PRESENT_CARD;
	updateGraphics();
	
	while (1) {
		client.publish("test_topic", "test_message");
		client.subscribe("resp");
		
		// Wait for button presses and do another cycle.
		
		// Structure used to receive data.
		rp2040_input_message_t message;
		
		// Wait forever for a button press (because of portMAX_DELAY)
		xQueueReceive(buttonQueue, &message, portMAX_DELAY);
		
		// Which button is currently pressed?
		if ((message.input == RP2040_INPUT_BUTTON_HOME) && message.state) {
			// If home is pressed, exit to launcher.
			exit_to_launcher();
		} else if ((message.input = RP2040_INPUT_BUTTON_ACCEPT) && message.state) {
			// If A is pressed, simulate card read.
			cardReadTime = esp_timer_get_time();
			cardReadName = "deadbeef";
		}
	}
}
