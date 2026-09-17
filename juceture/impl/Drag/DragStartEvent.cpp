#include "DragStartEvent.h"

DragStartEvent::DragStartEvent(const juce::Point<float>& startLocalPt,
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

DragStartEvent::~DragStartEvent()
{
}

CoordinateAccessor DragStartEvent::getStartLocal() const
{
    return CoordinateAccessor(startLocalPt);
}

CoordinateAccessor DragStartEvent::getCurrentLocal() const
{
    return CoordinateAccessor(currentLocalPt);
}

CoordinateAccessor DragStartEvent::getDelta() const
{
    return CoordinateAccessor(deltaFromLastPt);
}

CoordinateAccessor DragStartEvent::getStartGlobal() const
{
    return CoordinateAccessor(startGlobalPt, rawStartGlobalX, rawStartGlobalY);
}

CoordinateAccessor DragStartEvent::getCurrentGlobal() const
{
    return CoordinateAccessor(currentGlobalPt, rawCurrentGlobalX, rawCurrentGlobalY);
}
