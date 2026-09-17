#include "SingleTapEvent.h"

SingleTapEvent::SingleTapEvent(const juce::Point<float>& localPointPt,
                               const juce::Point<float>& globalPointPt,
                               float rawGlobalX, float rawGlobalY)
    : GestureEvent(), localPt(localPointPt), globalPt(globalPointPt),
      rawGlobalX(rawGlobalX), rawGlobalY(rawGlobalY)
{
}

SingleTapEvent::~SingleTapEvent()
{
}

CoordinateAccessor SingleTapEvent::getLocal() const
{
    return CoordinateAccessor(localPt);
}

CoordinateAccessor SingleTapEvent::getGlobal() const
{
    return CoordinateAccessor(globalPt, rawGlobalX, rawGlobalY);
}
