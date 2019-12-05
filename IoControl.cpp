/*
 * Project: Line Follwer Robot with Digital Sensors
 * File: IoControl.cpp
 * Copyright 2019, LIU, Yushun (aka DWVoid)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "IoControl.h"
#include "EventBus.h"

void InitializeIo() noexcept {
	struct Init {
		void InitSensors() noexcept {
			pinMode(PIN_LEFT_SENSOR, INPUT);
			pinMode(PIN_CENTER_SENSOR, INPUT);
			pinMode(PIN_RIGHT_SENSOR, INPUT);
			pinMode(PIN_BUMPER_SENSOR, INPUT);
		}

		void InitLeft() noexcept {
			pinMode(PIN_L_DIR, OUTPUT);
			pinMode(PIN_LQ3, OUTPUT);
			pinMode(PIN_LQ2, OUTPUT);
			pinMode(PIN_LQ1, OUTPUT);
			pinMode(PIN_LQ0, OUTPUT);
			SetLeftOutput(0);
		}

		void InitRight() noexcept {
			pinMode(PIN_R_DIR, OUTPUT);
			pinMode(PIN_RQ3, OUTPUT);
			pinMode(PIN_RQ2, OUTPUT);
			pinMode(PIN_RQ1, OUTPUT);
			pinMode(PIN_RQ0, OUTPUT);
			SetRightOutput(0);
		}

		void InitThis() noexcept {
			InitSensors();
			InitLeft();
			InitRight();
		}
	};

	Init().InitThis();
}

namespace{ uint8_t gLastFrame = 0b0000; }

void PollSensors() noexcept {
	const uint8_t thisFrame =
		(digitalRead(PIN_LEFT_SENSOR) 
			| digitalRead(PIN_CENTER_SENSOR) << 1
			| digitalRead(PIN_RIGHT_SENSOR) << 2 
			| digitalRead(PIN_BUMPER_SENSOR) << 3);
	const uint8_t diff = gLastFrame ^ thisFrame;
	gLastFrame = thisFrame;
	PublishEvents(diff);
}

uint8_t GetStatus() noexcept { return gLastFrame; }

void SetLeftOutput(int8_t level) noexcept {
	digitalWrite(PIN_L_DIR, level > 0 ? HIGH : (level = -level, LOW));
	digitalWrite(PIN_LQ3, (level & 8) ? HIGH : LOW);
	digitalWrite(PIN_LQ2, (level & 4) ? HIGH : LOW);
	digitalWrite(PIN_LQ1, (level & 2) ? HIGH : LOW);
	digitalWrite(PIN_LQ0, (level & 1) ? HIGH : LOW);
}

void SetRightOutput(int8_t level) noexcept {
	digitalWrite(PIN_R_DIR, level > 0 ? HIGH : (level = -level, LOW));
	digitalWrite(PIN_RQ3, (level & 8) ? HIGH : LOW);
	digitalWrite(PIN_RQ2, (level & 4) ? HIGH : LOW);
	digitalWrite(PIN_RQ1, (level & 2) ? HIGH : LOW);
	digitalWrite(PIN_RQ0, (level & 1) ? HIGH : LOW);
}
