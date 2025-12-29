#pragma once

#include "ILongTapHandler.h"
#include "ILongTapMediator.h"
#include <JuceHeader.h>

class LongTapDetector
{
  public:
    LongTapDetector();
    explicit LongTapDetector(ILongTapMediator& mediator);
    ~LongTapDetector();

    bool onRawInput(float rawX, float rawY);

  private:
    ILongTapMediator& mediator;
    ILongTapHandler* handler;
};

