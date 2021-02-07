#pragma once

#include <vector>

typedef enum
{
    KEYPRESS_UP,
    KEYPRESS_DOWN,
    KEYPRESS_LEFT,
    KEYPRESS_RIGHT,
    KEYPRESS_SPACE,
    KEYPRESS_RESET,
    KEYPRESS_SELECT
} keypress_event_t;

class EmulatorCore
{
public:
    virtual ~EmulatorCore() = default;

    virtual bool LoadROM(const char *szFilePath) = 0;
    virtual void RunToVBlank(std::vector<uint8_t>& framebuffer, int16_t *pSampleBuffer, int *pSampleCount) = 0;
    virtual void Reset() = 0;
    virtual void KeypressEvent(keypress_event_t evt, bool pressed) = 0;
};