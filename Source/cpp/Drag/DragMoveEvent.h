#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class DragMoveEvent : public GestureEvent
{
  public:
    DragMoveEvent(const juce::Point<float>& startLocalPt,
                  const juce::Point<float>& currentLocalPt,
                  const juce::Point<float>& deltaFromLastPt,
                  const juce::Point<float>& startGlobalPt,
                  const juce::Point<float>& currentGlobalPt,
                  const juce::Point<float>& startLocalPixels,
                  const juce::Point<float>& currentLocalPixels,
                  const juce::Point<float>& deltaFromLastPixels,
                  const juce::Point<float>& startGlobalPixels,
                  const juce::Point<float>& currentGlobalPixels);
    ~DragMoveEvent();

    // Get coordinates in Pt (logical coordinates with DPI scaling)
    juce::Point<float> getStartLocalInPt() const;
    juce::Point<float> getCurrentLocalInPt() const;
    juce::Point<float> getDeltaFromLastInPt() const;
    juce::Point<float> getStartGlobalInPt() const;
    juce::Point<float> getCurrentGlobalInPt() const;

    // Get coordinates in Pixels (physical coordinates)
    juce::Point<float> getStartLocalInPixels() const;
    juce::Point<float> getCurrentLocalInPixels() const;
    juce::Point<float> getDeltaFromLastInPixels() const;
    juce::Point<float> getStartGlobalInPixels() const;
    juce::Point<float> getCurrentGlobalInPixels() const;

  private:
    juce::Point<float> startLocalPt;
    juce::Point<float> currentLocalPt;
    juce::Point<float> deltaFromLastPt;
    juce::Point<float> startGlobalPt;
    juce::Point<float> currentGlobalPt;
    juce::Point<float> startLocalPixels;
    juce::Point<float> currentLocalPixels;
    juce::Point<float> deltaFromLastPixels;
    juce::Point<float> startGlobalPixels;
    juce::Point<float> currentGlobalPixels;
};
