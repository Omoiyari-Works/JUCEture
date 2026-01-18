#include "SingleTapEvent.h"

SingleTapEvent::SingleTapEvent(const juce::Point<float>& localPointPt,
                               const juce::Point<float>& globalPointPt,
                               const juce::Point<float>& localPointPixels,
                               const juce::Point<float>& globalPointPixels)
    : GestureEvent(), localPointPt(localPointPt), globalPointPt(globalPointPt),
      localPointPixels(localPointPixels), globalPointPixels(globalPointPixels)
{
}

SingleTapEvent::~SingleTapEvent()
{
}

juce::Point<float> SingleTapEvent::getLocalPointInPt() const
{
    return localPointPt;
}

juce::Point<float> SingleTapEvent::getGlobalPointInPt() const
{
    return globalPointPt;
}

juce::Point<float> SingleTapEvent::getLocalPointInPixels() const
{
    return localPointPixels;
}

juce::Point<float> SingleTapEvent::getGlobalPointInPixels() const
{
    return globalPointPixels;
}
