#include "GestureMediator.h"

GestureMediator& GestureMediator::getInstance()
{
    static GestureMediator instance;
    return instance;
}

GestureMediator::GestureMediator()
    : activeDragHandler(nullptr), activeDragComponent(nullptr),
      dragStartLocal(0.0f, 0.0f), lastLocal(0.0f, 0.0f),
      dragStartGlobal(0.0f, 0.0f), activePinchHandler(nullptr),
      activePinchComponent(nullptr)
{
}

bool GestureMediator::onSingleTap(float rawX, float rawY,
                                  juce::Point<float>& outLocalPt,
                                  juce::Point<float>& outGlobalPt,
                                  juce::Point<float>& outLocalPixels,
                                  juce::Point<float>& outGlobalPixels,
                                  ISingleTapHandler*& outTarget)
{
#if JUCE_ANDROID
    outTarget = getHandlerFromTopmostComponent<ISingleTapHandler>(rawX, rawY, outLocalPt, outGlobalPt);
    if (outTarget != nullptr)
    {
        outGlobalPixels = juce::Point<float>(rawX, rawY);
        outLocalPixels = CoordinateConverter::logicalToPhysical(outLocalPt);
        return true;
    }
    return false;
#else
    return false;
#endif
}

bool GestureMediator::onDragStart(float startRawX, float startRawY,
                                  float currentRawX, float currentRawY,
                                  float /*stepDeltaRawX*/, float /*stepDeltaRawY*/,
                                  juce::Point<float>& outStartLocalPt,
                                  juce::Point<float>& outCurrentLocalPt,
                                  juce::Point<float>& outStartGlobalPt,
                                  juce::Point<float>& outCurrentGlobalPt,
                                  juce::Point<float>& outStartLocalPixels,
                                  juce::Point<float>& outCurrentLocalPixels,
                                  juce::Point<float>& outStartGlobalPixels,
                                  juce::Point<float>& outCurrentGlobalPixels,
                                  IDragHandler*& outTarget)
{
#if JUCE_ANDROID
    juce::Point<float> startLocal;
    juce::Point<float> startGlobal;
    outTarget = getHandlerFromTopmostComponent<IDragHandler>(startRawX, startRawY, startLocal, startGlobal);

    if (outTarget != nullptr)
    {
        auto* comp = dynamic_cast<juce::Component*>(outTarget);
        if (comp == nullptr)
            return false;

        activeDragComponent = comp;

        juce::Point<float> currLocal;
        juce::Point<float> currGlobal;
        if (!CoordinateConverter::rawToComponentLocalAndGlobal(
                *comp, currentRawX, currentRawY, currLocal, currGlobal))
        {
            return false;
        }

        if (comp->getLocalBounds().toFloat().contains(startLocal))
        {
            activeDragHandler = outTarget;
            dragStartLocal = startLocal;
            lastLocal = startLocal;
            dragStartGlobal = startGlobal;

            outStartLocalPt = startLocal;
            outCurrentLocalPt = currLocal;
            outStartGlobalPt = startGlobal;
            outCurrentGlobalPt = currGlobal;
            outStartLocalPixels = CoordinateConverter::logicalToPhysical(startLocal);
            outCurrentLocalPixels = CoordinateConverter::logicalToPhysical(currLocal);
            outStartGlobalPixels = juce::Point<float>(startRawX, startRawY);
            outCurrentGlobalPixels = juce::Point<float>(currentRawX, currentRawY);
            return true;
        }
    }
    return false;
#else
    return false;
#endif
}

