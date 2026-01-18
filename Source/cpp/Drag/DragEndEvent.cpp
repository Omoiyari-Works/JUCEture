#include "DragEndEvent.h"

DragEndEvent::DragEndEvent(const juce::Point<float>& startLocalPt,
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

DragEndEvent::~DragEndEvent()
{
}

juce::Point<float> DragEndEvent::getStartLocalInPt() const
{
    return startLocalPt;
}

juce::Point<float> DragEndEvent::getCurrentLocalInPt() const
{
    return currentLocalPt;
}

juce::Point<float> DragEndEvent::getDeltaFromLastInPt() const
{
    return deltaFromLastPt;
}

juce::Point<float> DragEndEvent::getStartGlobalInPt() const
{
    return startGlobalPt;
}

juce::Point<float> DragEndEvent::getCurrentGlobalInPt() const
{
    return currentGlobalPt;
}

juce::Point<float> DragEndEvent::getStartLocalInPixels() const
{
    return startLocalPixels;
}

juce::Point<float> DragEndEvent::getCurrentLocalInPixels() const
{
    return currentLocalPixels;
}

juce::Point<float> DragEndEvent::getDeltaFromLastInPixels() const
{
    return deltaFromLastPixels;
}

juce::Point<float> DragEndEvent::getStartGlobalInPixels() const
{
    return startGlobalPixels;
}

juce::Point<float> DragEndEvent::getCurrentGlobalInPixels() const
{
    return currentGlobalPixels;
}
