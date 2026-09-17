/*******************************************************************************
 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 juceture
  vendor:             Omoiyari-Works
  version:            1.0.0
  name:               JUCEture
  description:        Android gesture recognition module for JUCE
  website:            https://github.com/Omoiyari-Works/JUCEture
  license:            MIT
  minimumCppStandard: 17

  dependencies:       juce_core juce_events juce_graphics juce_gui_basics

 END_JUCE_MODULE_DECLARATION
*******************************************************************************/

#pragma once
#define JUCETURE_H_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

// Common headers
#include "impl/CoordinateConverter.h"
#include "impl/CoordinateAccessor.h"
#include "impl/GestureEvent.h"

// Subdirectory headers
#include "impl/SingleTap/ISingleTapHandler.h"
#include "impl/SingleTap/ISingleTapMediator.h"
#include "impl/SingleTap/SingleTapEvent.h"
#include "impl/SingleTap/SingleTapDetector.h"

#include "impl/LongTap/ILongTapHandler.h"
#include "impl/LongTap/ILongTapMediator.h"
#include "impl/LongTap/LongTapEvent.h"
#include "impl/LongTap/LongTapDetector.h"

#include "impl/Drag/IDragHandler.h"
#include "impl/Drag/IDragMediator.h"
#include "impl/Drag/DragStartEvent.h"
#include "impl/Drag/DragMoveEvent.h"
#include "impl/Drag/DragEndEvent.h"
#include "impl/Drag/DragDetector.h"

#include "impl/Pinch/IPinchHandler.h"
#include "impl/Pinch/IPinchMediator.h"
#include "impl/Pinch/PinchStartEvent.h"
#include "impl/Pinch/PinchScaleEvent.h"
#include "impl/Pinch/PinchEndEvent.h"
#include "impl/Pinch/PinchDetector.h"

// Mediator & API
#include "impl/GestureMediator.h"
#include "impl/JUCEtureAPI.h"
#include "impl/NotifierGestureFromOSJNI.h"
