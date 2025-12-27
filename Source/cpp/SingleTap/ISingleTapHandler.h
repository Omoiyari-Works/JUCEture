#pragma once

#include "SingleTapEvent.h"

class ISingleTapHandler
{
  public:
    virtual ~ISingleTapHandler() = default;
    virtual void onSingleTap(const SingleTapEvent& event) = 0;
};
