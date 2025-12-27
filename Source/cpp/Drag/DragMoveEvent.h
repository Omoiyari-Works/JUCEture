#pragma once

#include "../GestureEvent.h"
#include <JuceHeader.h>

class DragMoveEvent : public GestureEvent
{
  public:
    DragMoveEvent(const juce::Point<float>& startLocal,
                  const juce::Point<float>& currentLocal,
                  const juce::Point<float>& deltaFromLast,
                  const juce::Point<float>& startGlobal,
                  const juce::Point<float>& currentGlobal);
    ~DragMoveEvent();

    juce::Point<float> getStartLocal() const;
    juce::Point<float> getCurrentLocal() const;
    juce::Point<float> getDeltaFromLast() const;
    juce::Point<float> getStartGlobal() const;
    juce::Point<float> getCurrentGlobal() const;

  private:
    juce::Point<float> startLocal;
    juce::Point<float> currentLocal;
    juce::Point<float> deltaFromLast;
    juce::Point<float> startGlobal;
    juce::Point<float> currentGlobal;
};
