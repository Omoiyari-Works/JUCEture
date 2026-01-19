#include "LongTapEvent.h"

LongTapEvent::LongTapEvent(const juce::Point<float>& localPointPt,
                           const juce::Point<float>& globalPointPt,
                           float rawGlobalX, float rawGlobalY)
    : GestureEvent(), localPt(localPointPt), globalPt(globalPointPt),
      rawGlobalX(rawGlobalX), rawGlobalY(rawGlobalY)
{
}

LongTapEvent::~LongTapEvent()
{
}

CoordinateAccessor LongTapEvent::getLocal() const
{
    return CoordinateAccessor(localPt);
}

CoordinateAccessor LongTapEvent::getGlobal() const
{
    return CoordinateAccessor(globalPt, rawGlobalX, rawGlobalY);
}
