#pragma once

#include "SingleTap/ISingleTapMediator.h"
#include "Drag/IDragMediator.h"
#include "Pinch/IPinchMediator.h"
#include "SingleTap/ISingleTapHandler.h"
#include "Drag/IDragHandler.h"
#include "Pinch/IPinchHandler.h"
#include "CoordinateConverter.h"
#include <JuceHeader.h>

class GestureMediator : public ISingleTapMediator, public IDragMediator, public IPinchMediator
{
  public:
    GestureMediator();
    ~GestureMediator();

    static GestureMediator& getInstance();

    // ISingleTapMediator implementation
    bool onSingleTap(float rawX, float rawY, juce::Point<float>& outLocal,
                     juce::Point<float>& outGlobal,
                     ISingleTapHandler*& outTarget) override;

    // IDragMediator implementation
    bool onDragStart(float startRawX, float startRawY, float currentRawX,
                     float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                     juce::Point<float>& outStartLocal,
                     juce::Point<float>& outCurrentLocal,
                     juce::Point<float>& outStartGlobal,
                     juce::Point<float>& outCurrentGlobal,
                     IDragHandler*& outTarget) override;

    bool onDragMove(float startRawX, float startRawY, float currentRawX,
                    float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                    juce::Point<float>& outStartLocal,
                    juce::Point<float>& outCurrentLocal,
                    juce::Point<float>& outDeltaFromLast,
                    juce::Point<float>& outStartGlobal,
                    juce::Point<float>& outCurrentGlobal) override;

    bool onDragEnd(float startRawX, float startRawY, float currentRawX,
                   float currentRawY, float stepDeltaRawX, float stepDeltaRawY,
                   juce::Point<float>& outStartLocal,
                   juce::Point<float>& outCurrentLocal,
                   juce::Point<float>& outDeltaFromLast,
                   juce::Point<float>& outStartGlobal,
                   juce::Point<float>& outCurrentGlobal) override;

    // IDragMediator implementation
    IDragHandler* getActiveDragHandler() const override;

    // IPinchMediator implementation
    bool onPinchStart(float focusRawX, float focusRawY, float scaleFactorStep,
                      juce::Point<float>& outFocusLocal,
                      juce::Point<float>& outFocusGlobal,
                      IPinchHandler*& outTarget) override;

    bool onPinchScale(float focusRawX, float focusRawY, float scaleFactorStep,
                      juce::Point<float>& outFocusLocal,
                      juce::Point<float>& outFocusGlobal) override;

    bool onPinchEnd(float focusRawX, float focusRawY, float scaleFactorStep,
                    juce::Point<float>& outFocusLocal,
                    juce::Point<float>& outFocusGlobal) override;

    // IPinchMediator implementation
    IPinchHandler* getActivePinchHandler() const override;

  private:
    IDragHandler* activeDragHandler;
    juce::Component* activeDragComponent;
    juce::Point<float> dragStartLocal;
    juce::Point<float> lastLocal;
    juce::Point<float> dragStartGlobal;

    IPinchHandler* activePinchHandler;
    juce::Component* activePinchComponent;

    // 指定座標の最前面コンポーネントがハンドラを持っている場合、そのハンドラを返す
    template <typename HandlerType>
    HandlerType* getHandlerFromTopmostComponent(float rawX, float rawY,
                                                 juce::Point<float>& outLocal,
                                                 juce::Point<float>& outGlobal);

    void resetDragState();
    void resetPinchState();
};

template <typename HandlerType>
HandlerType* GestureMediator::getHandlerFromTopmostComponent(float rawX, float rawY,
                                                              juce::Point<float>& outLocal,
                                                              juce::Point<float>& outGlobal)
{
#if JUCE_ANDROID
    auto numPeers = juce::ComponentPeer::getNumPeers();
    if (numPeers <= 0)
    {
        return nullptr;
    }

    auto* peer = juce::ComponentPeer::getPeer(0);
    if (peer == nullptr)
    {
        return nullptr;
    }

    auto& peerComp = peer->getComponent();

    if (!CoordinateConverter::rawToLogicalGlobal(rawX, rawY, outGlobal))
    {
        return nullptr;
    }

    juce::Point<float> topLocal = peer->globalToLocal(outGlobal);

    juce::Component* target = peerComp.getComponentAt(topLocal);
    if (target == nullptr)
    {
        return nullptr;
    }

    if (auto* handler = dynamic_cast<HandlerType*>(target))
    {
        outLocal = target->getLocalPoint(&peerComp, topLocal);
        return handler;
    }

    return nullptr;
#else
    return nullptr;
#endif
}
