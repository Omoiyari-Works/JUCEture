#include "LongTapEvent.h"

LongTapEvent::LongTapEvent(const juce::Point<float>& localPoint,
                           const juce::Point<float>& globalPoint)
    : GestureEvent(), localPoint(localPoint), globalPoint(globalPoint)
{
}

LongTapEvent::~LongTapEvent()
{
}

juce::Point<float> LongTapEvent::getLocalPoint() const
{
    return localPoint;
}

juce::Point<float> LongTapEvent::getGlobalPoint() const
{
    return globalPoint;
}
