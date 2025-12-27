#pragma once

#include "ISingleTapHandler.h"
#include <JuceHeader.h>

class ISingleTapMediator
{
  public:
    virtual ~ISingleTapMediator() = default;

    // Single tap judgment and arbitration
    // Return value: true to notify
    virtual bool onSingleTap(float rawX, float rawY, juce::Point<float>& outLocal,
                             juce::Point<float>& outGlobal,
                             ISingleTapHandler*& outTarget) = 0;
};
