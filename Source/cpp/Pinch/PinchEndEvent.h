#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class PinchEndEvent : public GestureEvent
{
  public:
    PinchEndEvent(const juce::Point<float>& focusLocal,
                  const juce::Point<float>& focusGlobal);
    ~PinchEndEvent();

    juce::Point<float> getFocusLocal() const;
    juce::Point<float> getFocusGlobal() const;

  private:
    juce::Point<float> focusLocal;
    juce::Point<float> focusGlobal;
};
