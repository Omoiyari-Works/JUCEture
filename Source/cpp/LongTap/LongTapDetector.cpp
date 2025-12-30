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
    juce::Point<float> local;
    juce::Point<float> global;
    ILongTapHandler* target = nullptr;

    if (mediator.onLongTap(rawX, rawY, local, global, target))
    {
        if (target != nullptr)
        {
            LongTapEvent event(local, global);
            target->onLongTap(event);
            return true;
        }
    }
    return false;
#else
    return false;
#endif
}
