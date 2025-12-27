#pragma once

#include "IDragHandler.h"
#include <JuceHeader.h>

class IDragMediator
{
  public:
    virtual ~IDragMediator() = default;

    // Determination and mediation of drag start
    // Return value: true to notify
    virtual bool onDragStart(float startRawX, float startRawY, float currentRawX,
                             float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                             juce::Point<float>& outStartLocal,
                             juce::Point<float>& outCurrentLocal,
                             juce::Point<float>& outStartGlobal,
                             juce::Point<float>& outCurrentGlobal,
                             IDragHandler*& outTarget) = 0;

    // Judgment and arbitration of drag movement
    // Return value: true to notify
    virtual bool onDragMove(float startRawX, float startRawY, float currentRawX,
                            float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                            juce::Point<float>& outStartLocal,
                            juce::Point<float>& outCurrentLocal,
                            juce::Point<float>& outDeltaFromLast,
                            juce::Point<float>& outStartGlobal,
                            juce::Point<float>& outCurrentGlobal) = 0;

    // Judgment and mediation of drag end
    // Return value: true to notify
    virtual bool onDragEnd(float startRawX, float startRawY, float currentRawX,
                           float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                           juce::Point<float>& outStartLocal,
                           juce::Point<float>& outCurrentLocal,
                           juce::Point<float>& outDeltaFromLast,
                           juce::Point<float>& outStartGlobal,
                           juce::Point<float>& outCurrentGlobal) = 0;

    // Get active drag handler (used with on drag move/on drag end)
    virtual IDragHandler* getActiveDragHandler() const = 0;
};
