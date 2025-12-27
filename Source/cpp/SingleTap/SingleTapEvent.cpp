#include "SingleTapEvent.h"

SingleTapEvent::SingleTapEvent(const juce::Point<float>& localPoint,
                               const juce::Point<float>& globalPoint)
    : GestureEvent(), localPoint(localPoint), globalPoint(globalPoint)
{
}

SingleTapEvent::~SingleTapEvent()
{
}

juce::Point<float> SingleTapEvent::getLocalPoint() const
{
    return localPoint;
}

juce::Point<float> SingleTapEvent::getGlobalPoint() const
{
    return globalPoint;
}
