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

ISingleTapHandler* GestureMediator::findTopmostSingleTapHandler(
    float rawX, float rawY, juce::Point<float>& outLocal,
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

    for (auto* comp = target; comp != nullptr; comp = comp->getParentComponent())
    {
        if (auto* handler = dynamic_cast<ISingleTapHandler*>(comp))
        {
            outLocal = comp->getLocalPoint(&peerComp, topLocal);
            return handler;
        }
    }

    return nullptr;
#else
    return nullptr;
#endif
}

IDragHandler* GestureMediator::findTopmostDragHandler(float rawX, float rawY,
                                                      juce::Point<float>& outLocal,
                                                      juce::Point<float>& outGlobal)
{
#if JUCE_ANDROID
    auto numPeers = juce::ComponentPeer::getNumPeers();
    if (numPeers <= 0)
        return nullptr;

    auto* peer = juce::ComponentPeer::getPeer(0);
    if (peer == nullptr)
        return nullptr;

    auto& peerComp = peer->getComponent();

    if (!CoordinateConverter::rawToLogicalGlobal(rawX, rawY, outGlobal))
        return nullptr;

    juce::Point<float> topLocal = peer->globalToLocal(outGlobal);

    juce::Component* target = peerComp.getComponentAt(topLocal);
    if (target == nullptr)
        return nullptr;

    for (auto* comp = target; comp != nullptr; comp = comp->getParentComponent())
    {
        if (auto* handler = dynamic_cast<IDragHandler*>(comp))
        {
            outLocal = comp->getLocalPoint(&peerComp, topLocal);
            return handler;
        }
    }

    return nullptr;
#else
    return nullptr;
#endif
}

bool GestureMediator::onSingleTap(float rawX, float rawY,
                                  juce::Point<float>& outLocal,
                                  juce::Point<float>& outGlobal,
                                  ISingleTapHandler*& outTarget)
{
#if JUCE_ANDROID
    outTarget = findTopmostSingleTapHandler(rawX, rawY, outLocal, outGlobal);
    return outTarget != nullptr;
#else
    return false;
#endif
}

