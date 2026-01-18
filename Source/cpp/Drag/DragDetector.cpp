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
    juce::Point<float> startLocalPt, currLocalPt, startGlobalPt, currGlobalPt;
    juce::Point<float> startLocalPx, currLocalPx, startGlobalPx, currGlobalPx;
    IDragHandler* target = nullptr;

    if (mediator.onDragStart(startRawX, startRawY, currentRawX, currentRawY,
                             stepDeltaRawX, stepDeltaRawY, startLocalPt, currLocalPt,
                             startGlobalPt, currGlobalPt, startLocalPx, currLocalPx,
                             startGlobalPx, currGlobalPx, target))
    {
        if (target != nullptr)
        {
            // Generate an Event object and notify the handler
            juce::Point<float> deltaPt(0.0f, 0.0f);
            juce::Point<float> deltaPx(0.0f, 0.0f);
            DragStartEvent event(startLocalPt, currLocalPt, deltaPt, startGlobalPt, currGlobalPt,
                                startLocalPx, currLocalPx, deltaPx, startGlobalPx, currGlobalPx);
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
    juce::Point<float> startLocalPt, currLocalPt, deltaPt, startGlobalPt, currGlobalPt;
    juce::Point<float> startLocalPx, currLocalPx, deltaPx, startGlobalPx, currGlobalPx;

    if (mediator.onDragMove(startRawX, startRawY, currentRawX, currentRawY,
                            stepDeltaRawX, stepDeltaRawY, startLocalPt, currLocalPt,
                            deltaPt, startGlobalPt, currGlobalPt, startLocalPx, currLocalPx,
                            deltaPx, startGlobalPx, currGlobalPx))
    {
        IDragHandler* target = mediator.getActiveDragHandler();
        if (target != nullptr)
        {
            DragMoveEvent event(startLocalPt, currLocalPt, deltaPt, startGlobalPt, currGlobalPt,
                               startLocalPx, currLocalPx, deltaPx, startGlobalPx, currGlobalPx);
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

    juce::Point<float> startLocalPt, currLocalPt, deltaPt, startGlobalPt, currGlobalPt;
    juce::Point<float> startLocalPx, currLocalPx, deltaPx, startGlobalPx, currGlobalPx;

    if (mediator.onDragEnd(startRawX, startRawY, currentRawX, currentRawY,
                           stepDeltaRawX, stepDeltaRawY, startLocalPt, currLocalPt,
                           deltaPt, startGlobalPt, currGlobalPt, startLocalPx, currLocalPx,
                           deltaPx, startGlobalPx, currGlobalPx))
    {
        if (target != nullptr)
        {
            DragEndEvent event(startLocalPt, currLocalPt, deltaPt, startGlobalPt, currGlobalPt,
                              startLocalPx, currLocalPx, deltaPx, startGlobalPx, currGlobalPx);
            target->onDragEnd(event);
        }
    }
#endif
}
