package com.juceture.android;

import android.view.GestureDetector;
import android.view.ScaleGestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.content.Context;
import android.util.Log;
import java.util.WeakHashMap;

public final class NotifierGestureFromAndroid {
    private NotifierGestureFromAndroid() {
    }

    // Notify C++ of coordinates (View -> peer -> global -> getComponentAt in C++)
    // Returns true if ISingleTapHandler was found and handled, false otherwise
    private static native boolean onSingleTap(float xPx, float yPx, float density);

    private static native void onDragStart(float startRawX, float startRawY,
            float currentRawX, float currentRawY,
            float deltaX, float deltaY,
            float density, long nativePtr);

    private static native void onDragMove(float startRawX, float startRawY,
            float currentRawX, float currentRawY,
            float deltaX, float deltaY,
            float density, long nativePtr);

    private static native void onDragEnd(float startRawX, float startRawY,
            float currentRawX, float currentRawY,
            float deltaX, float deltaY,
            float density, long nativePtr);

    // Pinch (scale) gesture
    private static native void onPinchStart(float focusXInView, float focusYInView,
            float scaleFactorStep, float density, long nativePtr);

    private static native void onPinchScale(float focusXInView, float focusYInView,
            float scaleFactorStep, float density, long nativePtr);

    private static native void onPinchEnd(float focusXInView, float focusYInView,
            float scaleFactorStep, float density, long nativePtr);

    private static final int PHASE_START = 0;
    private static final int PHASE_MOVE = 1;
    private static final int PHASE_END = 2;
    private static final int MIN_POINTER_COUNT_FOR_PINCH = 2;

    private static void sendDragToNative(int phase,
            float startRawX, float startRawY,
            float currentRawX, float currentRawY,
            float deltaX, float deltaY,
            float density, long nativePtr) {
        switch (phase) {
            case PHASE_START:
                onDragStart(startRawX, startRawY, currentRawX, currentRawY, deltaX, deltaY, density, nativePtr);
                break;
            case PHASE_MOVE:
                onDragMove(startRawX, startRawY, currentRawX, currentRawY, deltaX, deltaY, density, nativePtr);
                break;
            case PHASE_END:
                onDragEnd(startRawX, startRawY, currentRawX, currentRawY, deltaX, deltaY, density, nativePtr);
                break;
            default:
                Log.w("NotifierGestureFromAndroid", "Unknown drag phase: " + phase);
        }
    }

    // Track on touch wrapper attached for each View
    private static final WeakHashMap<View, OnTouchWrapper> sAttached = new WeakHashMap<>();

    public static void attach(final View view, final long nativePtr) {
        if (view == null) {
            return;
        }

        OnTouchWrapper wrapper = sAttached.get(view);
        if (wrapper == null) {
            wrapper = new OnTouchWrapper(view.getContext());
            view.setOnTouchListener(wrapper);
            sAttached.put(view, wrapper);
        }
        wrapper.setNativePtr(nativePtr);
        wrapper.addRef();
    }

    public static void detach(final View view, final long nativePtr) {
        if (view == null)
            return;

        final OnTouchWrapper wrapper = sAttached.get(view);
        if (wrapper != null) {
            wrapper.release();
            if (wrapper.getRefCount() <= 0) {
                view.setOnTouchListener(null);
                sAttached.remove(view);
            }
        }
    }

    private static final class OnTouchWrapper implements View.OnTouchListener {
        private final GestureDetector detector;
        private final ScaleGestureDetector scaleDetector;
        private int refCount = 0;
        private final Context context;
        private long nativePtr = 0L;
        private boolean dragging = false;
        private boolean pinching = false;
        private boolean pinchGestureActive = false;
        private float dragStartRawX = 0f;
        private float dragStartRawY = 0f;
        private float lastRawX = 0f;
        private float lastRawY = 0f;
        private float lastRawFocusX = 0f;
        private float lastRawFocusY = 0f;
        private boolean lastSingleTapHandled = false;

