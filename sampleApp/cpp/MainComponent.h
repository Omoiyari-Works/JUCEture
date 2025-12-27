#pragma once

#include <JuceHeader.h>
#include "GestureTestBox.h"
#include "ISingleTapHandler.h"
#include "IDragHandler.h"
#include "IPinchHandler.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AnimatedAppComponent,
                      public ISingleTapHandler,
                      public IDragHandler,
                      public IPinchHandler
{
  public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void update() override;
    void resized() override;
    void parentHierarchyChanged() override;
    void visibilityChanged() override;

    //==============================================================================
    // Set system bar (status bar/navigation bar) inset
    void setSystemBarInsets(int top, int bottom, int left, int right);

    // Get current inset
    juce::BorderSize<int> getSystemBarInsets() const
    {
        return systemBarInsets;
    }

    // Display status such as coordinates at the front of the screen
    void setStatusText(const juce::String& text);
    void showStatusAt(const juce::String& text, int x, int y);
    void showTwoStatusAt(const juce::String& rawText, int rawX, int rawY,
                         const juce::String& convText, int convX, int convY);

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
    //==============================================================================
    // Status label displayed at the forefront on the screen
    std::unique_ptr<juce::Label> statusLabelRaw;
    std::unique_ptr<juce::Label> statusLabelConv;
    static constexpr int kStatusHeight = 28;
    static constexpr int kStatusPadding = 8;
    static constexpr int kStatusCharApproxWidth = 8;
    static constexpr int kStatusHorizontalMargin = 6;

    std::unique_ptr<GestureTestBox> boxTop;
    std::unique_ptr<GestureTestBox> boxMiddle;
    std::unique_ptr<GestureTestBox> boxBottom;
    bool dragIndicatorActive = false;
    juce::Point<float> dragStartPoint{0.0f, 0.0f};
    juce::Point<float> dragCurrentPoint{0.0f, 0.0f};

    // System bar inset information
    juce::BorderSize<int> systemBarInsets;

    // Calculate content display area
    juce::Rectangle<int> getContentBounds() const;

    void updateDragIndicator(const juce::Point<float>& start,
                             const juce::Point<float>& current);
    void clearDragIndicator();

    juce::OpenGLContext openGLContext;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
