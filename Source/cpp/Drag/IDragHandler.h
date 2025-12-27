#pragma once

#include "DragStartEvent.h"
#include "DragMoveEvent.h"
#include "DragEndEvent.h"

class IDragHandler
{
  public:
    virtual ~IDragHandler() = default;
    virtual void onDragStart(const DragStartEvent& event) = 0;
    virtual void onDragMove(const DragMoveEvent& event) = 0;
    virtual void onDragEnd(const DragEndEvent& event) = 0;
};
