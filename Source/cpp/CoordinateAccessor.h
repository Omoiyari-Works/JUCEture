#pragma once

#include "CoordinateConverter.h"
#include <JuceHeader.h>

// Provides both Pt and Pixel coordinate access from a single stored value
class CoordinateAccessor
{
  public:
    CoordinateAccessor(const juce::Point<float>& logicalPoint, float rawX, float rawY);
    explicit CoordinateAccessor(const juce::Point<float>& logicalPoint);
    ~CoordinateAccessor();

    // Get coordinates in Pt (logical coordinates with DPI scaling)
    juce::Point<float> inPt() const;

    // Get coordinates in Pixels (physical coordinates)
    juce::Point<float> inPixels() const;

  private:
    juce::Point<float> logicalPoint;
    bool hasRawCoordinates;
    float rawX;
    float rawY;
};
