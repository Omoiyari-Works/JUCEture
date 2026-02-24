#pragma once

#include "LongTapEvent.h"

class ILongTapHandler
{
  public:
    virtual ~ILongTapHandler() = default;
    virtual void onLongTap(const LongTapEvent& event) = 0;
};
