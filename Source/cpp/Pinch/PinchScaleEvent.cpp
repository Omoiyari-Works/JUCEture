#include "PinchScaleEvent.h"

PinchScaleEvent::PinchScaleEvent(const juce::Point<float>& focusLocal,
                                 float scaleFactorStep,
                                 float scaleFactorStepX,
                                 float scaleFactorStepY,
                                 float scaleFactorTotal,
                                 float scaleFactorTotalX,
                                 float scaleFactorTotalY,
                                 const juce::Point<float>& focusGlobal)
    : GestureEvent(), focusLocal(focusLocal),
      scaleFactorStep(scaleFactorStep),
      scaleFactorStepX(scaleFactorStepX),
      scaleFactorStepY(scaleFactorStepY),
      scaleFactorTotal(scaleFactorTotal),
      scaleFactorTotalX(scaleFactorTotalX),
      scaleFactorTotalY(scaleFactorTotalY),
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

float PinchScaleEvent::getScaleFactorStepX() const
{
    return scaleFactorStepX;
}

float PinchScaleEvent::getScaleFactorStepY() const
{
    return scaleFactorStepY;
}

float PinchScaleEvent::getScaleFactorTotal() const
{
    return scaleFactorTotal;
}

float PinchScaleEvent::getScaleFactorTotalX() const
{
    return scaleFactorTotalX;
}

float PinchScaleEvent::getScaleFactorTotalY() const
{
    return scaleFactorTotalY;
}

juce::Point<float> PinchScaleEvent::getFocusGlobal() const
{
    return focusGlobal;
}
