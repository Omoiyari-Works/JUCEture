#include "DragMoveEvent.h"

DragMoveEvent::DragMoveEvent(const juce::Point<float>& startLocalPt,
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

DragMoveEvent::~DragMoveEvent()
{
}

CoordinateAccessor DragMoveEvent::getStartLocal() const
{
    return CoordinateAccessor(startLocalPt);
}

CoordinateAccessor DragMoveEvent::getCurrentLocal() const
{
    return CoordinateAccessor(currentLocalPt);
}

CoordinateAccessor DragMoveEvent::getDelta() const
{
    return CoordinateAccessor(deltaFromLastPt);
}

CoordinateAccessor DragMoveEvent::getStartGlobal() const
{
    return CoordinateAccessor(startGlobalPt, rawStartGlobalX, rawStartGlobalY);
}

CoordinateAccessor DragMoveEvent::getCurrentGlobal() const
{
    return CoordinateAccessor(currentGlobalPt, rawCurrentGlobalX, rawCurrentGlobalY);
}
