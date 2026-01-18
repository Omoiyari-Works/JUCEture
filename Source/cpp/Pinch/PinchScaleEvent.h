#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class PinchScaleEvent : public GestureEvent
{
  public:
    PinchScaleEvent(const juce::Point<float>& focusLocalPt,
                    float scaleFactorStep,
                    float scaleFactorStepX,
                    float scaleFactorStepY,
                    float scaleFactorTotal,
                    float scaleFactorTotalX,
                    float scaleFactorTotalY,
                    const juce::Point<float>& focusGlobalPt,
                    const juce::Point<float>& focusLocalPixels,
                    const juce::Point<float>& focusGlobalPixels);
    ~PinchScaleEvent();

    // Get coordinates in Pt (logical coordinates with DPI scaling)
    juce::Point<float> getFocusLocalInPt() const;
    juce::Point<float> getFocusGlobalInPt() const;

    // Get coordinates in Pixels (physical coordinates)
    juce::Point<float> getFocusLocalInPixels() const;
    juce::Point<float> getFocusGlobalInPixels() const;

    // Get scale factors
    float getScaleFactorStep() const;
    float getScaleFactorStepX() const;
    float getScaleFactorStepY() const;
    float getScaleFactorTotal() const;
    float getScaleFactorTotalX() const;
    float getScaleFactorTotalY() const;

  private:
    juce::Point<float> focusLocalPt;
    float scaleFactorStep;
    float scaleFactorStepX;
    float scaleFactorStepY;
    float scaleFactorTotal;
    float scaleFactorTotalX;
    float scaleFactorTotalY;
    juce::Point<float> focusGlobalPt;
    juce::Point<float> focusLocalPixels;
    juce::Point<float> focusGlobalPixels;
};
