#include "juceture.h"

// Common implementation
#include "impl/CoordinateAccessor.cpp"
#include "impl/CoordinateConverter.cpp"
#include "impl/GestureEvent.cpp"
#include "impl/GestureMediator.cpp"
#include "impl/GesturePeerWatcher.cpp"
#include "impl/JUCEtureAPI.cpp"
#include "impl/NotifierGestureFromOSJNI.cpp"

// SingleTap
#include "impl/SingleTap/SingleTapDetector.cpp"
#include "impl/SingleTap/SingleTapEvent.cpp"

// LongTap
#include "impl/LongTap/LongTapDetector.cpp"
#include "impl/LongTap/LongTapEvent.cpp"

// Drag
#include "impl/Drag/DragDetector.cpp"
#include "impl/Drag/DragStartEvent.cpp"
#include "impl/Drag/DragMoveEvent.cpp"
#include "impl/Drag/DragEndEvent.cpp"

// Pinch
#include "impl/Pinch/PinchDetector.cpp"
#include "impl/Pinch/PinchStartEvent.cpp"
#include "impl/Pinch/PinchScaleEvent.cpp"
#include "impl/Pinch/PinchEndEvent.cpp"
