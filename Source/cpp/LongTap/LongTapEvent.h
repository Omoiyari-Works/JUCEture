#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class LongTapEvent : public GestureEvent
{
  public:
    LongTapEvent(const juce::Point<float>& localPointPt,
                 const juce::Point<float>& globalPointPt,
                 const juce::Point<float>& localPointPixels,
                 const juce::Point<float>& globalPointPixels);
    ~LongTapEvent();

    // Get coordinates in Pt (logical coordinates with DPI scaling)
    juce::Point<float> getLocalPointInPt() const;
    juce::Point<float> getGlobalPointInPt() const;

    // Get coordinates in Pixels (physical coordinates)
    juce::Point<float> getLocalPointInPixels() const;
    juce::Point<float> getGlobalPointInPixels() const;

  private:
    juce::Point<float> localPointPt;
    juce::Point<float> globalPointPt;
    juce::Point<float> localPointPixels;
    juce::Point<float> globalPointPixels;
};
