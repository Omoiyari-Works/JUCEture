#pragma once

#include "IPinchHandler.h"
#include <JuceHeader.h>

class IPinchMediator
{
  public:
    virtual ~IPinchMediator() = default;

    // Determining the onset of a pinch and mediation
    // Return value: true to notify
    virtual bool onPinchStart(float focusRawX, float focusRawY, float scaleFactorStep,
                              juce::Point<float>& outFocusLocal,
                              juce::Point<float>& outFocusGlobal,
                              IPinchHandler*& outTarget) = 0;

    // Pinch scale determination and mediation
    // Return value: true to notify
    virtual bool onPinchScale(float focusRawX, float focusRawY, float scaleFactorStep,
                              juce::Point<float>& outFocusLocal,
                              juce::Point<float>& outFocusGlobal) = 0;

    // Determination and mediation of end of pinch
    // Return value: true to notify
    virtual bool onPinchEnd(float focusRawX, float focusRawY, float scaleFactorStep,
                            juce::Point<float>& outFocusLocal,
                            juce::Point<float>& outFocusGlobal) = 0;

    // Get active pinch handler (used with on pinch scale/on pinch end)
    virtual IPinchHandler* getActivePinchHandler() const = 0;
};