        OnTouchWrapper(Context context) {
            this.context = context;
            this.detector = new GestureDetector(
                    context,
                    new GestureDetector.SimpleOnGestureListener() {
                        @Override
                        public boolean onSingleTapUp(MotionEvent e) {
                            try {
                                // Pass screen coordinates (raw px) and density (move to top-level local in C++)
                                final float density = context.getResources().getDisplayMetrics().density;
                                boolean handled = onSingleTap(e.getRawX(), e.getRawY(), density);
                                // Store the result for use in onTouch()
                                lastSingleTapHandled = handled;
                                // Return true only if ISingleTapHandler was found and handled the event
                                // Return false to allow JUCE's normal mouse event processing
                                return handled;
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onSingleTap", err);
                                lastSingleTapHandled = false;
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onSingleTap", t);
                                lastSingleTapHandled = false;
                            }
                            // If exception occurred, return false to allow JUCE's normal processing
                            return false;
                        }

                        @Override
                        public boolean onDown(MotionEvent e) {
                            // Return true to receive On scroll
                            return true;
                        }

                        @Override
                        public boolean onScroll(MotionEvent dragStartPoint, MotionEvent dragCurrentPoint,
                                float distanceX, float distanceY) {
                            if (pinching) {
                                // Suppress drag notifications while in a pinch
                                return true;
                            }
                            final float density = context.getResources().getDisplayMetrics().density;
                            try {
                                if (!dragging) {
                                    dragging = true;
                                    dragStartRawX = dragStartPoint.getRawX();
                                    dragStartRawY = dragStartPoint.getRawY();
                                    lastRawX = dragStartRawX;
                                    lastRawY = dragStartRawY;
                                    // DragStart: current = start, delta = (0,0)
                                    sendDragToNative(PHASE_START,
                                            dragStartRawX, dragStartRawY,
                                            dragStartRawX, dragStartRawY,
                                            0f, 0f,
                                            density, nativePtr);
                                }
                                final float currentRawX = dragCurrentPoint.getRawX();
                                final float currentRawY = dragCurrentPoint.getRawY();
                                sendDragToNative(PHASE_MOVE,
                                        dragStartRawX, dragStartRawY,
                                        currentRawX, currentRawY,
                                        distanceX, distanceY,
                                        density, nativePtr);
                                lastRawX = currentRawX;
                                lastRawY = currentRawY;
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onScroll", err);
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onScroll", t);
                            }
                            return true;
                        }
                    });
            this.scaleDetector = new ScaleGestureDetector(
                    context,
                    new ScaleGestureDetector.OnScaleGestureListener() {
                        @Override
                        public boolean onScaleBegin(ScaleGestureDetector detector) {
                            try {
                                pinchGestureActive = true;
                                ensurePinchingForScale();
                                final float density = context.getResources().getDisplayMetrics().density;
                                final float scaleFactorStep = detector.getScaleFactor();
                                onPinchStart(lastRawFocusX, lastRawFocusY, scaleFactorStep, density, nativePtr);
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onScaleBegin", err);
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onScaleBegin", t);
                            }
                            return true;
                        }

                        @Override
                        public boolean onScale(ScaleGestureDetector detector) {
                            try {
                                final float scaleFactorStep = detector.getScaleFactor();
                                final float density = context.getResources().getDisplayMetrics().density;
                                onPinchScale(lastRawFocusX, lastRawFocusY, scaleFactorStep, density, nativePtr);
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onScale", err);
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onScale", t);
                            }
                            return true;
                        }

                        @Override
                        public void onScaleEnd(ScaleGestureDetector detector) {
                            try {
                                final float scaleFactorStep = detector.getScaleFactor();
                                final float density = context.getResources().getDisplayMetrics().density;
                                onPinchEnd(lastRawFocusX, lastRawFocusY, scaleFactorStep, density, nativePtr);
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onScaleEnd", err);
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onScaleEnd", t);
                            } finally {
                                pinchGestureActive = false;
                            }
                        }
                    });
        }

