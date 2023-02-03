
#include "graphics.hpp"
#include "main.h"

// ==== Drawing helpers ==== ///

#define TITLE_FONT pax_font_saira_condensed
#define TITLE_FONT_SIZE (TITLE_FONT->default_size)

// Draw a title text in the center of the screen.
static void drawTitle(pax_col_t color, std::string str0) {
	const char *str = str0.c_str();
	
	pax_vec1_t dims = pax_text_size(TITLE_FONT, TITLE_FONT_SIZE, str);
	pax_draw_text(&buf, color, TITLE_FONT, TITLE_FONT_SIZE, (buf.width-dims.x)/2, (buf.height-dims.y)/2, str);
}



// ==== Screens ==== //

// Screen: Present card.
// Idle state of the system.
static void scrPresentCard() {
	pax_background(&buf, 0xff007fff);
	drawTitle(0xffffffff, "Present Card!");
}

// Screen: Disconnected.
// Connecting to WiFi and/or server.
static void scrDisconnected() {
	pax_background(&buf, 0xff7f7f7f);
	drawTitle(0xffffffff, "Connecting...");
}

// Screen: Processing.
// Reading data from card, talking to server.
static void scrProcessing() {
	pax_background(&buf, 0xff7f7f7f);
	drawTitle(0xff007fff, "Processing...");
}

// Screen: Card accepted.
// Card read successful; displays entry or leave status.
static void scrCardAccepted() {
	pax_background(&buf, 0xff00af00);
	
	char tmp[64];
	snprintf(tmp, sizeof(tmp)-1, "Welcome, %s!", cardReadName.c_str());
	pax_vec1_t dims = pax_text_size(TITLE_FONT, TITLE_FONT_SIZE, tmp);
	if (dims.x > buf.width) {
		snprintf(tmp, sizeof(tmp)-1, "Welcome,\n%s!", cardReadName.c_str());
	}
	
	drawTitle(0xffffffff, tmp);
}

// Screen: Processing.
// Card read unsuccessful; unknown card.
static void scrCardRejected() {
	pax_background(&buf, 0xffff0000);
	drawTitle(0xffffffff, "Unknown card :(");
}



// ==== API ==== //

Screen currentScreen = Screen::PRESENT_CARD;

// Mark that graphics should be updated.
void updateGraphics() {
	switch (currentScreen) {
		default: break;
		case Screen::DISCONNECTED:  scrDisconnected(); break;
		case Screen::PRESENT_CARD:  scrPresentCard();  break;
		case Screen::PROCESSING:    scrProcessing();   break;
		case Screen::CARD_ACCEPTED: scrCardAccepted(); break;
		case Screen::CARD_REJECTED: scrCardRejected(); break;
	}
	disp_flush();
}
