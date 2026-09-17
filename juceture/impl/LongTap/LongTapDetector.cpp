#include "LongTapDetector.h"
#include "LongTapEvent.h"
#include "../GestureMediator.h"

LongTapDetector::LongTapDetector()
    : mediator(GestureMediator::getInstance()), handler(nullptr)
{
}

LongTapDetector::LongTapDetector(ILongTapMediator& mediator)
    : mediator(mediator), handler(nullptr)
{
}

LongTapDetector::~LongTapDetector()
{
}

bool LongTapDetector::onRawInput(float rawX, float rawY)
{
#if JUCE_ANDROID
    juce::Point<float> localPt, globalPt, localPx, globalPx;
    ILongTapHandler* target = nullptr;

    if (mediator.onLongTap(rawX, rawY, localPt, globalPt, localPx, globalPx, target))
    {
        if (target != nullptr)
        {
            LongTapEvent event(localPt, globalPt, rawX, rawY);
            target->onLongTap(event);
            return true;
        }
    }
    return false;
#else
    return false;
#endif
}
