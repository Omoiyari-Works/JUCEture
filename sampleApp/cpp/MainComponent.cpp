#include "MainComponent.h"
#include "JUCEtureAPI.h"
#include "SingleTapEvent.h"
#include "DragStartEvent.h"
#include "DragMoveEvent.h"
#include "DragEndEvent.h"
#include "PinchStartEvent.h"
#include "PinchScaleEvent.h"
#include "PinchEndEvent.h"

#if JUCE_ANDROID
#include <android/log.h>
#endif

namespace
{
#if JUCE_ANDROID
constexpr char kClipBoundsLogTagMainComponent[] = "ClipBoundsMainComponent";
#endif

constexpr float kDragCircleRadiusPx = 36.0f;
constexpr float kDragCircleStrokeThicknessPx = 3.0f;
constexpr float kDragCircleFillAlpha = 0.25f;
const juce::Colour kDragCircleStrokeColour = juce::Colours::orange;
const juce::Colour kDragCircleFillColour = juce::Colours::orange.withAlpha(kDragCircleFillAlpha);
constexpr int kGestureBoxCount = 3;
constexpr int kGestureBoxSpacingPx = 0;

struct NoHitLabel : public juce::Label
{
    NoHitLabel(const juce::String& name, const juce::String& text) : juce::Label(name, text)
    {
    }
    bool hitTest(int, int) override
    {
        return false;
    }
};
} // Namespace

//==============================================================================
MainComponent::MainComponent()
{
    // Initialize JUCEture framework
    JUCEtureAPI::initialize();

    juce::AnimatedAppComponent::setSynchroniseToVBlank(true);
    openGLContext.attachTo(*this);

    setOpaque(true);
    // Constantization (transparent color) to avoid magic numbers
    const juce::Colour colourTop = juce::Colour::fromRGB(0x33, 0x66, 0x99).withAlpha(0.5f);
    const juce::Colour colourMid = juce::Colour::fromRGB(0x66, 0x99, 0x33).withAlpha(0.5f);
    const juce::Colour colourBottom = juce::Colour::fromRGB(0x99, 0x66, 0x33).withAlpha(0.5f);

    boxTop = std::make_unique<GestureTestBox>(colourTop, "Top Box");
    boxMiddle = std::make_unique<GestureTestBox>(colourMid, "Middle Box");
    boxBottom = std::make_unique<GestureTestBox>(colourBottom, "Bottom Box");

    addAndMakeVisible(*boxTop);
    addAndMakeVisible(*boxMiddle);
    addAndMakeVisible(*boxBottom);

    setSize(600, 400);

    // Top status labels (raw/conv)
    statusLabelRaw = std::make_unique<NoHitLabel>("statusLabelRaw", "");
    statusLabelRaw->setJustificationType(juce::Justification::centredLeft);
    statusLabelRaw->setColour(juce::Label::textColourId, juce::Colours::yellow);
    statusLabelRaw->setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.6f));
    statusLabelRaw->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*statusLabelRaw);
    statusLabelRaw->toFront(true);

    statusLabelConv = std::make_unique<NoHitLabel>("statusLabelConv", "");
    statusLabelConv->setJustificationType(juce::Justification::centredLeft);
    statusLabelConv->setColour(juce::Label::textColourId, juce::Colours::cyan);
    statusLabelConv->setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.6f));
    statusLabelConv->setInterceptsMouseClicks(false, false);
    addAndMakeVisible(*statusLabelConv);
    statusLabelConv->toFront(true);
}

MainComponent::~MainComponent()
{
    openGLContext.detach();
}

