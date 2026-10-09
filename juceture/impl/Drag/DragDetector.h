#pragma once

#include "IDragHandler.h"
#include "IDragMediator.h"
#include <juce_gui_basics/juce_gui_basics.h>

class DragDetector
{
  public:
    DragDetector();
    explicit DragDetector(IDragMediator& mediator);
    ~DragDetector();

    // Returns true if an IDragHandler was found and notified of the drag start.
    // Returns false if there is no handler under the drag start point; the
    // caller should then leave the touch events to the normal JUCE processing.
    bool onDragStartRaw(float startRawX, float startRawY, float currentRawX,
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