bool GestureMediator::onDragMove(float startRawX, float startRawY,
                                 float currentRawX, float currentRawY,
                                 float stepDeltaRawX, float stepDeltaRawY,
                                 juce::Point<float>& outStartLocalPt,
                                 juce::Point<float>& outCurrentLocalPt,
                                 juce::Point<float>& outDeltaFromLastPt,
                                 juce::Point<float>& outStartGlobalPt,
                                 juce::Point<float>& outCurrentGlobalPt,
                                 juce::Point<float>& outStartLocalPixels,
                                 juce::Point<float>& outCurrentLocalPixels,
                                 juce::Point<float>& outDeltaFromLastPixels,
                                 juce::Point<float>& outStartGlobalPixels,
                                 juce::Point<float>& outCurrentGlobalPixels)
{
#if JUCE_ANDROID
    if (activeDragHandler == nullptr || activeDragComponent == nullptr)
    {
        IDragHandler* target = nullptr;
        juce::Point<float> startLocalPt, currLocalPt, startGlobPt, currGlobPt;
        juce::Point<float> startLocalPx, currLocalPx, startGlobPx, currGlobPx;
        if (onDragStart(startRawX, startRawY, currentRawX, currentRawY,
                        stepDeltaRawX, stepDeltaRawY, startLocalPt, currLocalPt,
                        startGlobPt, currGlobPt, startLocalPx, currLocalPx,
                        startGlobPx, currGlobPx, target))
        {
            outStartLocalPt = startLocalPt;
            outCurrentLocalPt = currLocalPt;
            outDeltaFromLastPt = juce::Point<float>(0.0f, 0.0f);
            outStartGlobalPt = startGlobPt;
            outCurrentGlobalPt = currGlobPt;
            outStartLocalPixels = startLocalPx;
            outCurrentLocalPixels = currLocalPx;
            outDeltaFromLastPixels = juce::Point<float>(0.0f, 0.0f);
            outStartGlobalPixels = startGlobPx;
            outCurrentGlobalPixels = currGlobPx;
            return true;
        }
        return false;
    }

    juce::Point<float> currLocal;
    juce::Point<float> currGlobal;
    if (!CoordinateConverter::rawToComponentLocalAndGlobal(
            *activeDragComponent, currentRawX, currentRawY, currLocal, currGlobal))
    {
        return false;
    }

    juce::Point<float> nextLocal;
    if (!CoordinateConverter::rawToComponentLocal(
            *activeDragComponent, currentRawX + stepDeltaRawX,
            currentRawY + stepDeltaRawY, nextLocal))
    {
        return false;
    }

    juce::Point<float> delta(nextLocal.x - currLocal.x, nextLocal.y - currLocal.y);
    lastLocal = nextLocal;

    outStartLocalPt = dragStartLocal;
    outCurrentLocalPt = currLocal;
    outDeltaFromLastPt = delta;
    outStartGlobalPt = dragStartGlobal;
    outCurrentGlobalPt = currGlobal;
    outStartLocalPixels = CoordinateConverter::logicalToPhysical(dragStartLocal);
    outCurrentLocalPixels = CoordinateConverter::logicalToPhysical(currLocal);
    outDeltaFromLastPixels = CoordinateConverter::logicalToPhysical(delta);
    outStartGlobalPixels = juce::Point<float>(startRawX, startRawY);
    outCurrentGlobalPixels = juce::Point<float>(currentRawX, currentRawY);
    return true;
#else
    return false;
#endif
}

