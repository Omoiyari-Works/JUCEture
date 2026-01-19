#pragma once

#include "../GestureEvent.h"
#include "../CoordinateAccessor.h"
#include <JuceHeader.h>

class LongTapEvent : public GestureEvent
{
  public:
    LongTapEvent(const juce::Point<float>& localPointPt,
                 const juce::Point<float>& globalPointPt,
                 float rawGlobalX, float rawGlobalY);
    ~LongTapEvent();

    // Get local coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getLocal() const;

    // Get global coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getGlobal() const;

  private:
    juce::Point<float> localPt;
    juce::Point<float> globalPt;
    float rawGlobalX;
    float rawGlobalY;
};
