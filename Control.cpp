/*
 * Project: Line Follwer Robot with Digital Sensors
 * File: Control.cpp
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
#include "EventBus.h"
#include "IoControl.h"

namespace {
    void StepAdvance() noexcept;

    const uint8_t* gPC = nullptr;

    void HoldStart() noexcept {
        SetEventHandler(SLOT_BUMPER_SENSOR,
                        [](int8_t) noexcept -> uint8_t {
                            SetEventHandler(SLOT_BUMPER_SENSOR, nullptr);
                            StepAdvance();
                            return 0;
                        });
    }

    namespace Driver {
        enum class Direction {
            Left,
            Right,
            Stop
        } gDirection;

        constexpr int8_t gcForwardTable[][2] = {
            {-10, 10}, {7, 10}, {10, 10}, {10, 7}, {10, -10}, {0, 0}
        };

        constexpr int8_t gcBackwardTable[][2] = {
            {10, -10}, {-10, -7}, {-10, -10}, {-7, -10}, {-10, 10}, {0, 0}
        };

        using Unit = int8_t[2];

        const Unit* gTableBase = nullptr;

        void SetDrive(const int8_t* const table) noexcept {
            SetLeftOutput(table[0]);
            SetRightOutput(table[1]);
        }

        void TrackReset(const EventHandler handler) noexcept {
            SetEventHandler(SLOT_LEFT_SENSOR, handler);
            SetEventHandler(SLOT_CENTER_SENSOR, handler);
            SetEventHandler(SLOT_RIGHT_SENSOR, handler);
        }

        void Stop() noexcept {
            TrackReset(nullptr);
            SetEventHandler(SLOT_BUMPER_SENSOR, nullptr);
            StepAdvance();
        }

        // this code can be reduced further, but kept as is for readability
        uint8_t Trace(const uint8_t status) noexcept {
            switch (status) {
            case MASK_LEFT_SENSOR: SetDrive(gTableBase[0]);
                break;
            case MASK_LEFT_SENSOR | MASK_CENTER_SENSOR: SetDrive(gTableBase[1]);
                break;
            case MASK_CENTER_SENSOR: SetDrive(gTableBase[2]);
                break;
            case MASK_RIGHT_SENSOR | MASK_CENTER_SENSOR: SetDrive(gTableBase[3]);
                break;
            case MASK_RIGHT_SENSOR: SetDrive(gTableBase[4]);
                break;
            default: ;
            }
            return 0b00000111;
        }

        uint8_t Terminal(int8_t) noexcept {
            auto status = GetStatus() & 0b111;
            const auto raw = status;
            switch (gDirection) {
            case Direction::Stop: SetDrive(gTableBase[5]);
                goto complete;
            case Direction::Left: status &= MASK_LEFT_SENSOR | MASK_CENTER_SENSOR;
                break;
            case Direction::Right: status &= MASK_RIGHT_SENSOR | MASK_CENTER_SENSOR;
                break;
            default: ;
            }
            if (raw == MASK_CENTER_SENSOR) goto complete;
            return Trace(status);
        complete:
            Stop();
            return 0b00000111;
        }

        uint8_t Normal(int8_t) noexcept {
            const auto status = GetStatus() & 0b111;
            return (status & 0b101) == 0b101 ? (TrackReset(&Terminal), Terminal(0)) : Trace(status);
        }

        void Start() noexcept {
            TrackReset(&Normal);
            Normal(0);
        }

        void AttachBumperStop() noexcept {
            SetEventHandler(SLOT_BUMPER_SENSOR, [](int8_t) noexcept-> uint8_t {
                SetDrive(gTableBase[5]);
                Stop();
                return 0;
            });
        }

        void ForwardStart(const Direction direction) noexcept {
            gDirection = direction;
            gTableBase = gcForwardTable;
            if (gDirection == Direction::Stop) { AttachBumperStop(); }
            Start();
        }

        void BackwardStart(const Direction direction) noexcept {
            gDirection = direction;
            gTableBase = gcBackwardTable;
            Start();
        }

        uint8_t SpinEnter(int8_t) noexcept {
            const auto status = GetStatus() & 0b111;
            if (status == MASK_CENTER_SENSOR) { Stop(); }
            return 0b00000111;
        }

        uint8_t SpinLeave(int8_t) noexcept {
            const auto status = GetStatus() & 0b111;
            if (status == 0) { TrackReset(&SpinEnter); }
            return 0b00000111;
        }

        void SpinStart() noexcept {
            TrackReset(&SpinLeave);
            SpinLeave(0);
        }

        void SpinLeft() noexcept {
            SetDrive(gcForwardTable[0]);
            SpinStart();
        }

        void SpinRight() noexcept {
            SetDrive(gcBackwardTable[0]);
            SpinStart();
        }
    }

    void Terminate() noexcept {
        gPC = nullptr;
        SetLeftOutput(0);
        SetRightOutput(0);
    }

    void DispatchCurrent() noexcept {
    start:
        switch (*gPC & OP_MAX_OP) {
        case OP_HOLD: HoldStart();
            return;
        case OP_DRIVE_LEFT: Driver::ForwardStart(Driver::Direction::Left);
            return;
        case OP_DRIVE_RIGHT: Driver::ForwardStart(Driver::Direction::Right);
            return;
        case OP_DRIVE_STOP: Driver::ForwardStart(Driver::Direction::Stop);
            return;
        case OP_REVERSE_LEFT: Driver::BackwardStart(Driver::Direction::Left);
            return;
        case OP_REVERSE_RIGHT: Driver::BackwardStart(Driver::Direction::Right);
            return;
        case OP_REVERSE_STOP: Driver::BackwardStart(Driver::Direction::Stop);
            return;
        case OP_JMP_FW(0): gPC += *gPC >> 4;
            goto start;
        case OP_JMP_BK(0): gPC -= *gPC >> 4;
            goto start;
        case OP_LEFT_360: Driver::SpinLeft();
            return;
        case OP_RIGHT_360: Driver::SpinRight();
            return;
        case OP_HALT: Terminate();
        default: ;
        }
    }

    void StepAdvance() noexcept {
        ++gPC, PublishEvents(0b00010000); Serial.println("Step");
    }
}

void InitializeControl(const uint8_t* const program) noexcept {
    gPC = program;
    SetEventHandler(4, [](int8_t) noexcept-> uint8_t {
        DispatchCurrent();
        return 0;
    });
    PublishEvents(1 << 4);
}