bool GestureMediator::onDragEnd(float startRawX, float startRawY,
                                float currentRawX, float currentRawY,
                                float stepDeltaRawX, float stepDeltaRawY,
                                juce::Point<float>& outStartLocalPt,
                                juce::Point<float>& outCurrentLocalPt,
                                juce::Point<float>& outDeltaFromLastPt,
                                juce::Point<float>& outStartGlobalPt,
                                juce::Point<float>& outCurrentGlobalPt,
                                juce::Point<float>& outStartLocalPixels,
                                juce::Point<float>& outCurrentLocalPixels,
                                juce::Point<float>& outDeltaFromLastPixels,
                                juce::Point<float>& outStartGlobalPixels,
                                juce::Point<float>& outCurrentGlobalPixels)
{
#if JUCE_ANDROID
    if (activeDragHandler == nullptr || activeDragComponent == nullptr)
    {
        IDragHandler* target = nullptr;
        juce::Point<float> startLocalPt, currLocalPt, startGlobPt, currGlobPt;
        juce::Point<float> startLocalPx, currLocalPx, startGlobPx, currGlobPx;
        if (onDragStart(startRawX, startRawY, currentRawX, currentRawY,
                        stepDeltaRawX, stepDeltaRawY, startLocalPt, currLocalPt,
                        startGlobPt, currGlobPt, startLocalPx, currLocalPx,
                        startGlobPx, currGlobPx, target))
        {
            outStartLocalPt = startLocalPt;
            outCurrentLocalPt = currLocalPt;
            outDeltaFromLastPt = juce::Point<float>(0.0f, 0.0f);
            outStartGlobalPt = startGlobPt;
            outCurrentGlobalPt = currGlobPt;
            outStartLocalPixels = startLocalPx;
            outCurrentLocalPixels = currLocalPx;
            outDeltaFromLastPixels = juce::Point<float>(0.0f, 0.0f);
            outStartGlobalPixels = startGlobPx;
            outCurrentGlobalPixels = currGlobPx;
            resetDragState();
            return true;
        }
        resetDragState();
        return false;
    }

    juce::Point<float> currLocal;
    juce::Point<float> currGlobal;
    if (!CoordinateConverter::rawToComponentLocalAndGlobal(
            *activeDragComponent, currentRawX, currentRawY, currLocal, currGlobal))
    {
        resetDragState();
        return false;
    }

    juce::Point<float> nextLocal;
    if (!CoordinateConverter::rawToComponentLocal(
            *activeDragComponent, currentRawX + stepDeltaRawX,
            currentRawY + stepDeltaRawY, nextLocal))
    {
        resetDragState();
        return false;
    }

    juce::Point<float> delta(nextLocal.x - currLocal.x, nextLocal.y - currLocal.y);

    outStartLocalPt = dragStartLocal;
    outCurrentLocalPt = currLocal;
    outDeltaFromLastPt = delta;
    outStartGlobalPt = dragStartGlobal;
    outCurrentGlobalPt = currGlobal;
    outStartLocalPixels = CoordinateConverter::logicalToPhysical(dragStartLocal);
    outCurrentLocalPixels = CoordinateConverter::logicalToPhysical(currLocal);
    outDeltaFromLastPixels = CoordinateConverter::logicalToPhysical(delta);
    outStartGlobalPixels = juce::Point<float>(startRawX, startRawY);
    outCurrentGlobalPixels = juce::Point<float>(currentRawX, currentRawY);

    resetDragState();
    return true;
#else
    return false;
#endif
}

GestureMediator::~GestureMediator()
{
}

IDragHandler* GestureMediator::getActiveDragHandler() const
{
    return activeDragHandler;
}

bool GestureMediator::onPinchStart(float focusRawX, float focusRawY,
                                   float /*scaleFactorStep*/,
                                   juce::Point<float>& outFocusLocalPt,
                                   juce::Point<float>& outFocusGlobalPt,
                                   juce::Point<float>& outFocusLocalPixels,
                                   juce::Point<float>& outFocusGlobalPixels,
                                   IPinchHandler*& outTarget)
{
#if JUCE_ANDROID
    outTarget = getHandlerFromTopmostComponent<IPinchHandler>(focusRawX, focusRawY, outFocusLocalPt, outFocusGlobalPt);

    if (outTarget != nullptr)
    {
        auto* comp = dynamic_cast<juce::Component*>(outTarget);
        if (comp == nullptr)
            return false;

        activePinchComponent = comp;

        if (comp->getLocalBounds().toFloat().contains(outFocusLocalPt))
        {
            activePinchHandler = outTarget;
            outFocusLocalPixels = CoordinateConverter::logicalToPhysical(outFocusLocalPt);
            outFocusGlobalPixels = juce::Point<float>(focusRawX, focusRawY);
            return true;
        }
    }
    return false;
#else
    return false;
#endif
}

