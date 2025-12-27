#pragma once

#include <JuceHeader.h>
#include <functional>

#include "ISingleTapHandler.h"
#include "IDragHandler.h"
#include "IPinchHandler.h"

class GestureTestBox : public juce::Component,
                       public ISingleTapHandler,
                       public IDragHandler,
                       public IPinchHandler,
                       private juce::Timer
{
  public:
    GestureTestBox(juce::Colour bg, const juce::String& nameToUse);

    void setStatusText(const juce::String& text);

    // Directly handle taps at local coordinates
    void handleSingleTapAtLocal(int localX, int localY);

    void paint(juce::Graphics& g) override;

    bool hitTest(int, int) override;

#if JUCE_ANDROID
    // Pick up the timing when a peer is created/destroyed due to a change in the parent hierarchy
    void parentHierarchyChanged() override;

    void visibilityChanged() override;
#endif

    ~GestureTestBox() override;

    void resized() override;

    void moved() override;

    // ISingleTapHandler implementation
    void onSingleTap(const SingleTapEvent& event) override;

    // IDragHandler implementation
    void onDragStart(const DragStartEvent& event) override;
    void onDragMove(const DragMoveEvent& event) override;
    void onDragEnd(const DragEndEvent& event) override;

    // IPinchHandler implementation
    void onPinchStart(const PinchStartEvent& event) override;
    void onPinchScale(const PinchScaleEvent& event) override;
    void onPinchEnd(const PinchEndEvent& event) override;

  private:
    juce::Colour background;
    juce::String boxName;
    juce::String status;
    juce::String tapStatus;
    // Retains parameters for each last notified event
    bool hasLastDragStart = false;
    juce::Point<float> lastDragStart_start{0.0f, 0.0f};
    juce::Point<float> lastDragStart_current{0.0f, 0.0f};
    juce::Point<float> lastDragStart_delta{0.0f, 0.0f};

    bool hasLastDragMove = false;
    juce::Point<float> lastDragMove_start{0.0f, 0.0f};
    juce::Point<float> lastDragMove_current{0.0f, 0.0f};
    juce::Point<float> lastDragMove_delta{0.0f, 0.0f};

    bool hasLastDragEnd = false;
    juce::Point<float> lastDragEnd_start{0.0f, 0.0f};
    juce::Point<float> lastDragEnd_current{0.0f, 0.0f};
    juce::Point<float> lastDragEnd_delta{0.0f, 0.0f};

    bool hasLastScaleBegin = false;
    juce::Point<float> lastScaleBegin_focus{0.0f, 0.0f};

    bool hasLastScale = false;
    juce::Point<float> lastScale_focus{0.0f, 0.0f};
    float lastScale_step = 0.0f;

    bool hasLastScaleEnd = false;
    juce::Point<float> lastScaleEnd_focus{0.0f, 0.0f};

    std::function<void(GestureTestBox&, float, float)> onTapped;
    std::function<void(GestureTestBox&,
                       const juce::Point<float>&,
                       const juce::Point<float>&,
                       const juce::Point<float>&)>
        onDragStartCb;
    std::function<void(GestureTestBox&,
                       const juce::Point<float>&,
                       const juce::Point<float>&,
                       const juce::Point<float>&)>
        onDragMoveCb;
    std::function<void(GestureTestBox&,
                       const juce::Point<float>&,
                       const juce::Point<float>&,
                       const juce::Point<float>&)>
        onDragEndCb;
    std::function<void(GestureTestBox&, const juce::Point<float>&)> onPinchStartCb;
    std::function<void(GestureTestBox&, const juce::Point<float>&, float)> onPinchScaleCb;
    std::function<void(GestureTestBox&, const juce::Point<float>&)> onPinchEndCb;
    juce::Rectangle<int> lastScreenBounds;
    juce::Rectangle<int> lastStatusArea;
    juce::Rectangle<int> lastTapArea;
    juce::Rectangle<int> pendingDirtyArea;
    bool frameTimerRunning = false;

    void setTapText(const juce::String& text);
    void maybeSetInitialScreenBoundsStatus();
    void repaintStatusArea();
    void repaintTapArea();
    juce::Rectangle<int> calculateStatusArea() const;
    juce::Rectangle<int> calculateTapArea() const;
    void requestThrottledRepaint(const juce::Rectangle<int>& area);
    void ensureFrameTimer();
    void stopFrameTimer();
    void timerCallback() override;

  public:
    void setOnTapped(std::function<void(GestureTestBox&, float, float)> callback);
    void setOnDragStart(std::function<void(GestureTestBox&,
                                           const juce::Point<float>&,
                                           const juce::Point<float>&,
                                           const juce::Point<float>&)>
                            callback);
    void setOnDragMove(std::function<void(GestureTestBox&,
                                          const juce::Point<float>&,
                                          const juce::Point<float>&,
                                          const juce::Point<float>&)>
                           callback);
    void setOnDragEnd(std::function<void(GestureTestBox&,
                                         const juce::Point<float>&,
                                         const juce::Point<float>&,
                                         const juce::Point<float>&)>
                          callback);
    void setOnPinchStart(std::function<void(GestureTestBox&, const juce::Point<float>&)> callback);
    void setOnPinchScale(std::function<void(GestureTestBox&, const juce::Point<float>&, float)> callback);
    void setOnPinchEnd(std::function<void(GestureTestBox&, const juce::Point<float>&)> callback);
};
