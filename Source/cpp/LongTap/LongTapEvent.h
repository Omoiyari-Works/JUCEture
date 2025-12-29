#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class LongTapEvent : public GestureEvent
{
  public:
    LongTapEvent(const juce::Point<float>& localPoint,
                 const juce::Point<float>& globalPoint);
    ~LongTapEvent();

    juce::Point<float> getLocalPoint() const;
    juce::Point<float> getGlobalPoint() const;

  private:
    juce::Point<float> localPoint;
    juce::Point<float> globalPoint;
};

