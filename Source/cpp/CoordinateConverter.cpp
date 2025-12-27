#include "CoordinateConverter.h"

bool CoordinateConverter::rawToLogicalGlobal(float rawX, float rawY,
                                             juce::Point<float>& outLogicalGlobal)
{
    juce::Point<float> physicalGlobal(rawX, rawY);
    outLogicalGlobal =
        juce::Desktop::getInstance().getDisplays().physicalToLogical(physicalGlobal);
    return true;
}

bool CoordinateConverter::rawToComponentLocal(juce::Component& component,
                                              float rawX, float rawY,
                                              juce::Point<float>& outLocal)
{
    if (auto* peer = component.getPeer())
    {
        auto& peerComp = peer->getComponent();

        juce::Point<float> physicalGlobal(rawX, rawY);
        auto logicalGlobal =
            juce::Desktop::getInstance().getDisplays().physicalToLogical(
                physicalGlobal);
        auto topLocalF = peer->globalToLocal(logicalGlobal);

        outLocal = component.getLocalPoint(&peerComp, topLocalF);
        return true;
    }
    return false;
}

bool CoordinateConverter::rawToComponentLocalAndGlobal(
    juce::Component& component, float rawX, float rawY,
    juce::Point<float>& outLocal, juce::Point<float>& outGlobal)
{
    if (auto* peer = component.getPeer())
    {
        auto& peerComp = peer->getComponent();

        juce::Point<float> physicalGlobal(rawX, rawY);
        auto logicalGlobal =
            juce::Desktop::getInstance().getDisplays().physicalToLogical(
                physicalGlobal);
        outGlobal = logicalGlobal;
        auto topLocalF = peer->globalToLocal(logicalGlobal);

        outLocal = component.getLocalPoint(&peerComp, topLocalF);
        return true;
    }
    return false;
}
