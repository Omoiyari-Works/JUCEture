#pragma once

#include "ISingleTapHandler.h"
#include <JuceHeader.h>

class ISingleTapMediator
{
  public:
    virtual ~ISingleTapMediator() = default;

    // Single tap judgment and arbitration
    // Return value: true to notify
    virtual bool onSingleTap(float rawX, float rawY,
                             juce::Point<float>& outLocalPt,
                             juce::Point<float>& outGlobalPt,
                             juce::Point<float>& outLocalPixels,
                             juce::Point<float>& outGlobalPixels,
                             ISingleTapHandler*& outTarget) = 0;
};
