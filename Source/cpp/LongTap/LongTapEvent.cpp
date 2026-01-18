#include "LongTapEvent.h"

LongTapEvent::LongTapEvent(const juce::Point<float>& localPointPt,
                           const juce::Point<float>& globalPointPt,
                           const juce::Point<float>& localPointPixels,
                           const juce::Point<float>& globalPointPixels)
    : GestureEvent(), localPointPt(localPointPt), globalPointPt(globalPointPt),
      localPointPixels(localPointPixels), globalPointPixels(globalPointPixels)
{
}

LongTapEvent::~LongTapEvent()
{
}

juce::Point<float> LongTapEvent::getLocalPointInPt() const
{
    return localPointPt;
}

juce::Point<float> LongTapEvent::getGlobalPointInPt() const
{
    return globalPointPt;
}

juce::Point<float> LongTapEvent::getLocalPointInPixels() const
{
    return localPointPixels;
}

juce::Point<float> LongTapEvent::getGlobalPointInPixels() const
{
    return globalPointPixels;
}