        @Override
        public boolean onTouch(View v, MotionEvent event) {
            final int action = event.getActionMasked();
            if (action == MotionEvent.ACTION_DOWN) {
                lastSingleTapHandled = false; // Reset for new touch sequence
            }
            // Calculate raw focus coordinates from current pointers (used for pinching)
            int pointerCount = 0;
            try {
                pointerCount = event.getPointerCount();
                if (pointerCount > 0) {
                    float sumX = 0f;
                    float sumY = 0f;
                    for (int i = 0; i < pointerCount; ++i) {
                        sumX += event.getRawX(i);
                        sumY += event.getRawY(i);
                    }
                    lastRawFocusX = sumX / pointerCount;
                    lastRawFocusY = sumY / pointerCount;
                }
            } catch (Throwable t) {
                // Ignore
            }
            updatePinchingStateFromPointerCount(pointerCount);
            final boolean handledGesture = detector.onTouchEvent(event);
            final boolean handledScale = scaleDetector.onTouchEvent(event);
            
            // Determine if we should consume the event
            boolean handled = false;
            if (action == MotionEvent.ACTION_UP) {
                // For ACTION_UP, check if ISingleTapHandler was found
                // (onSingleTapUp() was called inside detector.onTouchEvent() and set lastSingleTapHandled)
                if (handledScale) {
                    // Only consume if it's actually a pinch gesture (not just scale detector returning true)
                    // Check if we're actually pinching
                    if (pinching || pinchGestureActive) {
                        handled = true;
                    } else {
                        // Scale detector returned true but not actually pinching, check single tap
                        handled = lastSingleTapHandled;
                    }
                } else if (handledGesture) {
                    handled = lastSingleTapHandled;
                }
            } else {
                // For other actions (DOWN, MOVE, etc.), consume if gesture detector handled it
                if (handledScale) {
                    // Only consume if it's actually a pinch gesture (not just scale detector returning true)
                    // Check if we're actually pinching
                    if (pinching || pinchGestureActive) {
                        handled = true;
                    } else {
                        // Scale detector returned true but not actually pinching, don't consume
                        // This allows JUCE's standard event handling to process the event
                        handled = false;
                    }
                } else if (handledGesture) {
                    // For ACTION_DOWN, we don't know yet if ISingleTapHandler will be found
                    // So we don't consume the event, allowing JUCE's standard event handling
                    if (action == MotionEvent.ACTION_DOWN) {
                        handled = false;
                    } else {
                        // For MOVE, etc., consume if gesture detector handled it
                        handled = handledGesture;
                    }
                }
            }

            if ((action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) && dragging) {
                try {
                    final float density = context.getResources().getDisplayMetrics().density;
                    final float endRawX = event.getRawX();
                    final float endRawY = event.getRawY();
                    final float stepDeltaX = endRawX - lastRawX;
                    final float stepDeltaY = endRawY - lastRawY;
                    sendDragToNative(PHASE_END,
                            dragStartRawX, dragStartRawY,
                            endRawX, endRawY,
                            stepDeltaX, stepDeltaY,
                            density, nativePtr);
                } catch (UnsatisfiedLinkError err) {
                    Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onDragEnd", err);
                } catch (Throwable t) {
                    Log.e("NotifierGestureFromAndroid", "Throwable in onDragEnd", t);
                } finally {
                    dragging = false;
                    dragStartRawX = dragStartRawY = 0f;
                    lastRawX = lastRawY = 0f;
                }
            }

            return handled;
        }

        private void ensurePinchingForScale() {
            if (!pinching) {
                pinching = true;
                terminateDragDueToPinch();
            }
        }

        private void updatePinchingStateFromPointerCount(int pointerCount) {
            final boolean multiTouch = pointerCount >= MIN_POINTER_COUNT_FOR_PINCH;
            if (multiTouch) {
                if (!pinching) {
                    pinching = true;
                    terminateDragDueToPinch();
                }
            } else if (!pinchGestureActive) {
                pinching = false;
            }
        }

        private void terminateDragDueToPinch() {
            if (!dragging) {
                return;
            }
            try {
                final float density = context.getResources().getDisplayMetrics().density;
                sendDragToNative(PHASE_END,
                        dragStartRawX, dragStartRawY,
                        lastRawX, lastRawY,
                        0f, 0f,
                        density, nativePtr);
            } catch (UnsatisfiedLinkError err) {
                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in terminateDragDueToPinch", err);
            } catch (Throwable t) {
                Log.e("NotifierGestureFromAndroid", "Throwable in terminateDragDueToPinch", t);
            } finally {
                dragging = false;
                dragStartRawX = dragStartRawY = 0f;
                lastRawX = lastRawY = 0f;
            }
        }

        void addRef() {
            refCount++;
        }

        void release() {
            refCount--;
        }

        int getRefCount() {
            return refCount;
        }

        void setNativePtr(long nativePtr) {
            this.nativePtr = nativePtr;
        }
    }
}
