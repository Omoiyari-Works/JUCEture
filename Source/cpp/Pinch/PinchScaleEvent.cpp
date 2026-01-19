#include "PinchScaleEvent.h"

PinchScaleEvent::PinchScaleEvent(const juce::Point<float>& focusLocalPt,
                                 float scaleFactorStep,
                                 float scaleFactorStepX,
                                 float scaleFactorStepY,
                                 float scaleFactorTotal,
                                 float scaleFactorTotalX,
                                 float scaleFactorTotalY,
                                 const juce::Point<float>& focusGlobalPt,
                                 float rawFocusGlobalX, float rawFocusGlobalY)
    : GestureEvent(), focusLocalPt(focusLocalPt),
      scaleFactorStep(scaleFactorStep),
      scaleFactorStepX(scaleFactorStepX),
      scaleFactorStepY(scaleFactorStepY),
      scaleFactorTotal(scaleFactorTotal),
      scaleFactorTotalX(scaleFactorTotalX),
      scaleFactorTotalY(scaleFactorTotalY),
      focusGlobalPt(focusGlobalPt),
      rawFocusGlobalX(rawFocusGlobalX),
      rawFocusGlobalY(rawFocusGlobalY)
{
}

PinchScaleEvent::~PinchScaleEvent()
{
}

CoordinateAccessor PinchScaleEvent::getFocusLocal() const
{
    return CoordinateAccessor(focusLocalPt);
}

CoordinateAccessor PinchScaleEvent::getFocusGlobal() const
{
    return CoordinateAccessor(focusGlobalPt, rawFocusGlobalX, rawFocusGlobalY);
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
