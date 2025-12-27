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

    void onPinchStartRaw(float focusRawX, float focusRawY, float scaleFactorStep);
    void onPinchScaleRaw(float focusRawX, float focusRawY, float scaleFactorStep);
    void onPinchEndRaw(float focusRawX, float focusRawY, float scaleFactorStep);

  private:
    IPinchMediator& mediator;
    IPinchHandler* handler;
};
