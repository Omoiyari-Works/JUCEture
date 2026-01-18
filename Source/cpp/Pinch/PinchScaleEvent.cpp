#include "PinchScaleEvent.h"

PinchScaleEvent::PinchScaleEvent(const juce::Point<float>& focusLocalPt,
                                 float scaleFactorStep,
                                 float scaleFactorStepX,
                                 float scaleFactorStepY,
                                 float scaleFactorTotal,
                                 float scaleFactorTotalX,
                                 float scaleFactorTotalY,
                                 const juce::Point<float>& focusGlobalPt,
                                 const juce::Point<float>& focusLocalPixels,
                                 const juce::Point<float>& focusGlobalPixels)
    : GestureEvent(), focusLocalPt(focusLocalPt),
      scaleFactorStep(scaleFactorStep),
      scaleFactorStepX(scaleFactorStepX),
      scaleFactorStepY(scaleFactorStepY),
      scaleFactorTotal(scaleFactorTotal),
      scaleFactorTotalX(scaleFactorTotalX),
      scaleFactorTotalY(scaleFactorTotalY),
      focusGlobalPt(focusGlobalPt),
      focusLocalPixels(focusLocalPixels),
      focusGlobalPixels(focusGlobalPixels)
{
}

PinchScaleEvent::~PinchScaleEvent()
{
}

juce::Point<float> PinchScaleEvent::getFocusLocalInPt() const
{
    return focusLocalPt;
}

juce::Point<float> PinchScaleEvent::getFocusGlobalInPt() const
{
    return focusGlobalPt;
}

juce::Point<float> PinchScaleEvent::getFocusLocalInPixels() const
{
    return focusLocalPixels;
}

juce::Point<float> PinchScaleEvent::getFocusGlobalInPixels() const
{
    return focusGlobalPixels;
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
