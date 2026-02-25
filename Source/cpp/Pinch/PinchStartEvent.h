#pragma once

#include "../GestureEvent.h"
#include "../CoordinateAccessor.h"
#include <JuceHeader.h>

class PinchStartEvent : public GestureEvent
{
  public:
    PinchStartEvent(const juce::Point<float>& focusLocalPt,
                    const juce::Point<float>& focusGlobalPt,
                    float rawFocusGlobalX, float rawFocusGlobalY);
    ~PinchStartEvent();

    // Get focus local coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getFocusLocal() const;

    // Get focus global coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getFocusGlobal() const;

  private:
    juce::Point<float> focusLocalPt;
    juce::Point<float> focusGlobalPt;
    float rawFocusGlobalX;
    float rawFocusGlobalY;
};
