#pragma once

#include "IDragHandler.h"
#include "IDragMediator.h"
#include <JuceHeader.h>

class DragDetector
{
  public:
    DragDetector();
    explicit DragDetector(IDragMediator& mediator);
    ~DragDetector();

    void onDragStartRaw(float startRawX, float startRawY, float currentRawX,
                        float currentRawY, float stepDeltaRawX,
                        float stepDeltaRawY);
    void onDragMoveRaw(float startRawX, float startRawY, float currentRawX,
                       float currentRawY, float stepDeltaRawX,
                       float stepDeltaRawY);
    void onDragEndRaw(float startRawX, float startRawY, float currentRawX,
                      float currentRawY, float stepDeltaRawX,
                      float stepDeltaRawY);

  private:
    IDragMediator& mediator;
    IDragHandler* handler;
};
