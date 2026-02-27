#pragma once

#include <JuceHeader.h>

// Public API: Drag gesture support
#include "Drag/IDragHandler.h"
#include "Drag/IDragMediator.h"
#include "Drag/DragStartEvent.h"
#include "Drag/DragMoveEvent.h"
#include "Drag/DragEndEvent.h"

// Public API: Pinch gesture support
#include "Pinch/IPinchHandler.h"
#include "Pinch/IPinchMediator.h"
#include "Pinch/PinchStartEvent.h"
#include "Pinch/PinchScaleEvent.h"
#include "Pinch/PinchEndEvent.h"

// Public API: Single tap support
#include "SingleTap/ISingleTapHandler.h"
#include "SingleTap/ISingleTapMediator.h"
#include "SingleTap/SingleTapEvent.h"

// Public API: Long tap support
#include "LongTap/ILongTapHandler.h"
#include "LongTap/ILongTapMediator.h"
#include "LongTap/LongTapEvent.h"

namespace JUCEtureAPI
{
/**
 * Initialize JUCEture framework.
 * This function must be called once at application startup (e.g., in MainComponent constructor).
 * On Android, this attaches the gesture listener to the main View.
 *
 * @return true if initialization succeeded, false otherwise
 */
bool initialize();
} // namespace JUCEtureAPI
