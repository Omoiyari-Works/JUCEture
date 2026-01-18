#include "PinchEndEvent.h"

PinchEndEvent::PinchEndEvent(const juce::Point<float>& focusLocalPt,
                             const juce::Point<float>& focusGlobalPt,
                             const juce::Point<float>& focusLocalPixels,
                             const juce::Point<float>& focusGlobalPixels)
    : GestureEvent(), focusLocalPt(focusLocalPt), focusGlobalPt(focusGlobalPt),
      focusLocalPixels(focusLocalPixels), focusGlobalPixels(focusGlobalPixels)
{
}

PinchEndEvent::~PinchEndEvent()
{
}

juce::Point<float> PinchEndEvent::getFocusLocalInPt() const
{
    return focusLocalPt;
}

juce::Point<float> PinchEndEvent::getFocusGlobalInPt() const
{
    return focusGlobalPt;
}

juce::Point<float> PinchEndEvent::getFocusLocalInPixels() const
{
    return focusLocalPixels;
}

juce::Point<float> PinchEndEvent::getFocusGlobalInPixels() const
{
    return focusGlobalPixels;
}
