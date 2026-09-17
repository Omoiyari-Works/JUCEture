#pragma once

#include "ISingleTapHandler.h"
#include "ISingleTapMediator.h"
#include <juce_gui_basics/juce_gui_basics.h>

class SingleTapDetector
{
  public:
    SingleTapDetector();
    explicit SingleTapDetector(ISingleTapMediator& mediator);
    ~SingleTapDetector();

    bool onRawInput(float rawX, float rawY);

  private:
    ISingleTapMediator& mediator;
    ISingleTapHandler* handler;
};
