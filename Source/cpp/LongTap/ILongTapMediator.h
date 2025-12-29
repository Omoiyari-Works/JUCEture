#pragma once

#include "ILongTapHandler.h"
#include <JuceHeader.h>

class ILongTapMediator
{
  public:
    virtual ~ILongTapMediator() = default;

    // Long tap judgment and arbitration
    // Return value: true to notify
    virtual bool onLongTap(float rawX, float rawY, juce::Point<float>& outLocal,
                           juce::Point<float>& outGlobal,
                           ILongTapHandler*& outTarget) = 0;
};

