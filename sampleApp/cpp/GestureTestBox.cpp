#include <JuceHeader.h>
#if JUCE_ANDROID
#include <android/log.h>
#endif
#include "GestureTestBox.h"
#include "PinchStartEvent.h"
#include "PinchScaleEvent.h"
#include "PinchEndEvent.h"
#include "SingleTapEvent.h"
#include "DragStartEvent.h"
#include "DragMoveEvent.h"
#include "DragEndEvent.h"

namespace
{
#if JUCE_ANDROID
constexpr char kClipBoundsLogTagGestureTestBox[] = "ClipBoundsGestureBox";
constexpr char kRepaintLogTagGestureTestBox[] = "RepaintGestureBox";
#endif
constexpr int kContentInsetPx = 4;
constexpr int kTitleStatusGapPx = 4;
constexpr float kBodyFontSizePt = 14.0f;
constexpr int kTargetFrameRateHz = 30;

int getBodyLineHeightPx()
{
    static const int lineHeightPx = juce::roundToInt(juce::Font(juce::FontOptions(kBodyFontSizePt)).getHeight());
    return lineHeightPx;
}
} // Namespace

// =========================
// GestureTestBox definitions
// =========================

GestureTestBox::GestureTestBox(juce::Colour bg, const juce::String& nameToUse)
    : background(bg), boxName(nameToUse)
{
    setInterceptsMouseClicks(false, false);
    setOpaque(false);
}

void GestureTestBox::setStatusText(const juce::String& text)
{
    if (text.isNotEmpty())
    {
        status = text;
    }
    else
    {
        // Displays the parameters of each last notified event in one line
        auto fmtPt = [](const juce::Point<float>& p) -> juce::String
        {
            return juce::String("(") + juce::String((int)p.x) + ", " + juce::String((int)p.y) + ")";
        };

        juce::String parts;

        parts << "DragStart=";
        if (hasLastDragStart)
            parts << "start=" << fmtPt(lastDragStart_start)
                  << " current=" << fmtPt(lastDragStart_current)
                  << " delta=" << fmtPt(lastDragStart_delta);
        else
            parts << "-";

        parts << " \nDrag=";
        if (hasLastDragMove)
            parts << "start=" << fmtPt(lastDragMove_start)
                  << " current=" << fmtPt(lastDragMove_current)
                  << " delta=" << fmtPt(lastDragMove_delta);
        else
            parts << "-";

        parts << " \nDragEnd=";
        if (hasLastDragEnd)
            parts << "start=" << fmtPt(lastDragEnd_start)
                  << " current=" << fmtPt(lastDragEnd_current)
                  << " delta=" << fmtPt(lastDragEnd_delta);
        else
            parts << "-";

        parts << " \nScaleBegin=";
        if (hasLastScaleBegin)
            parts << "focus=" << fmtPt(lastScaleBegin_focus);
        else
            parts << "-";

        parts << " \nScale=";
        if (hasLastScale)
            parts << "focus=" << fmtPt(lastScale_focus)
                  << " step=" << juce::String(lastScale_step, 3);
        else
            parts << "-";

        parts << " \nScaleEnd=";
        if (hasLastScaleEnd)
            parts << "focus=" << fmtPt(lastScaleEnd_focus);
        else
            parts << "-";

        status = parts;
    }

    repaintStatusArea();
}

void GestureTestBox::handleSingleTapAtLocal(int localX, int localY)
{
    if (onTapped)
    {
        onTapped(*this, (float)localX, (float)localY);
    }
    else
    {
        setTapText(juce::String("Tap@Local (") + juce::String(localX) + ", " + juce::String(localY) + ")");
    }
}

// ISingleTapHandler implementation
void GestureTestBox::onSingleTap(const SingleTapEvent& event)
{
    const int lx = (int)event.getLocalPoint().x;
    const int ly = (int)event.getLocalPoint().y;
    setTapText(juce::String("SingleTap (") + juce::String(lx) + ", " + juce::String(ly) + ")");
}

// IDragHandler implementation
void GestureTestBox::onDragStart(const DragStartEvent& event)
{
    hasLastDragStart = true;
    lastDragStart_start = event.getStartLocal();
    lastDragStart_current = event.getCurrentLocal();
    lastDragStart_delta = event.getDeltaFromLast();
    setStatusText(juce::String());
}

void GestureTestBox::onDragMove(const DragMoveEvent& event)
{
    hasLastDragMove = true;
    lastDragMove_start = event.getStartLocal();
    lastDragMove_current = event.getCurrentLocal();
    lastDragMove_delta = event.getDeltaFromLast();
    setStatusText(juce::String());
}

