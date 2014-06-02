#define WS2811_PORT PORTB

#include <avr/interrupt.h>
#include "lib/ws2811/ws2811_8.h"
//#include "lib/rtc/ds1307/ds1307.h"
#include "lib/serial/serial.h"
#include "lib/rtc/pcf8563/pcf8563.h"
#include "lib/remote/remote.h"
#include "time32/time.h"
#include "time32/usa_dst.h"
#include <util/delay.h>

#define MODE_HMS 0
#define MODE_MD 1
#define MODE_WD 2
#define MODE_PCT 3
#define MODE_MOON 4
#define MODE_SPECTRUM 5

#define MODE_YEAR 127
#define MODE_MONTH 128
#define MODE_DAY 129
#define MODE_HOUR 130
#define MODE_MIN 131
#define MODE_SEC 132

extern time_t __system_time;

int main() {
	
	DDRB |= _BV(1); // led strip output
	
	ws2811_t black = {.red = 0x00, .green = 0x00, .blue = 0x00 };
	ws2811_t red = {.red = 0xff, .green = 0x00, .blue = 0x00 };
	ws2811_t orange = {.red = 0xdd, .green = 0x33, .blue = 0x00 };
	ws2811_t yellow = {.red = 0xff, .green = 0xff, .blue = 0x00 };
	ws2811_t chartreuse = {.red = 0x33, .green = 0xdd, .blue = 0x00 };
	ws2811_t green = {.red = 0x00, .green = 0xff, .blue = 0x00 };
	ws2811_t spring = {.red = 0x00, .green = 0xdd, .blue = 0x33 };
	ws2811_t cyan = {.red = 0x00, .green = 0xff, .blue = 0xff };
	ws2811_t azure = {.red = 0x00, .green = 0x33, .blue = 0xdd };
	ws2811_t blue = {.red = 0x00, .green = 0x00, .blue = 0xff };
	ws2811_t violet = {.red = 0x33, .green = 0x00, .blue = 0xdd };
	ws2811_t magenta = {.red = 0xff, .green = 0x00, .blue = 0xff };
	ws2811_t rose = {.red = 0xdd, .green = 0x00, .blue = 0x33 };
	ws2811_t grey = {.red = 0xdd, .green = 0xdd, .blue = 0xdd };
	ws2811_t white = {.red = 0xff, .green = 0xff, .blue = 0xff };

	
	struct ws2811_t colors[60];
	remote_init();
	
	EICRA |= _BV(ISC11);	// trigger on falling edge of int1
	EIMSK |= _BV(INT1);		// enable external interrupts on int1
	
	DDRD &= ~_BV(PD3);		// set 1 Hz int1 as input
	PORTD |= _BV(PD3);		// set 1 Hz int1 as pull-up
	
	pcf8563_init();
	serial_init_b(9600);
	
	sei();

	int8_t mday = -1;		// used to avoid computing sunrise, sunset, and moon phase more than once per day;
	volatile time_t systime;
	time_t risetime;
	time_t settime;
	struct tm sys;
	struct tm rise;
	struct tm set;
	int8_t phase = 0;	// the number of segments to light / 2
	uint8_t year = 0;
	uint8_t week = 0;
	uint8_t mode = 0;
	uint8_t update = 1;
	struct pcf8563_t rtc;

	// hard coded location and time zone for now
	set_zone(-7 * ONE_HOUR);
	set_dst(usa_dst); // mountain
	set_position(183762, -410606); // calgary

	pcf8563_get(&rtc);
	sys.tm_year = rtc.year;
	sys.tm_mon = rtc.month;
	sys.tm_mday = rtc.mday;
	sys.tm_hour = rtc.hour;
	sys.tm_min = rtc.minute;
	systime = mktime(&sys);
	set_system_time(systime);
	
	struct ws2811_t markers = grey;
	struct ws2811_t fill = violet;
	struct ws2811_t pixel = blue;
	
	while (1) {
		// update display
		// * if the update flag is set
		// * if the systime has changed and the mode is time based
		// * the remote isn't in the middle of receiving a message
		if (remote_state() == 0 && (update == 1 || (mode <= MODE_MOON && __system_time > systime))) {
			update = 0;
			systime = __system_time;
			UDR0 = 'X';
			if (mode <= MODE_MOON ) {
				// recompute time structure
				localtime_r(&__system_time, &sys);

				if (sys.tm_mday != mday) {
					// recompute sunrise, sunset, moon phase
					mday = sys.tm_mday;
					risetime = sun_rise(&systime);
					settime = sun_set(&systime);
					localtime_r(&risetime, &rise);
					localtime_r(&settime, &set);
					week = week_of_year(&sys, 0);
					int8_t mp = moon_phase(&systime);
					phase = 15; //0.3 * mp;
				}
			}
			
			if (systime < risetime || systime > settime) {
				// invert display after sunset
				markers = magenta;
			} else {
				markers = grey;
			}
			
			for (uint8_t i = 0; i < 60; i++) {
				colors[i] = black;
			}
			
			if (mode == MODE_HMS) {
				// hours, minutes, seconds
				// markers
				for (uint8_t i = 0; i < 60; i = i + 5) {
					colors[i] = markers;
				}
				// hour fill
				uint8_t hour = (sys.tm_hour % 12) * 5;
				for (uint8_t i = hour + 1; i < hour + 5; i++) {
					colors[i] = fill;
				}
				colors[sys.tm_min] = pixel;
				colors[sys.tm_sec] = red;
			} else if (mode == MODE_MD) {
				// month, day of month
				if (sys.tm_mon < 2) fill = blue; // jan, feb
				else if (sys.tm_mon < 5) fill = green; // mar, apr, may
				else if (sys.tm_mon < 8) fill = yellow; // jun, jul, aug
				else if (sys.tm_mon < 11) fill = orange; // sep, oct, nov
				else fill = blue; // dec
				
				// markers
				for (uint8_t i = 0; i < 60; i = i + 5) {
					colors[i] = markers;
				}
				// month fill
				uint8_t mon = (sys.tm_mon % 12) * 5;
				for (uint8_t i = mon + 1; i < mon + 5; i++) {
					colors[i] = fill;
				}
				// day fill
				if (sys.tm_mday < 31) {
					uint8_t i = sys.tm_mday * 2;
					colors[i] = pixel;
					colors[i+1] = pixel;
				} else {
					colors[58] = pixel;
					colors[59] = pixel;
					colors[0] = pixel;
					colors[1] = pixel;
				}
			} else if (mode == MODE_WD) {
				// week, day of week
				// markers
				for (uint8_t i = 0; i < 60; i = i + 8) {
					colors[i] = markers;
				}
				// week fill
				uint8_t wday = sys.tm_wday * 8;
				for (uint8_t i = wday + 1; i < wday + 7; i++) {
					colors[i] = fill;
				}
				colors[week] = pixel;
			} else if (mode == MODE_PCT) {
				// persoixante of day
				uint8_t ps = ((sys.tm_hour * 60) + sys.tm_min) / 24;
				uint8_t sr = ((rise.tm_hour * 60) + rise.tm_min) / 24;
				uint8_t ss = ((set.tm_hour * 60) + set.tm_min) / 24;
				for (uint8_t i = 0; i < ps; i ++) {
					colors[i] = fill;
				}
				colors[sr] = yellow;
				colors[ss] = magenta;
			} else if (mode == MODE_MOON) {
				// moon phase
				if (phase > 0) {
					// waxing
					for (int8_t i = 15; i < 15 + phase; i++) {
						colors[i] = grey;
					}
					for (int8_t i = 15; i > 15 - phase; i--) {
						if (i < 0) i = 60 - i;
						colors[i] = grey;
					}
				} else {
					// waning
					for (int8_t i = 45; i < 45 + phase; i++) {
						colors[i] = grey;
					}
					for (int8_t i = 45; i > 45 - phase; i--) {
						if (i > 60) i = i - 60;
						colors[i] = grey;
					}
				}
			} else if (mode == MODE_SPECTRUM) {
				for (uint8_t i = 0; i < 60; i++) {
					if (i < 5) colors[i] = red;
					else if (i < 10) colors[i] = orange;
					else if (i < 15) colors[i] = yellow;
					else if (i < 20) colors[i] = chartreuse;
					else if (i < 25) colors[i] = green;
					else if (i < 30) colors[i] = spring;
					else if (i < 35) colors[i] = cyan;
					else if (i < 40) colors[i] = azure;
					else if (i < 45) colors[i] = blue;
					else if (i < 50) colors[i] = violet;
					else if (i < 55) colors[i] = magenta;
					else if (i < 60) colors[i] = rose;
				}
			} else if (mode == MODE_YEAR) {
				colors[year] = yellow;
			} else if (mode == MODE_MONTH) {
				for (uint8_t i = 0; i < 60; i = i + 5) {
					colors[i] = markers;
				}
				uint8_t mon = (sys.tm_mon % 12) * 5;
				for (uint8_t i = mon + 1; i < mon + 5; i++) {
					colors[i] = green;
				}
			} else if (mode == MODE_DAY) {
				if (sys.tm_mday < 31) {
					uint8_t i = sys.tm_mday * 2;
					colors[i] = cyan;
					colors[i+1] = cyan;
				} else {
					colors[58] = cyan;
					colors[59] = cyan;
					colors[0] = cyan;
					colors[1] = cyan;
				}
			} else if (mode == MODE_HOUR) {
				for (uint8_t i = 0; i < 60; i = i + 5) {
					colors[i] = markers;
				}uint8_t hour = (sys.tm_hour % 12) * 5;
				for (uint8_t i = hour + 1; i < hour + 5; i++) {
					colors[i] = blue;
				}
			} else if (mode == MODE_MIN) {
				colors[sys.tm_min] = magenta;
			} else if (mode == MODE_SEC) {
				colors[sys.tm_sec] = red;
			}

			// TODO translate the array
			ws2811_set(colors, 60, 1);
		}

		// 	// read ir
		uint8_t command = remote_get();
		if (command > 0) {
			UDR0 = 'C';
			update = 1;
		}
		
		if (mode <= MODE_SPECTRUM) {
			if (command == REMOTE_LEFT) {
				if (mode > MODE_HMS) mode--;
			} else if (command == REMOTE_RIGHT) {
				if (mode < MODE_SPECTRUM) mode++;
			} else if (command == REMOTE_MENU) {
				mode = MODE_YEAR;
				year = 2000 - sys.tm_year;
				if (year > 59) year = 0;
			}
		} else {
			if (command == REMOTE_UP) {
				UDR0 = 'U';
				if (mode == MODE_YEAR && year < 59) year++;
				else if (mode == MODE_YEAR) year = 0;
				else if (mode == MODE_MONTH && sys.tm_mon < 11) sys.tm_mon++;
				else if (mode == MODE_MONTH) sys.tm_mon = 0;
				else if (mode == MODE_DAY && sys.tm_mday < 31) sys.tm_mday++;
				else if (mode == MODE_DAY) sys.tm_mday = 1;
				else if (mode == MODE_HOUR && sys.tm_hour < 23) sys.tm_hour++;
				else if (mode == MODE_HOUR) sys.tm_hour = 0;
				else if (mode == MODE_MIN && sys.tm_min < 59) sys.tm_min++;
				else if (mode == MODE_MIN) sys.tm_min = 0;
				else if (mode == MODE_SEC && sys.tm_sec < 59) sys.tm_sec++;
				else if (mode == MODE_SEC) sys.tm_sec = 0;
			} else if (command == REMOTE_DOWN) {
				UDR0 = 'D';
				if (mode == MODE_YEAR && year > 0) year--;
				else if (mode == MODE_YEAR) year = 59;
				else if (mode == MODE_MONTH && sys.tm_mon > 0) sys.tm_mon--;
				else if (mode == MODE_MONTH) sys.tm_mon = 11;
				else if (mode == MODE_DAY && sys.tm_mday > 0) sys.tm_mday--;
				else if (mode == MODE_DAY) sys.tm_mday = 31;
				else if (mode == MODE_HOUR && sys.tm_hour > 0) sys.tm_hour--;
				else if (mode == MODE_HOUR) sys.tm_hour = 23;
				else if (mode == MODE_MIN && sys.tm_min > 0) sys.tm_min--;
				else if (mode == MODE_MIN) sys.tm_min = 59;
				else if (mode == MODE_SEC && sys.tm_sec > 0) sys.tm_sec--;
				else if (mode == MODE_SEC) sys.tm_sec = 59;
			} else if (command == REMOTE_LEFT) {
				UDR0 = 'L';
				if (mode > MODE_YEAR) mode--;
			} else if (command == REMOTE_RIGHT) {
				UDR0 = 'R';
				if (mode < MODE_SEC) mode++;
				else {
					systime = mktime(&sys);
					set_system_time(systime);
					rtc.year = 2000 + year;
					rtc.month = sys.tm_mon;
					rtc.mday = sys.tm_mday;
					rtc.hour = sys.tm_hour;
					rtc.minute = sys.tm_min;
					rtc.second = sys.tm_sec;
					pcf8563_set(&rtc);
					mode = MODE_HMS;
				}
			} else if (command == REMOTE_MENU) {
				UDR0 = 'M';
				mode = MODE_HMS;
			}
		}
	}
}

ISR(INT1_vect, ISR_NAKED) {
	system_tick();
	reti();
}
