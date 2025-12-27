#include "DragMoveEvent.h"

DragMoveEvent::DragMoveEvent(const juce::Point<float>& startLocal,
                             const juce::Point<float>& currentLocal,
                             const juce::Point<float>& deltaFromLast,
                             const juce::Point<float>& startGlobal,
                             const juce::Point<float>& currentGlobal)
    : GestureEvent(), startLocal(startLocal), currentLocal(currentLocal),
      deltaFromLast(deltaFromLast), startGlobal(startGlobal),
      currentGlobal(currentGlobal)
{
}

DragMoveEvent::~DragMoveEvent()
{
}

juce::Point<float> DragMoveEvent::getStartLocal() const
{
    return startLocal;
}

juce::Point<float> DragMoveEvent::getCurrentLocal() const
{
    return currentLocal;
}

juce::Point<float> DragMoveEvent::getDeltaFromLast() const
{
    return deltaFromLast;
}

juce::Point<float> DragMoveEvent::getStartGlobal() const
{
    return startGlobal;
}

juce::Point<float> DragMoveEvent::getCurrentGlobal() const
{
    return currentGlobal;
}
