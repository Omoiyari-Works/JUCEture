#include "PinchStartEvent.h"

PinchStartEvent::PinchStartEvent(const juce::Point<float>& focusLocalPt,
                                 const juce::Point<float>& focusGlobalPt,
                                 float rawFocusGlobalX, float rawFocusGlobalY)
    : GestureEvent(), focusLocalPt(focusLocalPt), focusGlobalPt(focusGlobalPt),
      rawFocusGlobalX(rawFocusGlobalX), rawFocusGlobalY(rawFocusGlobalY)
{
}

PinchStartEvent::~PinchStartEvent()
{
}

CoordinateAccessor PinchStartEvent::getFocusLocal() const
{
    return CoordinateAccessor(focusLocalPt);
}

CoordinateAccessor PinchStartEvent::getFocusGlobal() const
{
    return CoordinateAccessor(focusGlobalPt, rawFocusGlobalX, rawFocusGlobalY);
}
