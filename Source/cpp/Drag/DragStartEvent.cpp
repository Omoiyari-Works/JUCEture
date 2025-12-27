#include "DragStartEvent.h"

DragStartEvent::DragStartEvent(const juce::Point<float>& startLocal,
                               const juce::Point<float>& currentLocal,
                               const juce::Point<float>& deltaFromLast,
                               const juce::Point<float>& startGlobal,
                               const juce::Point<float>& currentGlobal)
    : GestureEvent(), startLocal(startLocal), currentLocal(currentLocal),
      deltaFromLast(deltaFromLast), startGlobal(startGlobal),
      currentGlobal(currentGlobal)
{
}

DragStartEvent::~DragStartEvent()
{
}

juce::Point<float> DragStartEvent::getStartLocal() const
{
    return startLocal;
}

juce::Point<float> DragStartEvent::getCurrentLocal() const
{
    return currentLocal;
}

juce::Point<float> DragStartEvent::getDeltaFromLast() const
{
    return deltaFromLast;
}

juce::Point<float> DragStartEvent::getStartGlobal() const
{
    return startGlobal;
}

juce::Point<float> DragStartEvent::getCurrentGlobal() const
{
    return currentGlobal;
}
