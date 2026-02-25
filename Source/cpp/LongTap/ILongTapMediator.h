#pragma once

#include "ILongTapHandler.h"
#include <JuceHeader.h>

class ILongTapMediator
{
  public:
    virtual ~ILongTapMediator() = default;

    // Long tap judgment and arbitration
    // Return value: true to notify
    virtual bool onLongTap(float rawX, float rawY,
                           juce::Point<float>& outLocalPt,
                           juce::Point<float>& outGlobalPt,
                           juce::Point<float>& outLocalPixels,
                           juce::Point<float>& outGlobalPixels,
                           ILongTapHandler*& outTarget) = 0;
};
