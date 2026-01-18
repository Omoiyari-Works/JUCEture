#include "PinchStartEvent.h"

PinchStartEvent::PinchStartEvent(const juce::Point<float>& focusLocalPt,
                                 const juce::Point<float>& focusGlobalPt,
                                 const juce::Point<float>& focusLocalPixels,
                                 const juce::Point<float>& focusGlobalPixels)
    : GestureEvent(), focusLocalPt(focusLocalPt), focusGlobalPt(focusGlobalPt),
      focusLocalPixels(focusLocalPixels), focusGlobalPixels(focusGlobalPixels)
{
}

PinchStartEvent::~PinchStartEvent()
{
}

juce::Point<float> PinchStartEvent::getFocusLocalInPt() const
{
    return focusLocalPt;
}

juce::Point<float> PinchStartEvent::getFocusGlobalInPt() const
{
    return focusGlobalPt;
}

juce::Point<float> PinchStartEvent::getFocusLocalInPixels() const
{
    return focusLocalPixels;
}

juce::Point<float> PinchStartEvent::getFocusGlobalInPixels() const
{
    return focusGlobalPixels;
}
