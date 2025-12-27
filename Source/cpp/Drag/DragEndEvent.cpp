#include "DragEndEvent.h"

DragEndEvent::DragEndEvent(const juce::Point<float>& startLocal,
                           const juce::Point<float>& currentLocal,
                           const juce::Point<float>& deltaFromLast,
                           const juce::Point<float>& startGlobal,
                           const juce::Point<float>& currentGlobal)
    : GestureEvent(), startLocal(startLocal), currentLocal(currentLocal),
      deltaFromLast(deltaFromLast), startGlobal(startGlobal),
      currentGlobal(currentGlobal)
{
}

DragEndEvent::~DragEndEvent()
{
}

juce::Point<float> DragEndEvent::getStartLocal() const
{
    return startLocal;
}

juce::Point<float> DragEndEvent::getCurrentLocal() const
{
    return currentLocal;
}

juce::Point<float> DragEndEvent::getDeltaFromLast() const
{
    return deltaFromLast;
}

juce::Point<float> DragEndEvent::getStartGlobal() const
{
    return startGlobal;
}

juce::Point<float> DragEndEvent::getCurrentGlobal() const
{
    return currentGlobal;
}
