#include "PinchDetector.h"
#include "PinchStartEvent.h"
#include "PinchScaleEvent.h"
#include "PinchEndEvent.h"
#include "../GestureMediator.h"

PinchDetector::PinchDetector()
    : mediator(GestureMediator::getInstance()), handler(nullptr)
{
}

PinchDetector::PinchDetector(IPinchMediator& mediator)
    : mediator(mediator), handler(nullptr)
{
}

PinchDetector::~PinchDetector()
{
}

void PinchDetector::onPinchStartRaw(float focusRawX, float focusRawY,
                                    float scaleFactorStep)
{
#if JUCE_ANDROID
    juce::Point<float> focusLocal;
    juce::Point<float> focusGlobal;
    IPinchHandler* target = nullptr;

    if (mediator.onPinchStart(focusRawX, focusRawY, scaleFactorStep, focusLocal,
                              focusGlobal, target))
    {
        if (target != nullptr)
        {
            PinchStartEvent event(focusLocal, focusGlobal);
            target->onPinchStart(event);
        }
    }
#endif
}

void PinchDetector::onPinchScaleRaw(float focusRawX, float focusRawY,
                                    float scaleFactorStep)
{
#if JUCE_ANDROID
    juce::Point<float> focusLocal;
    juce::Point<float> focusGlobal;

    if (mediator.onPinchScale(focusRawX, focusRawY, scaleFactorStep, focusLocal,
                              focusGlobal))
    {
        IPinchHandler* target = mediator.getActivePinchHandler();
        if (target != nullptr)
        {
            PinchScaleEvent event(focusLocal, scaleFactorStep, focusGlobal);
            target->onPinchScale(event);
        }
    }
#endif
}

void PinchDetector::onPinchEndRaw(float focusRawX, float focusRawY,
                                  float scaleFactorStep)
{
#if JUCE_ANDROID
    IPinchHandler* target = mediator.getActivePinchHandler();

    juce::Point<float> focusLocal;
    juce::Point<float> focusGlobal;

    if (mediator.onPinchEnd(focusRawX, focusRawY, scaleFactorStep, focusLocal,
                            focusGlobal))
    {
        if (target != nullptr)
        {
            PinchEndEvent event(focusLocal, focusGlobal);
            target->onPinchEnd(event);
        }
    }
#endif
}