bool GestureMediator::onPinchScale(float focusRawX, float focusRawY,
                                   float /*scaleFactorStep*/,
                                   juce::Point<float>& outFocusLocalPt,
                                   juce::Point<float>& outFocusGlobalPt,
                                   juce::Point<float>& outFocusLocalPixels,
                                   juce::Point<float>& outFocusGlobalPixels)
{
#if JUCE_ANDROID
    if (activePinchHandler == nullptr || activePinchComponent == nullptr)
    {
        IPinchHandler* target = nullptr;
        juce::Point<float> focusLocalPt, focusGlobPt, focusLocalPx, focusGlobPx;
        if (onPinchStart(focusRawX, focusRawY, 1.0f, focusLocalPt, focusGlobPt,
                        focusLocalPx, focusGlobPx, target))
        {
            outFocusLocalPt = focusLocalPt;
            outFocusGlobalPt = focusGlobPt;
            outFocusLocalPixels = focusLocalPx;
            outFocusGlobalPixels = focusGlobPx;
            return true;
        }
        return false;
    }

    if (!CoordinateConverter::rawToComponentLocalAndGlobal(
            *activePinchComponent, focusRawX, focusRawY, outFocusLocalPt, outFocusGlobalPt))
    {
        return false;
    }

    outFocusLocalPixels = CoordinateConverter::logicalToPhysical(outFocusLocalPt);
    outFocusGlobalPixels = juce::Point<float>(focusRawX, focusRawY);

    return true;
#else
    return false;
#endif
}

bool GestureMediator::onPinchEnd(float focusRawX, float focusRawY,
                                 float /*scaleFactorStep*/,
                                 juce::Point<float>& outFocusLocalPt,
                                 juce::Point<float>& outFocusGlobalPt,
                                 juce::Point<float>& outFocusLocalPixels,
                                 juce::Point<float>& outFocusGlobalPixels)
{
#if JUCE_ANDROID
    if (activePinchHandler == nullptr || activePinchComponent == nullptr)
    {
        IPinchHandler* target = nullptr;
        juce::Point<float> focusLocalPt, focusGlobPt, focusLocalPx, focusGlobPx;
        if (onPinchStart(focusRawX, focusRawY, 1.0f, focusLocalPt, focusGlobPt,
                        focusLocalPx, focusGlobPx, target))
        {
            outFocusLocalPt = focusLocalPt;
            outFocusGlobalPt = focusGlobPt;
            outFocusLocalPixels = focusLocalPx;
            outFocusGlobalPixels = focusGlobPx;
            resetPinchState();
            return true;
        }
        resetPinchState();
        return false;
    }

    if (!CoordinateConverter::rawToComponentLocalAndGlobal(
            *activePinchComponent, focusRawX, focusRawY, outFocusLocalPt, outFocusGlobalPt))
    {
        resetPinchState();
        return false;
    }

    outFocusLocalPixels = CoordinateConverter::logicalToPhysical(outFocusLocalPt);
    outFocusGlobalPixels = juce::Point<float>(focusRawX, focusRawY);

    resetPinchState();
    return true;
#else
    return false;
#endif
}

IPinchHandler* GestureMediator::getActivePinchHandler() const
{
    return activePinchHandler;
}

void GestureMediator::resetDragState()
{
    activeDragHandler = nullptr;
    activeDragComponent = nullptr;
    dragStartLocal = juce::Point<float>(0.0f, 0.0f);
    lastLocal = juce::Point<float>(0.0f, 0.0f);
    dragStartGlobal = juce::Point<float>(0.0f, 0.0f);
}

void GestureMediator::resetPinchState()
{
    activePinchHandler = nullptr;
    activePinchComponent = nullptr;
}

bool GestureMediator::onLongTap(float rawX, float rawY,
                                juce::Point<float>& outLocalPt,
                                juce::Point<float>& outGlobalPt,
                                juce::Point<float>& outLocalPixels,
                                juce::Point<float>& outGlobalPixels,
                                ILongTapHandler*& outTarget)
{
#if JUCE_ANDROID
    outTarget = getHandlerFromTopmostComponent<ILongTapHandler>(rawX, rawY, outLocalPt, outGlobalPt);
    if (outTarget != nullptr)
    {
        outGlobalPixels = juce::Point<float>(rawX, rawY);
        outLocalPixels = CoordinateConverter::logicalToPhysical(outLocalPt);
        return true;
    }
    return false;
#else
    return false;
#endif
}
