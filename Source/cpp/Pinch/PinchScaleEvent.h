#pragma once

#include "../GestureEvent.h"
#include "../CoordinateAccessor.h"
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
                    float rawFocusGlobalX, float rawFocusGlobalY);
    ~PinchScaleEvent();

    // Get focus local coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getFocusLocal() const;

    // Get focus global coordinates (supports both Pt and Pixels via .inPt() or .inPixels())
    CoordinateAccessor getFocusGlobal() const;

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
    float rawFocusGlobalX;
    float rawFocusGlobalY;
};
