/*
 * Project: Line Follwer Robot with Digital Sensors
 * File: IoControl.h
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

#pragma once 

#include "Environment.h"

#define PIN_LQ3 3              //pin D3
#define PIN_LQ2 4              //pin D4
#define PIN_LQ1 5              //pin D5
#define PIN_LQ0 6              //pin D6

#define PIN_RQ3 7              //pin D7
#define PIN_RQ2 8              //pin D8
#define PIN_RQ1 9              //pin D9
#define PIN_RQ0 10             //pin D10

#define PIN_L_DIR 11            //pin D11
#define PIN_R_DIR 12            //pin D12

#define PIN_LEFT_SENSOR   A0
#define PIN_CENTER_SENSOR 13
#define PIN_RIGHT_SENSOR  A3
#define PIN_BUMPER_SENSOR A4

#define SLOT_LEFT_SENSOR   0
#define SLOT_CENTER_SENSOR 1
#define SLOT_RIGHT_SENSOR  2
#define SLOT_BUMPER_SENSOR 3

#define MASK_LEFT_SENSOR 1 << SLOT_LEFT_SENSOR
#define MASK_CENTER_SENSOR 1 << SLOT_CENTER_SENSOR
#define MASK_RIGHT_SENSOR 1 << SLOT_RIGHT_SENSOR
#define MASK_BUMPER_SENSOR 1 << SLOT_BUMPER_SENSOR

void InitializeIo() noexcept;

void PollSensors() noexcept;

uint8_t GetStatus() noexcept;

void SetLeftOutput(int8_t level) noexcept;

void SetRightOutput(int8_t level) noexcept;