void GestureTestBox::onDragEnd(const DragEndEvent& event)
{
    hasLastDragEnd = true;
    lastDragEnd_start = event.getStartLocal();
    lastDragEnd_current = event.getCurrentLocal();
    lastDragEnd_delta = event.getDeltaFromLast();
    setStatusText(juce::String());
}

void GestureTestBox::onPinchStart(const PinchStartEvent& event)
{
    const auto focusLocal = event.getFocusLocal();
    if (onPinchStartCb)
        onPinchStartCb(*this, focusLocal);

    hasLastScaleBegin = true;
    lastScaleBegin_focus = focusLocal;
    setStatusText(juce::String());
}

void GestureTestBox::onPinchScale(const PinchScaleEvent& event)
{
    const auto focusLocal = event.getFocusLocal();
    const auto scaleFactorStep = event.getScaleFactorStep();
    if (onPinchScaleCb)
        onPinchScaleCb(*this, focusLocal, scaleFactorStep);

    hasLastScale = true;
    lastScale_focus = focusLocal;
    lastScale_step = scaleFactorStep;
    setStatusText(juce::String());
}

void GestureTestBox::onPinchEnd(const PinchEndEvent& event)
{
    const auto focusLocal = event.getFocusLocal();
    if (onPinchEndCb)
        onPinchEndCb(*this, focusLocal);

    hasLastScaleEnd = true;
    lastScaleEnd_focus = focusLocal;
    setStatusText(juce::String());
}

void GestureTestBox::paint(juce::Graphics& g)
{
    g.fillAll(background);
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(kBodyFontSizePt));
    auto all = getLocalBounds().reduced(kContentInsetPx);
    const int lineHeightPx = getBodyLineHeightPx();

    if (lineHeightPx <= 0 || all.isEmpty())
    {
        lastStatusArea = {};
        lastTapArea = {};
        return;
    }

    auto titleArea = all.removeFromTop(lineHeightPx);
    g.drawText(boxName, titleArea, juce::Justification::topLeft, true);

    if (status.isNotEmpty())
    {
        juce::StringArray lines;
        lines.addLines(status);
        const int numLines = lines.size();

        if (numLines > 0)
        {
            all.removeFromTop(kTitleStatusGapPx);
            const int statusHeight = juce::jmin(all.getHeight(), lineHeightPx * numLines);
            auto statusArea = all.removeFromTop(statusHeight);
            auto remainingArea = statusArea;

            for (const auto& line : lines)
            {
                if (remainingArea.isEmpty())
                    break;

                auto lineRect = remainingArea.removeFromTop(lineHeightPx);
                g.drawText(line, lineRect, juce::Justification::topLeft, true);
            }
        }
    }

    const auto tapArea = calculateTapArea();
    lastTapArea = tapArea;
    lastStatusArea = calculateStatusArea();

    if (tapStatus.isNotEmpty())
        g.drawText(tapStatus, tapArea, juce::Justification::centred, true);
}

bool GestureTestBox::hitTest(int, int)
{
    return true;
}

#if JUCE_ANDROID
void GestureTestBox::parentHierarchyChanged()
{
    juce::Component::parentHierarchyChanged();
    maybeSetInitialScreenBoundsStatus();
}

void GestureTestBox::visibilityChanged()
{
    juce::Component::visibilityChanged();
    maybeSetInitialScreenBoundsStatus();
}

#endif

GestureTestBox::~GestureTestBox()
{
    stopFrameTimer();
}

void GestureTestBox::resized()
{
    maybeSetInitialScreenBoundsStatus();
}

void GestureTestBox::moved()
{
    maybeSetInitialScreenBoundsStatus();
}

void GestureTestBox::maybeSetInitialScreenBoundsStatus()
{
    // When there is a Peer and the size is non-zero, reflect the latest screen coordinates (updated only when changed)
    if (!isShowing() || getPeer() == nullptr || getWidth() <= 0 || getHeight() <= 0)
        return;

    auto r = getScreenBounds();
    if (r != lastScreenBounds)
    {
        lastScreenBounds = r;
        setStatusText(juce::String("ScreenBounds x=") + juce::String(r.getX()) + " y=" + juce::String(r.getY()) + " w=" + juce::String(r.getWidth()) + " h=" + juce::String(r.getHeight()));
    }
}

void GestureTestBox::repaintStatusArea()
{
    const auto newArea = calculateStatusArea();
    juce::Rectangle<int> dirty = lastStatusArea.isEmpty() ? newArea : lastStatusArea.getUnion(newArea);

    if (!dirty.isEmpty())
        requestThrottledRepaint(dirty);

    lastStatusArea = newArea;
}

