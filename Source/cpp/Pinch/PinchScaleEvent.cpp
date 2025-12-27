#include "PinchScaleEvent.h"

PinchScaleEvent::PinchScaleEvent(const juce::Point<float>& focusLocal,
                                 float scaleFactorStep,
                                 const juce::Point<float>& focusGlobal)
    : GestureEvent(), focusLocal(focusLocal), scaleFactorStep(scaleFactorStep),
      focusGlobal(focusGlobal)
{
}

PinchScaleEvent::~PinchScaleEvent()
{
}

juce::Point<float> PinchScaleEvent::getFocusLocal() const
{
    return focusLocal;
}

float PinchScaleEvent::getScaleFactorStep() const
{
    return scaleFactorStep;
}

juce::Point<float> PinchScaleEvent::getFocusGlobal() const
{
    return focusGlobal;
}