bool GestureMediator::onDragStart(float startRawX, float startRawY,
                                  float currentRawX, float currentRawY,
                                  float /*stepDeltaRawX*/, float /*stepDeltaRawY*/,
                                  juce::Point<float>& outStartLocal,
                                  juce::Point<float>& outCurrentLocal,
                                  juce::Point<float>& outStartGlobal,
                                  juce::Point<float>& outCurrentGlobal,
                                  IDragHandler*& outTarget)
{
#if JUCE_ANDROID
    juce::Point<float> startLocal;
    juce::Point<float> startGlobal;
    outTarget = findTopmostDragHandler(startRawX, startRawY, startLocal, startGlobal);

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

            outStartLocal = startLocal;
            outCurrentLocal = currLocal;
            outStartGlobal = startGlobal;
            outCurrentGlobal = currGlobal;
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
                                 juce::Point<float>& outStartLocal,
                                 juce::Point<float>& outCurrentLocal,
                                 juce::Point<float>& outDeltaFromLast,
                                 juce::Point<float>& outStartGlobal,
                                 juce::Point<float>& outCurrentGlobal)
{
#if JUCE_ANDROID
    if (activeDragHandler == nullptr || activeDragComponent == nullptr)
    {
        IDragHandler* target = nullptr;
        juce::Point<float> startLocal;
        juce::Point<float> currLocal;
        juce::Point<float> startGlob;
        juce::Point<float> currGlob;
        if (onDragStart(startRawX, startRawY, currentRawX, currentRawY,
                        stepDeltaRawX, stepDeltaRawY, startLocal, currLocal,
                        startGlob, currGlob, target))
        {
            outStartLocal = startLocal;
            outCurrentLocal = currLocal;
            outDeltaFromLast = juce::Point<float>(0.0f, 0.0f);
            outStartGlobal = startGlob;
            outCurrentGlobal = currGlob;
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

    outStartLocal = dragStartLocal;
    outCurrentLocal = currLocal;
    outDeltaFromLast = delta;
    outStartGlobal = dragStartGlobal;
    outCurrentGlobal = currGlobal;
    return true;
#else
    return false;
#endif
}

bool GestureMediator::onDragEnd(float startRawX, float startRawY,
                                float currentRawX, float currentRawY,
                                float stepDeltaRawX, float stepDeltaRawY,
                                juce::Point<float>& outStartLocal,
                                juce::Point<float>& outCurrentLocal,
                                juce::Point<float>& outDeltaFromLast,
                                juce::Point<float>& outStartGlobal,
                                juce::Point<float>& outCurrentGlobal)
{
#if JUCE_ANDROID
    if (activeDragHandler == nullptr || activeDragComponent == nullptr)
    {
        IDragHandler* target = nullptr;
        juce::Point<float> startLocal;
        juce::Point<float> currLocal;
        juce::Point<float> startGlob;
        juce::Point<float> currGlob;
        if (onDragStart(startRawX, startRawY, currentRawX, currentRawY,
                        stepDeltaRawX, stepDeltaRawY, startLocal, currLocal,
                        startGlob, currGlob, target))
        {
            outStartLocal = startLocal;
            outCurrentLocal = currLocal;
            outDeltaFromLast = juce::Point<float>(0.0f, 0.0f);
            outStartGlobal = startGlob;
            outCurrentGlobal = currGlob;
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

    outStartLocal = dragStartLocal;
    outCurrentLocal = currLocal;
    outDeltaFromLast = delta;
    outStartGlobal = dragStartGlobal;
    outCurrentGlobal = currGlobal;

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

IPinchHandler* GestureMediator::findTopmostPinchHandler(float rawX, float rawY,
                                                        juce::Point<float>& outLocal,
                                                        juce::Point<float>& outGlobal)
{
#if JUCE_ANDROID
    auto numPeers = juce::ComponentPeer::getNumPeers();
    if (numPeers <= 0)
        return nullptr;

    auto* peer = juce::ComponentPeer::getPeer(0);
    if (peer == nullptr)
        return nullptr;

    auto& peerComp = peer->getComponent();

    if (!CoordinateConverter::rawToLogicalGlobal(rawX, rawY, outGlobal))
        return nullptr;

    juce::Point<float> topLocal = peer->globalToLocal(outGlobal);

    juce::Component* target = peerComp.getComponentAt(topLocal);
    if (target == nullptr)
        return nullptr;

    for (auto* comp = target; comp != nullptr; comp = comp->getParentComponent())
    {
        if (auto* handler = dynamic_cast<IPinchHandler*>(comp))
        {
            outLocal = comp->getLocalPoint(&peerComp, topLocal);
            return handler;
        }
    }

    return nullptr;
#else
    return nullptr;
#endif
}

bool GestureMediator::onPinchStart(float focusRawX, float focusRawY,
                                   float /*scaleFactorStep*/,
                                   juce::Point<float>& outFocusLocal,
                                   juce::Point<float>& outFocusGlobal,
                                   IPinchHandler*& outTarget)
{
#if JUCE_ANDROID
    outTarget = findTopmostPinchHandler(focusRawX, focusRawY, outFocusLocal, outFocusGlobal);

    if (outTarget != nullptr)
    {
        auto* comp = dynamic_cast<juce::Component*>(outTarget);
        if (comp == nullptr)
            return false;

        activePinchComponent = comp;

        if (comp->getLocalBounds().toFloat().contains(outFocusLocal))
        {
            activePinchHandler = outTarget;
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
                                   juce::Point<float>& outFocusLocal,
                                   juce::Point<float>& outFocusGlobal)
{
#if JUCE_ANDROID
    if (activePinchHandler == nullptr || activePinchComponent == nullptr)
    {
        IPinchHandler* target = nullptr;
        juce::Point<float> focusLocal;
        juce::Point<float> focusGlob;
        if (onPinchStart(focusRawX, focusRawY, 1.0f, focusLocal, focusGlob, target))
        {
            outFocusLocal = focusLocal;
            outFocusGlobal = focusGlob;
            return true;
        }
        return false;
    }

    if (!CoordinateConverter::rawToComponentLocalAndGlobal(
            *activePinchComponent, focusRawX, focusRawY, outFocusLocal, outFocusGlobal))
    {
        return false;
    }

    return true;
#else
    return false;
#endif
}

bool GestureMediator::onPinchEnd(float focusRawX, float focusRawY,
                                 float /*scaleFactorStep*/,
                                 juce::Point<float>& outFocusLocal,
                                 juce::Point<float>& outFocusGlobal)
{
#if JUCE_ANDROID
    if (activePinchHandler == nullptr || activePinchComponent == nullptr)
    {
        IPinchHandler* target = nullptr;
        juce::Point<float> focusLocal;
        juce::Point<float> focusGlob;
        if (onPinchStart(focusRawX, focusRawY, 1.0f, focusLocal, focusGlob, target))
        {
            outFocusLocal = focusLocal;
            outFocusGlobal = focusGlob;
            resetPinchState();
            return true;
        }
        resetPinchState();
        return false;
    }

    if (!CoordinateConverter::rawToComponentLocalAndGlobal(
            *activePinchComponent, focusRawX, focusRawY, outFocusLocal, outFocusGlobal))
    {
        resetPinchState();
        return false;
    }

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
