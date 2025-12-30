#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class PinchScaleEvent : public GestureEvent
{
  public:
    PinchScaleEvent(const juce::Point<float>& focusLocal,
                    float scaleFactorStep,
                    float scaleFactorStepX,
                    float scaleFactorStepY,
                    float scaleFactorTotal,
                    float scaleFactorTotalX,
                    float scaleFactorTotalY,
                    const juce::Point<float>& focusGlobal);
    ~PinchScaleEvent();

    juce::Point<float> getFocusLocal() const;
    float getScaleFactorStep() const;
    float getScaleFactorStepX() const;
    float getScaleFactorStepY() const;
    float getScaleFactorTotal() const;
    float getScaleFactorTotalX() const;
    float getScaleFactorTotalY() const;
    juce::Point<float> getFocusGlobal() const;

  private:
    juce::Point<float> focusLocal;
    float scaleFactorStep;
    float scaleFactorStepX;
    float scaleFactorStepY;
    float scaleFactorTotal;
    float scaleFactorTotalX;
    float scaleFactorTotalY;
    juce::Point<float> focusGlobal;
};
