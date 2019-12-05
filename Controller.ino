/*
 * Project: Line Follwer Robot with Digital Sensors
 * File: Controller.ino
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
 
#include "Control.h"
#include "IoControl.h"
#include "EventBus.h"

static const uint8_t gProgram[] = {
	OP_HOLD, OP_DRIVE_RIGHT, OP_DRIVE_LEFT, OP_DRIVE_LEFT, OP_DRIVE_RIGHT, OP_DRIVE_STOP,
    OP_REVERSE_LEFT, OP_REVERSE_RIGHT, OP_REVERSE_STOP,
    OP_HALT
};

void setup() {
    Serial.begin(9600);
	InitializeIo();
	InitializeControl(gProgram);
}

void loop() {
	PollSensors();
	PollEvents();
}
