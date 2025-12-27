#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class SingleTapEvent : public GestureEvent
{
  public:
    SingleTapEvent(const juce::Point<float>& localPoint,
                   const juce::Point<float>& globalPoint);
    ~SingleTapEvent();

    juce::Point<float> getLocalPoint() const;
    juce::Point<float> getGlobalPoint() const;

  private:
    juce::Point<float> localPoint;
    juce::Point<float> globalPoint;
};
