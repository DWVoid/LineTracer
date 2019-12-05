/*
 * Project: Line Follwer Robot with Digital Sensors
 * File: EventBus.cpp
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

#include "EventBus.h"

namespace {
	EventHandler gHandlers[8] = { nullptr };
    uint8_t gEventStatus;
}

void PublishEvents(const uint8_t mask) noexcept { gEventStatus |= mask; }

void PollEvents() noexcept {
	for (int8_t i = 0; i < 8; ++i) {
		if (gHandlers[i] && (gEventStatus & (1 << i))) {
			gEventStatus &= ~(gHandlers[i](i));
		}
	}
	gEventStatus = 0u;
}

EventHandler SetEventHandler(const int8_t id, const EventHandler newHandler) noexcept {
	const auto old = gHandlers[id];
	gHandlers[id] = newHandler;
	return old;
}
