#include "PinchDetector.h"
#include "PinchStartEvent.h"
#include "PinchScaleEvent.h"
#include "PinchEndEvent.h"
#include "../GestureMediator.h"

PinchDetector::PinchDetector()
    : mediator(GestureMediator::getInstance()), handler(nullptr),
      totalScaleFactor(INITIAL_TOTAL_SCALE_FACTOR),
      totalScaleFactorX(INITIAL_TOTAL_SCALE_FACTOR),
      totalScaleFactorY(INITIAL_TOTAL_SCALE_FACTOR)
{
}

PinchDetector::PinchDetector(IPinchMediator& mediator)
    : mediator(mediator), handler(nullptr),
      totalScaleFactor(INITIAL_TOTAL_SCALE_FACTOR),
      totalScaleFactorX(INITIAL_TOTAL_SCALE_FACTOR),
      totalScaleFactorY(INITIAL_TOTAL_SCALE_FACTOR)
{
}

PinchDetector::~PinchDetector()
{
}

void PinchDetector::onPinchStartRaw(float focusRawX, float focusRawY,
                                    float scaleFactorStep,
                                    float scaleFactorStepX,
                                    float scaleFactorStepY)
{
#if JUCE_ANDROID
    // 総倍率を初期値にリセット（開始時点）
    totalScaleFactor = INITIAL_TOTAL_SCALE_FACTOR;
    totalScaleFactorX = INITIAL_TOTAL_SCALE_FACTOR;
    totalScaleFactorY = INITIAL_TOTAL_SCALE_FACTOR;

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
                                    float scaleFactorStep,
                                    float scaleFactorStepX,
                                    float scaleFactorStepY)
{
#if JUCE_ANDROID
    // 総倍率を計算
    totalScaleFactor *= scaleFactorStep;
    totalScaleFactorX *= scaleFactorStepX;
    totalScaleFactorY *= scaleFactorStepY;

    juce::Point<float> focusLocal;
    juce::Point<float> focusGlobal;

    if (mediator.onPinchScale(focusRawX, focusRawY, scaleFactorStep, focusLocal,
                              focusGlobal))
    {
        IPinchHandler* target = mediator.getActivePinchHandler();
        if (target != nullptr)
        {
            PinchScaleEvent event(focusLocal,
                                  scaleFactorStep, scaleFactorStepX, scaleFactorStepY,
                                  totalScaleFactor, totalScaleFactorX, totalScaleFactorY,
                                  focusGlobal);
            target->onPinchScale(event);
        }
    }
#endif
}

void PinchDetector::onPinchEndRaw(float focusRawX, float focusRawY,
                                  float scaleFactorStep,
                                  float scaleFactorStepX,
                                  float scaleFactorStepY)
{
#if JUCE_ANDROID
    // onPinchEndでは総倍率は変更しない
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

    // 総倍率を初期値にリセット
    totalScaleFactor = INITIAL_TOTAL_SCALE_FACTOR;
    totalScaleFactorX = INITIAL_TOTAL_SCALE_FACTOR;
    totalScaleFactorY = INITIAL_TOTAL_SCALE_FACTOR;
#endif
}
