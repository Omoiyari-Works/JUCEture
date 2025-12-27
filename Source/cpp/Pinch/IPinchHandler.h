#pragma once

#include "PinchStartEvent.h"
#include "PinchScaleEvent.h"
#include "PinchEndEvent.h"

class IPinchHandler
{
  public:
    virtual ~IPinchHandler() = default;

    virtual void onPinchStart(const PinchStartEvent& event) = 0;

    virtual void onPinchScale(const PinchScaleEvent& event) = 0;

    virtual void onPinchEnd(const PinchEndEvent& event) = 0;
};