void MainComponent::update()
{
    // This function is called at the frequency specified by the setFramesPerSecond() call
    // in the constructor. You can use it to update counters, animate values, etc.
}
//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // fill the background
    g.fillAll(juce::Colours::cyan);

    // Get content display area (considering system bar inset)
    auto contentBounds = getContentBounds();

    // For debugging: Visualize the content area
    g.setColour(juce::Colours::darkblue);
    g.drawRect(contentBounds, 2);

    // g.setFont (juce::FontOptions (16.0f));
    // g.setColour (juce::Colours::white);

    // // Place text within content area
    // g.drawText ("Upper Edge (Safe Area)", contentBounds, juce::Justification::topLeft, true);
    // g.drawText ("Downer Edge (Safe Area)", contentBounds, juce::Justification::bottomRight, true);

    // Show inset information
    // auto insetsText = juce::String::formatted ("Insets: T:%d B:%d L:%d R:%d",
    //                                             systemBarInsets.getTop(),
    //                                             systemBarInsets.getBottom(),
    //                                             systemBarInsets.getLeft(),
    //                                             systemBarInsets.getRight());
    // g.drawText (insetsText, contentBounds, juce::Justification::centred, true);

    if (dragIndicatorActive)
    {
        auto circleBounds = juce::Rectangle<float>(kDragCircleRadiusPx * 2.0f,
                                                   kDragCircleRadiusPx * 2.0f)
                                .withCentre(dragCurrentPoint);
        g.setColour(kDragCircleFillColour);
        g.fillEllipse(circleBounds);
        g.setColour(kDragCircleStrokeColour);
        g.drawEllipse(circleBounds, kDragCircleStrokeThicknessPx);
    }
}

void MainComponent::resized()
{
    auto area = getContentBounds();

    auto layoutStatusLabel = [&area](std::unique_ptr<juce::Label>& label)
    {
        if (label == nullptr)
            return;

        auto statusArea = area.removeFromTop(kStatusHeight);
        statusArea = statusArea.reduced(kStatusHorizontalMargin, 0);
        label->setBounds(statusArea);
        label->toFront(true);
    };

    layoutStatusLabel(statusLabelRaw);
    layoutStatusLabel(statusLabelConv);

    auto gestureArea = area;
    if (gestureArea.isEmpty())
        return;

    const int totalSpacing = kGestureBoxSpacingPx * (kGestureBoxCount - 1);
    const int usableHeight = juce::jmax(0, gestureArea.getHeight() - totalSpacing);
    const int baseHeight = (kGestureBoxCount > 0) ? (usableHeight / kGestureBoxCount) : 0;
    int heightRemainder = (kGestureBoxCount > 0) ? (usableHeight % kGestureBoxCount) : 0;

    auto assignBoxBounds = [&gestureArea, &heightRemainder, baseHeight](std::unique_ptr<GestureTestBox>& box,
                                                                        int boxIndex)
    {
        if (box == nullptr)
            return;

        int height = baseHeight;
        if (heightRemainder > 0)
        {
            ++height;
            --heightRemainder;
        }

        auto bounds = gestureArea.removeFromTop(height);
        box->setBounds(bounds);

        if (boxIndex < kGestureBoxCount - 1)
            gestureArea.removeFromTop(kGestureBoxSpacingPx);
    };

    assignBoxBounds(boxTop, 0);
    assignBoxBounds(boxMiddle, 1);
    assignBoxBounds(boxBottom, 2);
}

void MainComponent::parentHierarchyChanged()
{
    juce::Component::parentHierarchyChanged();
}

void MainComponent::visibilityChanged()
{
    juce::Component::visibilityChanged();
    if (!isShowing())
        clearDragIndicator();
}

//==============================================================================
void MainComponent::setSystemBarInsets(int top, int bottom, int left, int right)
{
    systemBarInsets = juce::BorderSize<int>(top, left, bottom, right);

    // Redraw because the inset has changed
    repaint();
    // Resize to affect layout
    resized();
}

juce::Rectangle<int> MainComponent::getContentBounds() const
{
    // Return content area considering system bar insets
    auto bounds = getLocalBounds();
    return systemBarInsets.subtractedFrom(bounds);
}

void MainComponent::updateDragIndicator(const juce::Point<float>& start,
                                        const juce::Point<float>& current)
{
    dragIndicatorActive = true;
    dragStartPoint = start;
    dragCurrentPoint = current;
    repaint();
}

