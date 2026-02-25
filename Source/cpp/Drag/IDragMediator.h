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
                             juce::Point<float>& outStartLocalPt,
                             juce::Point<float>& outCurrentLocalPt,
                             juce::Point<float>& outStartGlobalPt,
                             juce::Point<float>& outCurrentGlobalPt,
                             juce::Point<float>& outStartLocalPixels,
                             juce::Point<float>& outCurrentLocalPixels,
                             juce::Point<float>& outStartGlobalPixels,
                             juce::Point<float>& outCurrentGlobalPixels,
                             IDragHandler*& outTarget) = 0;

    // Judgment and arbitration of drag movement
    // Return value: true to notify
    virtual bool onDragMove(float startRawX, float startRawY, float currentRawX,
                            float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                            juce::Point<float>& outStartLocalPt,
                            juce::Point<float>& outCurrentLocalPt,
                            juce::Point<float>& outDeltaFromLastPt,
                            juce::Point<float>& outStartGlobalPt,
                            juce::Point<float>& outCurrentGlobalPt,
                            juce::Point<float>& outStartLocalPixels,
                            juce::Point<float>& outCurrentLocalPixels,
                            juce::Point<float>& outDeltaFromLastPixels,
                            juce::Point<float>& outStartGlobalPixels,
                            juce::Point<float>& outCurrentGlobalPixels) = 0;

    // Judgment and mediation of drag end
    // Return value: true to notify
    virtual bool onDragEnd(float startRawX, float startRawY, float currentRawX,
                           float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                           juce::Point<float>& outStartLocalPt,
                           juce::Point<float>& outCurrentLocalPt,
                           juce::Point<float>& outDeltaFromLastPt,
                           juce::Point<float>& outStartGlobalPt,
                           juce::Point<float>& outCurrentGlobalPt,
                           juce::Point<float>& outStartLocalPixels,
                           juce::Point<float>& outCurrentLocalPixels,
                           juce::Point<float>& outDeltaFromLastPixels,
                           juce::Point<float>& outStartGlobalPixels,
                           juce::Point<float>& outCurrentGlobalPixels) = 0;

    // Get active drag handler (used with on drag move/on drag end)
    virtual IDragHandler* getActiveDragHandler() const = 0;
};
