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

void SingleTapDetector::onRawInput(float rawX, float rawY)
{
#if JUCE_ANDROID
    juce::Point<float> local;
    juce::Point<float> global;
    ISingleTapHandler* target = nullptr;

    if (mediator.onSingleTap(rawX, rawY, local, global, target))
    {
        if (target != nullptr)
        {
            SingleTapEvent event(local, global);
            target->onSingleTap(event);
        }
    }
#endif
}