void GestureTestBox::repaintTapArea()
{
    const auto newArea = calculateTapArea();
    juce::Rectangle<int> dirty = lastTapArea.isEmpty() ? newArea : lastTapArea.getUnion(newArea);

    if (!dirty.isEmpty())
        requestThrottledRepaint(dirty);

    lastTapArea = newArea;
}

juce::Rectangle<int> GestureTestBox::calculateStatusArea() const
{
    if (status.isEmpty())
        return {};

    auto local = getLocalBounds();
    if (local.isEmpty())
        return {};

    auto content = local.reduced(kContentInsetPx);
    if (content.isEmpty())
        return {};

    const int lineHeightPx = getBodyLineHeightPx();
    if (lineHeightPx <= 0)
        return {};

    auto working = content;
    working.removeFromTop(lineHeightPx);
    working.removeFromTop(kTitleStatusGapPx);

    juce::StringArray lines;
    lines.addLines(status);
    const int numLines = lines.size();
    if (numLines <= 0 || working.isEmpty())
        return {};

    const int statusHeight = juce::jmin(working.getHeight(), lineHeightPx * numLines);
    return working.removeFromTop(statusHeight);
}

juce::Rectangle<int> GestureTestBox::calculateTapArea() const
{
    auto area = getLocalBounds();
    if (area.isEmpty())
        return {};

    area = area.reduced(kContentInsetPx);
    if (area.getWidth() <= 0 || area.getHeight() <= 0)
        return {};

    const int lineHeightPx = getBodyLineHeightPx();
    if (lineHeightPx <= 0)
        return {};

    auto working = area;

    // exclude title line
    working.removeFromTop(lineHeightPx);

    // Exclude status display area
    juce::StringArray lines;
    lines.addLines(status);
    const int numLines = lines.size();
    if (numLines > 0)
    {
        working.removeFromTop(kTitleStatusGapPx);
        const int statusHeight = juce::jmin(working.getHeight(), lineHeightPx * numLines);
        working.removeFromTop(statusHeight);
    }

    if (working.isEmpty())
        return {};

    const int tapHeight = juce::jmin(working.getHeight(), lineHeightPx * 2);
    auto tapArea = working.removeFromBottom(tapHeight);
    return tapArea.reduced(kContentInsetPx / 2, 0);
}

void GestureTestBox::requestThrottledRepaint(const juce::Rectangle<int>& area)
{
    if (area.isEmpty())
        return;

    repaint(area);
}

void GestureTestBox::ensureFrameTimer()
{
    if (frameTimerRunning)
        return;

    startTimerHz(kTargetFrameRateHz);
    frameTimerRunning = true;
}

void GestureTestBox::stopFrameTimer()
{
    if (!frameTimerRunning)
        return;

    stopTimer();
    frameTimerRunning = false;
}

void GestureTestBox::timerCallback()
{
    if (pendingDirtyArea.isEmpty())
    {
        stopFrameTimer();
        return;
    }

    const auto dirty = pendingDirtyArea;
    pendingDirtyArea = {};

    repaint(dirty);

    if (pendingDirtyArea.isEmpty())
        stopFrameTimer();
}

void GestureTestBox::setOnTapped(std::function<void(GestureTestBox&, float, float)> callback)
{
    onTapped = std::move(callback);
}

void GestureTestBox::setOnDragStart(std::function<void(GestureTestBox&,
                                                       const juce::Point<float>&,
                                                       const juce::Point<float>&,
                                                       const juce::Point<float>&)>
                                        callback)
{
    onDragStartCb = std::move(callback);
}

void GestureTestBox::setOnDragMove(std::function<void(GestureTestBox&,
                                                      const juce::Point<float>&,
                                                      const juce::Point<float>&,
                                                      const juce::Point<float>&)>
                                       callback)
{
    onDragMoveCb = std::move(callback);
}

void GestureTestBox::setOnDragEnd(std::function<void(GestureTestBox&,
                                                     const juce::Point<float>&,
                                                     const juce::Point<float>&,
                                                     const juce::Point<float>&)>
                                      callback)
{
    onDragEndCb = std::move(callback);
}

void GestureTestBox::setTapText(const juce::String& text)
{
    tapStatus = text;
    repaintTapArea();
}

void GestureTestBox::setOnPinchStart(std::function<void(GestureTestBox&, const juce::Point<float>&)> callback)
{
    onPinchStartCb = std::move(callback);
}

void GestureTestBox::setOnPinchScale(std::function<void(GestureTestBox&, const juce::Point<float>&, float)> callback)
{
    onPinchScaleCb = std::move(callback);
}

void GestureTestBox::setOnPinchEnd(std::function<void(GestureTestBox&, const juce::Point<float>&)> callback)
{
    onPinchEndCb = std::move(callback);
}
