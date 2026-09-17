#include "DragEndEvent.h"

DragEndEvent::DragEndEvent(const juce::Point<float>& startLocalPt,
                           const juce::Point<float>& currentLocalPt,
                           const juce::Point<float>& deltaFromLastPt,
                           const juce::Point<float>& startGlobalPt,
                           const juce::Point<float>& currentGlobalPt,
                           float rawStartGlobalX, float rawStartGlobalY,
                           float rawCurrentGlobalX, float rawCurrentGlobalY)
    : GestureEvent(), startLocalPt(startLocalPt), currentLocalPt(currentLocalPt),
      deltaFromLastPt(deltaFromLastPt), startGlobalPt(startGlobalPt),
      currentGlobalPt(currentGlobalPt),
      rawStartGlobalX(rawStartGlobalX), rawStartGlobalY(rawStartGlobalY),
      rawCurrentGlobalX(rawCurrentGlobalX), rawCurrentGlobalY(rawCurrentGlobalY)
{
}

DragEndEvent::~DragEndEvent()
{
}

CoordinateAccessor DragEndEvent::getStartLocal() const
{
    return CoordinateAccessor(startLocalPt);
}

CoordinateAccessor DragEndEvent::getCurrentLocal() const
{
    return CoordinateAccessor(currentLocalPt);
}

CoordinateAccessor DragEndEvent::getDelta() const
{
    return CoordinateAccessor(deltaFromLastPt);
}

CoordinateAccessor DragEndEvent::getStartGlobal() const
{
    return CoordinateAccessor(startGlobalPt, rawStartGlobalX, rawStartGlobalY);
}

CoordinateAccessor DragEndEvent::getCurrentGlobal() const
{
    return CoordinateAccessor(currentGlobalPt, rawCurrentGlobalX, rawCurrentGlobalY);
}
