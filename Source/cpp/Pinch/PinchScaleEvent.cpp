#include "PinchScaleEvent.h"

PinchScaleEvent::PinchScaleEvent(const juce::Point<float>& focusLocal,
                                 float scaleFactorStep,
                                 float scaleFactorTotal,
                                 const juce::Point<float>& focusGlobal)
    : GestureEvent(), focusLocal(focusLocal), scaleFactorStep(scaleFactorStep),
      scaleFactorTotal(scaleFactorTotal), focusGlobal(focusGlobal)
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

float PinchScaleEvent::getScaleFactorTotal() const
{
    return scaleFactorTotal;
}

juce::Point<float> PinchScaleEvent::getFocusGlobal() const
{
    return focusGlobal;
}
