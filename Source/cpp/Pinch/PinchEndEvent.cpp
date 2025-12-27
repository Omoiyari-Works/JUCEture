#include "PinchEndEvent.h"

PinchEndEvent::PinchEndEvent(const juce::Point<float>& focusLocal,
                             const juce::Point<float>& focusGlobal)
    : GestureEvent(), focusLocal(focusLocal), focusGlobal(focusGlobal)
{
}

PinchEndEvent::~PinchEndEvent()
{
}

juce::Point<float> PinchEndEvent::getFocusLocal() const
{
    return focusLocal;
}

juce::Point<float> PinchEndEvent::getFocusGlobal() const
{
    return focusGlobal;
}
