#include "PinchEndEvent.h"

PinchEndEvent::PinchEndEvent(const juce::Point<float>& focusLocalPt,
                             const juce::Point<float>& focusGlobalPt,
                             float rawFocusGlobalX, float rawFocusGlobalY)
    : GestureEvent(), focusLocalPt(focusLocalPt), focusGlobalPt(focusGlobalPt),
      rawFocusGlobalX(rawFocusGlobalX), rawFocusGlobalY(rawFocusGlobalY)
{
}

PinchEndEvent::~PinchEndEvent()
{
}

CoordinateAccessor PinchEndEvent::getFocusLocal() const
{
    return CoordinateAccessor(focusLocalPt);
}

CoordinateAccessor PinchEndEvent::getFocusGlobal() const
{
    return CoordinateAccessor(focusGlobalPt, rawFocusGlobalX, rawFocusGlobalY);
}
