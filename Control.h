/*
 * Project: Line Follwer Robot with Digital Sensors
 * File: Control.h
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

#define OP_HOLD           0
#define OP_DRIVE_LEFT     1
#define OP_DRIVE_RIGHT    2
#define OP_DRIVE_STOP     3
#define OP_REVERSE_LEFT   4
#define OP_REVERSE_RIGHT  5
#define OP_REVERSE_STOP   6
#define OP_JMP_FW(x)      (7 | (x) << 4)
#define OP_JMP_BK(x)      (8 | (x) << 4)
#define OP_LEFT_360       9 
#define OP_RIGHT_360      10
#define OP_HALT           11
#define OP_MAX_OP         15

void InitializeControl(const uint8_t* program) noexcept;
