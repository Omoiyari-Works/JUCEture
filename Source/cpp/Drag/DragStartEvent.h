#pragma once

#include "../GestureEvent.h"
#include "../CoordinateAccessor.h"
#include <JuceHeader.h>

class DragStartEvent : public GestureEvent
{
  public:
    DragStartEvent(const juce::Point<float>& startLocalPt,
                   const juce::Point<float>& currentLocalPt,
                   const juce::Point<float>& deltaFromLastPt,
                   const juce::Point<float>& startGlobalPt,
                   const juce::Point<float>& currentGlobalPt,
                   float rawStartGlobalX, float rawStartGlobalY,
                   float rawCurrentGlobalX, float rawCurrentGlobalY);
    ~DragStartEvent();

    // Get start local coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getStartLocal() const;

    // Get current local coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getCurrentLocal() const;

    // Get delta coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getDelta() const;

    // Get start global coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getStartGlobal() const;

    // Get current global coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getCurrentGlobal() const;

  private:
    juce::Point<float> startLocalPt;
    juce::Point<float> currentLocalPt;
    juce::Point<float> deltaFromLastPt;
    juce::Point<float> startGlobalPt;
    juce::Point<float> currentGlobalPt;
    float rawStartGlobalX;
    float rawStartGlobalY;
    float rawCurrentGlobalX;
    float rawCurrentGlobalY;
};
