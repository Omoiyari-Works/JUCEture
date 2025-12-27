#include "PinchStartEvent.h"

PinchStartEvent::PinchStartEvent(const juce::Point<float>& focusLocal,
                                 const juce::Point<float>& focusGlobal)
    : GestureEvent(), focusLocal(focusLocal), focusGlobal(focusGlobal)
{
}

PinchStartEvent::~PinchStartEvent()
{
}

juce::Point<float> PinchStartEvent::getFocusLocal() const
{
    return focusLocal;
}

juce::Point<float> PinchStartEvent::getFocusGlobal() const
{
    return focusGlobal;
}
