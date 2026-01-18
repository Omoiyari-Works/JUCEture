#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class PinchStartEvent : public GestureEvent
{
  public:
    PinchStartEvent(const juce::Point<float>& focusLocalPt,
                    const juce::Point<float>& focusGlobalPt,
                    const juce::Point<float>& focusLocalPixels,
                    const juce::Point<float>& focusGlobalPixels);
    ~PinchStartEvent();

    // Get coordinates in Pt (logical coordinates with DPI scaling)
    juce::Point<float> getFocusLocalInPt() const;
    juce::Point<float> getFocusGlobalInPt() const;

    // Get coordinates in Pixels (physical coordinates)
    juce::Point<float> getFocusLocalInPixels() const;
    juce::Point<float> getFocusGlobalInPixels() const;

  private:
    juce::Point<float> focusLocalPt;
    juce::Point<float> focusGlobalPt;
    juce::Point<float> focusLocalPixels;
    juce::Point<float> focusGlobalPixels;
};
