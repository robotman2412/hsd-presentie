
#pragma once

#include <stdlib.h>
#include <pax_gfx.h>

enum Screen {
	// Blank screen, backlight off.
	BLANK,
	// Disconnected from the internets.
	DISCONNECTED,
	// "Present card!" screen.
	PRESENT_CARD,
	// Processing card and/or talking to server.
	PROCESSING,
	// Card read successful.
	CARD_ACCEPTED,
	// Card read unseccessful.
	CARD_REJECTED
};

extern Screen currentScreen;

// Mark that graphics should be updated.
void updateGraphics();
