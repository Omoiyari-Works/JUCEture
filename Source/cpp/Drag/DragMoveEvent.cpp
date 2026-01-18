#include "DragMoveEvent.h"

DragMoveEvent::DragMoveEvent(const juce::Point<float>& startLocalPt,
                             const juce::Point<float>& currentLocalPt,
                             const juce::Point<float>& deltaFromLastPt,
                             const juce::Point<float>& startGlobalPt,
                             const juce::Point<float>& currentGlobalPt,
                             const juce::Point<float>& startLocalPixels,
                             const juce::Point<float>& currentLocalPixels,
                             const juce::Point<float>& deltaFromLastPixels,
                             const juce::Point<float>& startGlobalPixels,
                             const juce::Point<float>& currentGlobalPixels)
    : GestureEvent(), startLocalPt(startLocalPt), currentLocalPt(currentLocalPt),
      deltaFromLastPt(deltaFromLastPt), startGlobalPt(startGlobalPt),
      currentGlobalPt(currentGlobalPt), startLocalPixels(startLocalPixels),
      currentLocalPixels(currentLocalPixels), deltaFromLastPixels(deltaFromLastPixels),
      startGlobalPixels(startGlobalPixels), currentGlobalPixels(currentGlobalPixels)
{
}

DragMoveEvent::~DragMoveEvent()
{
}

juce::Point<float> DragMoveEvent::getStartLocalInPt() const
{
    return startLocalPt;
}

juce::Point<float> DragMoveEvent::getCurrentLocalInPt() const
{
    return currentLocalPt;
}

juce::Point<float> DragMoveEvent::getDeltaFromLastInPt() const
{
    return deltaFromLastPt;
}

juce::Point<float> DragMoveEvent::getStartGlobalInPt() const
{
    return startGlobalPt;
}

juce::Point<float> DragMoveEvent::getCurrentGlobalInPt() const
{
    return currentGlobalPt;
}

juce::Point<float> DragMoveEvent::getStartLocalInPixels() const
{
    return startLocalPixels;
}

juce::Point<float> DragMoveEvent::getCurrentLocalInPixels() const
{
    return currentLocalPixels;
}

juce::Point<float> DragMoveEvent::getDeltaFromLastInPixels() const
{
    return deltaFromLastPixels;
}

juce::Point<float> DragMoveEvent::getStartGlobalInPixels() const
{
    return startGlobalPixels;
}

juce::Point<float> DragMoveEvent::getCurrentGlobalInPixels() const
{
    return currentGlobalPixels;
}
