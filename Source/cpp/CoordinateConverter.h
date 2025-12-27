#pragma once

#include <JuceHeader.h>

class CoordinateConverter
{
  public:
    // Convert from raw coordinates (physical coordinates) to logical global coordinates
    static bool rawToLogicalGlobal(float rawX, float rawY,
                                   juce::Point<float>& outLogicalGlobal);

    // Convert from raw coordinates to local coordinates of specified component
    static bool rawToComponentLocal(juce::Component& component, float rawX,
                                    float rawY, juce::Point<float>& outLocal);

    // Get both logical global coordinates and local coordinates of specified component from raw coordinates
    static bool rawToComponentLocalAndGlobal(juce::Component& component,
                                             float rawX, float rawY,
                                             juce::Point<float>& outLocal,
                                             juce::Point<float>& outGlobal);
};
