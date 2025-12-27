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

    // 最前面のISingleTapHandlerを実装したコンポーネントを見つける
    ISingleTapHandler* findTopmostSingleTapHandler(float rawX, float rawY,
                                                   juce::Point<float>& outLocal,
                                                   juce::Point<float>& outGlobal);

    // 最前面のIDragHandlerを実装したコンポーネントを見つける
    IDragHandler* findTopmostDragHandler(float rawX, float rawY,
                                         juce::Point<float>& outLocal,
                                         juce::Point<float>& outGlobal);

    // 最前面のIPinchHandlerを実装したコンポーネントを見つける
    IPinchHandler* findTopmostPinchHandler(float rawX, float rawY,
                                           juce::Point<float>& outLocal,
                                           juce::Point<float>& outGlobal);

    void resetDragState();
    void resetPinchState();
};
