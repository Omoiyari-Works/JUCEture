#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class PinchStartEvent : public GestureEvent
{
  public:
    PinchStartEvent(const juce::Point<float>& focusLocal,
                    const juce::Point<float>& focusGlobal);
    ~PinchStartEvent();

    juce::Point<float> getFocusLocal() const;
    juce::Point<float> getFocusGlobal() const;

  private:
    juce::Point<float> focusLocal;
    juce::Point<float> focusGlobal;
};