void MainComponent::clearDragIndicator()
{
    if (!dragIndicatorActive)
        return;

    dragIndicatorActive = false;
    if (statusLabelRaw != nullptr)
        statusLabelRaw->setVisible(false);
    repaint();
}

void MainComponent::setStatusText(const juce::String& text)
{
    if (statusLabelRaw != nullptr)
    {
        statusLabelRaw->setText(text, juce::dontSendNotification);
        statusLabelRaw->setVisible(true);
        statusLabelRaw->toFront(true);
        // Maintains front view even when changing size
        resized();
        repaint();
    }
}

void MainComponent::showStatusAt(const juce::String& text, int x, int y)
{
    if (statusLabelRaw == nullptr)
        return;

    statusLabelRaw->setText(text, juce::dontSendNotification);
    statusLabelRaw->setVisible(true);

    // Simple width calculation (approximate character x approximate width + padding)
    const int approxWidth = (int)(text.length() * kStatusCharApproxWidth) + kStatusPadding * 2;
    const int width = juce::jlimit(50, getWidth(), approxWidth);
    const int height = kStatusHeight;

    // Align the top left of the label with (x, y) and clamp it to prevent it from sticking out.
    int left = juce::jlimit(0, juce::jmax(0, getWidth() - width), x);
    int top = juce::jlimit(0, juce::jmax(0, getHeight() - height), y);

    statusLabelRaw->setBounds(left, top, width, height);
    statusLabelRaw->toFront(true);
    repaint();
}

void MainComponent::showTwoStatusAt(const juce::String& rawText, int rawX, int rawY,
                                    const juce::String& convText, int convX, int convY)
{
    if (statusLabelRaw != nullptr)
    {
        const int rawWidth = (int)(rawText.length() * kStatusCharApproxWidth) + kStatusPadding * 2;
        const int w1 = juce::jlimit(50, getWidth(), rawWidth);
        const int h = kStatusHeight;
        const int l1 = juce::jlimit(0, juce::jmax(0, getWidth() - w1), rawX);
        const int t1 = juce::jlimit(0, juce::jmax(0, getHeight() - h), rawY);
        statusLabelRaw->setText(rawText, juce::dontSendNotification);
        statusLabelRaw->setBounds(l1, t1, w1, h);
        statusLabelRaw->setVisible(true);
        statusLabelRaw->toFront(true);
    }

    if (statusLabelConv != nullptr)
    {
        const int convWidth = (int)(convText.length() * kStatusCharApproxWidth) + kStatusPadding * 2;
        const int w2 = juce::jlimit(50, getWidth(), convWidth);
        const int h = kStatusHeight;
        const int l2 = juce::jlimit(0, juce::jmax(0, getWidth() - w2), convX);
        const int t2 = juce::jlimit(0, juce::jmax(0, getHeight() - h), convY);
        statusLabelConv->setText(convText, juce::dontSendNotification);
        statusLabelConv->setBounds(l2, t2, w2, h);
        statusLabelConv->setVisible(true);
        statusLabelConv->toFront(true);
    }

    repaint();
}

void MainComponent::onSingleTap(const SingleTapEvent& event)
{
    juce::ignoreUnused(event);
}

void MainComponent::onDragStart(const DragStartEvent& event)
{
    updateDragIndicator(event.getStartLocal(), event.getCurrentLocal());
}

void MainComponent::onDragMove(const DragMoveEvent& event)
{
    updateDragIndicator(event.getStartLocal(), event.getCurrentLocal());
}

void MainComponent::onDragEnd(const DragEndEvent& event)
{
    juce::ignoreUnused(event);
    clearDragIndicator();
}

void MainComponent::onPinchStart(const PinchStartEvent& event)
{
    juce::ignoreUnused(event);
}

void MainComponent::onPinchScale(const PinchScaleEvent& event)
{
    juce::ignoreUnused(event);
}

void MainComponent::onPinchEnd(const PinchEndEvent& event)
{
    juce::ignoreUnused(event);
}
