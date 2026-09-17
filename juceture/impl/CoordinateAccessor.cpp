#include "CoordinateAccessor.h"

CoordinateAccessor::CoordinateAccessor(const juce::Point<float>& logicalPoint, float rawX, float rawY)
    : logicalPoint(logicalPoint), hasRawCoordinates(true), rawX(rawX), rawY(rawY)
{
}

CoordinateAccessor::CoordinateAccessor(const juce::Point<float>& logicalPoint)
    : logicalPoint(logicalPoint), hasRawCoordinates(false), rawX(0.0f), rawY(0.0f)
{
}

CoordinateAccessor::~CoordinateAccessor()
{
}

juce::Point<float> CoordinateAccessor::inPt() const
{
    return logicalPoint;
}

juce::Point<float> CoordinateAccessor::inPixels() const
{
    if (hasRawCoordinates)
    {
        // For global coordinates, we have the raw (physical) coordinates
        return juce::Point<float>(rawX, rawY);
    }
    else
    {
        // For local coordinates, convert from logical to physical
        return CoordinateConverter::logicalToPhysical(logicalPoint);
    }
}
