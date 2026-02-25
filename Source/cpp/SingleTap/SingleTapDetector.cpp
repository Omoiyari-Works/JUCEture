#include "SingleTapDetector.h"
#include "SingleTapEvent.h"
#include "../GestureMediator.h"

SingleTapDetector::SingleTapDetector()
    : mediator(GestureMediator::getInstance()), handler(nullptr)
{
}

SingleTapDetector::SingleTapDetector(ISingleTapMediator& mediator)
    : mediator(mediator), handler(nullptr)
{
}

SingleTapDetector::~SingleTapDetector()
{
}

bool SingleTapDetector::onRawInput(float rawX, float rawY)
{
#if JUCE_ANDROID
    juce::Point<float> localPt, globalPt, localPx, globalPx;
    ISingleTapHandler* target = nullptr;

    if (mediator.onSingleTap(rawX, rawY, localPt, globalPt, localPx, globalPx, target))
    {
        if (target != nullptr)
        {
            SingleTapEvent event(localPt, globalPt, rawX, rawY);
            target->onSingleTap(event);
            return true;
        }
    }
    return false;
#else
    return false;
#endif
}
