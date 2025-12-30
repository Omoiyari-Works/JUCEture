#pragma once

#include "IPinchHandler.h"
#include "IPinchMediator.h"
#include <JuceHeader.h>

class PinchDetector
{
  public:
    PinchDetector();
    explicit PinchDetector(IPinchMediator& mediator);
    ~PinchDetector();

    void onPinchStartRaw(float focusRawX, float focusRawY, float scaleFactorStep, float scaleFactorStepX, float scaleFactorStepY);
    void onPinchScaleRaw(float focusRawX, float focusRawY, float scaleFactorStep, float scaleFactorStepX, float scaleFactorStepY);
    void onPinchEndRaw(float focusRawX, float focusRawY, float scaleFactorStep, float scaleFactorStepX, float scaleFactorStepY);

  private:
    static constexpr float INITIAL_TOTAL_SCALE_FACTOR = 1.0f;

    IPinchMediator& mediator;
    IPinchHandler* handler;
    float totalScaleFactor;
    float totalScaleFactorX;
    float totalScaleFactorY;
};
