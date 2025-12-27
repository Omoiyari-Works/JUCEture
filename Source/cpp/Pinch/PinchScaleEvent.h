#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class PinchScaleEvent : public GestureEvent
{
  public:
    PinchScaleEvent(const juce::Point<float>& focusLocal,
                    float scaleFactorStep,
                    const juce::Point<float>& focusGlobal);
    ~PinchScaleEvent();

    juce::Point<float> getFocusLocal() const;
    float getScaleFactorStep() const;
    juce::Point<float> getFocusGlobal() const;

  private:
    juce::Point<float> focusLocal;
    float scaleFactorStep;
    juce::Point<float> focusGlobal;
};
