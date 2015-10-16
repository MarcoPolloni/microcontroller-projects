#include "ClockSet.h"
#include <Buttons.h>
#include <Hsv.h>
#include <Rgb.h>
#include "Matrix.h"
#include <util/delay.h>
#include <stdlib.h>

using namespace digitalcave;

extern Hsv hsv;
extern Matrix matrix;
extern Buttons buttons;
extern uint8_t sample;

ClockSet::ClockSet() {
}

ClockSet::~ClockSet() {
}

void ClockSet::run() {
	mcp79410_time_t time;
	uint8_t running = 1;
	uint8_t a;
	uint8_t b;
	
	uint8_t released;
	uint8_t held = 0;
	uint8_t repeat;

	while (running) {
		mcp79410_set(&time);
		matrix.setColor(0,0,0);
		matrix.rectangle(0,0,11,11,DRAW_FILLED);
		matrix.setColor(Rgb(hsv));
		
		if (running == 1) {
			a = time.year / 10;
			b = time.year - (10 * a);
			
			matrix.text(0,0,"YR",0);
			matrix.character(0,6,0x30+a,0);
			matrix.character(6,6,0x30+b,0);
		}
		else if (running == 2) {
			a = time.month / 10;
			b = time.month - (10 * a);
			
			matrix.text(0,0,"MO",0);
			matrix.character(0,6,0x30+a,0);
			matrix.character(6,6,0x30+b,0);
		}
		else if (running == 3) {
			a = time.mday / 10;
			b = time.mday - (10 * a);
			
			matrix.text(0,0,"DY",0);
			matrix.character(0,6,0x30+a,0);
			matrix.character(6,6,0x30+b,0);
		}
		else if (running == 4) {
			a = time.hour / 10;
			b = time.hour - (10 * a);
			
			matrix.text(0,0,"HR",0);
			matrix.character(0,6,0x30+a,0);
			matrix.character(6,6,0x30+b,0);
		}
		if (running == 5) {
			a = time.minute / 10;
			b = time.minute - (10 * a);
			
			matrix.text(0,0,"MI",0);
			matrix.character(0,6,0x30+a,0);
			matrix.character(6,6,0x30+b,0);
		}

		matrix.flush();
		
		if (sample) {
			buttons.sample();
			released = buttons.released();
			held = buttons.held();
			repeat = buttons.repeat();

			sample = 0;
		}
		
		if (held && 0x01) {
			// exit
			time.second = 0;
			mcp79410_set(&time);
			running = 0;
		}
		if (released && 0x01) {
			// change field
			running++;
			
			if (running == 6) {
				time.second = 0;
				mcp79410_set(&time);
				running = 0;
			}
		}
		else if ((released && 0x02) || (repeat && 0x02)) {
			// increment field value
			if (running == 1) {
				time.year++;
				time.year %= 100;
			}
			else if (running == 2) {
				time.month++;
				time.month %= 12;
			}
			else if (running == 3) {
				time.mday++;
				time.mday %= 31;
			}
			else if (running == 4) {
				time.hour++;
				time.hour %= 24;
			}
			else if (running == 5) {
				time.minute++;
				time.minute %= 60;
			}
		}
		
		_delay_ms(100);
	}
}