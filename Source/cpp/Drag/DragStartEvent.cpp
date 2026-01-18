#include "DragStartEvent.h"

DragStartEvent::DragStartEvent(const juce::Point<float>& startLocalPt,
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

DragStartEvent::~DragStartEvent()
{
}

juce::Point<float> DragStartEvent::getStartLocalInPt() const
{
    return startLocalPt;
}

juce::Point<float> DragStartEvent::getCurrentLocalInPt() const
{
    return currentLocalPt;
}

juce::Point<float> DragStartEvent::getDeltaFromLastInPt() const
{
    return deltaFromLastPt;
}

juce::Point<float> DragStartEvent::getStartGlobalInPt() const
{
    return startGlobalPt;
}

juce::Point<float> DragStartEvent::getCurrentGlobalInPt() const
{
    return currentGlobalPt;
}

juce::Point<float> DragStartEvent::getStartLocalInPixels() const
{
    return startLocalPixels;
}

juce::Point<float> DragStartEvent::getCurrentLocalInPixels() const
{
    return currentLocalPixels;
}

juce::Point<float> DragStartEvent::getDeltaFromLastInPixels() const
{
    return deltaFromLastPixels;
}

juce::Point<float> DragStartEvent::getStartGlobalInPixels() const
{
    return startGlobalPixels;
}

juce::Point<float> DragStartEvent::getCurrentGlobalInPixels() const
{
    return currentGlobalPixels;
}
