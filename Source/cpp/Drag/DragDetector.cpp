#include "DragDetector.h"
#include "DragStartEvent.h"
#include "DragMoveEvent.h"
#include "DragEndEvent.h"
#include "../GestureMediator.h"

DragDetector::DragDetector()
    : mediator(GestureMediator::getInstance()), handler(nullptr)
{
}

DragDetector::DragDetector(IDragMediator& mediator)
    : mediator(mediator), handler(nullptr)
{
}

DragDetector::~DragDetector()
{
}

void DragDetector::onDragStartRaw(float startRawX, float startRawY,
                                  float currentRawX, float currentRawY,
                                  float stepDeltaRawX, float stepDeltaRawY)
{
#if JUCE_ANDROID
    juce::Point<float> startLocal;
    juce::Point<float> currLocal;
    juce::Point<float> startGlobal;
    juce::Point<float> currGlobal;
    IDragHandler* target = nullptr;

    if (mediator.onDragStart(startRawX, startRawY, currentRawX, currentRawY,
                             stepDeltaRawX, stepDeltaRawY, startLocal, currLocal,
                             startGlobal, currGlobal, target))
    {
        if (target != nullptr)
        {
            // Generate an Event object and notify the handler
            juce::Point<float> delta(0.0f, 0.0f);
            DragStartEvent event(startLocal, currLocal, delta, startGlobal, currGlobal);
            target->onDragStart(event);
        }
    }
#endif
}

void DragDetector::onDragMoveRaw(float startRawX, float startRawY,
                                 float currentRawX, float currentRawY,
                                 float stepDeltaRawX, float stepDeltaRawY)
{
#if JUCE_ANDROID
    juce::Point<float> startLocal;
    juce::Point<float> currLocal;
    juce::Point<float> delta;
    juce::Point<float> startGlobal;
    juce::Point<float> currGlobal;

    if (mediator.onDragMove(startRawX, startRawY, currentRawX, currentRawY,
                            stepDeltaRawX, stepDeltaRawY, startLocal, currLocal,
                            delta, startGlobal, currGlobal))
    {
        IDragHandler* target = mediator.getActiveDragHandler();
        if (target != nullptr)
        {
            DragMoveEvent event(startLocal, currLocal, delta, startGlobal, currGlobal);
            target->onDragMove(event);
        }
    }
#endif
}

void DragDetector::onDragEndRaw(float startRawX, float startRawY,
                                float currentRawX, float currentRawY,
                                float stepDeltaRawX, float stepDeltaRawY)
{
#if JUCE_ANDROID
    IDragHandler* target = mediator.getActiveDragHandler();

    juce::Point<float> startLocal;
    juce::Point<float> currLocal;
    juce::Point<float> delta;
    juce::Point<float> startGlobal;
    juce::Point<float> currGlobal;

    if (mediator.onDragEnd(startRawX, startRawY, currentRawX, currentRawY,
                           stepDeltaRawX, stepDeltaRawY, startLocal, currLocal,
                           delta, startGlobal, currGlobal))
    {
        if (target != nullptr)
        {
            DragEndEvent event(startLocal, currLocal, delta, startGlobal, currGlobal);
            target->onDragEnd(event);
        }
    }
#endif
}
