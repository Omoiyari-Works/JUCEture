package com.juceture.android;

import java.util.WeakHashMap;

import android.view.View;
import android.content.Context;

import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;

public final class NotifierGestureFromAndroid {
    private NotifierGestureFromAndroid() {
    }

    // Notify C++ of coordinates (View -> peer -> global -> getComponentAt in C++)
    // Returns true if ISingleTapHandler was found and handled, false otherwise
    private static native boolean onSingleTap(float xPx, float yPx, float density);

    // Notify C++ of long tap coordinates
    // Returns true if ILongTapHandler was found and handled, false otherwise
    private static native boolean onLongTap(float xPx, float yPx, float density);

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
            float scaleFactorStep, float scaleFactorStepX, float scaleFactorStepY, float density, long nativePtr);

    private static native void onPinchScale(float focusXInView, float focusYInView,
            float scaleFactorStep, float scaleFactorStepX, float scaleFactorStepY, float density, long nativePtr);

    private static native void onPinchEnd(float focusXInView, float focusYInView,
            float scaleFactorStep, float scaleFactorStepX, float scaleFactorStepY, float density, long nativePtr);

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
        private int refCount = 0;
        private final Context context;
        private long nativePtr = 0L;
        private boolean dragging = false;
        private boolean pinching = false;
        private float dragStartRawX = 0f;
        private float dragStartRawY = 0f;
        private float lastRawX = 0f;
        private float lastRawY = 0f;
        private float lastRawFocusX = 0f;
        private float lastRawFocusY = 0f;
        private float lastPinchSpan = 0f;
        private float lastPinchSpanX = 0f;
        private float lastPinchSpanY = 0f;
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
                        public void onLongPress(MotionEvent e) {
                            try {
                                // Pass screen coordinates (raw px) and density (move to top-level local in C++)
                                final float density = context.getResources().getDisplayMetrics().density;
                                onLongTap(e.getRawX(), e.getRawY(), density);
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onLongPress", err);
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onLongPress", t);
                            }
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
        }

        /**
         * Calculate the Euclidean distance between the first two pointers.
         * Returns 0 if fewer than two pointers are present.
         */
        private float calculateSpan(MotionEvent e) {
            if (e.getPointerCount() < 2) return 0f;
            final float dx = e.getRawX(0) - e.getRawX(1);
            final float dy = e.getRawY(0) - e.getRawY(1);
            return (float) Math.sqrt(dx * dx + dy * dy);
        }

        /**
         * Calculate the X-axis distance between the first two pointers.
         * Returns 0 if fewer than two pointers are present.
         */
        private float calculateSpanX(MotionEvent e) {
            if (e.getPointerCount() < 2) return 0f;
            return Math.abs(e.getRawX(0) - e.getRawX(1));
        }

        /**
         * Calculate the Y-axis distance between the first two pointers.
         * Returns 0 if fewer than two pointers are present.
         */
        private float calculateSpanY(MotionEvent e) {
            if (e.getPointerCount() < 2) return 0f;
            return Math.abs(e.getRawY(0) - e.getRawY(1));
        }

        /**
         * Calculate the midpoint (focus) between the first two pointers.
         * Returns the first pointer position if fewer than two pointers are present.
         */
        private float[] calculateFocus(MotionEvent e) {
            if (e.getPointerCount() < 2) {
                return new float[]{ e.getRawX(), e.getRawY() };
            }
            return new float[]{
                (e.getRawX(0) + e.getRawX(1)) / 2f,
                (e.getRawY(0) + e.getRawY(1)) / 2f
            };
        }

        @Override
        public boolean onTouch(View v, MotionEvent event) {
            final int action = event.getActionMasked();
            final int pointerCount = event.getPointerCount();
            final float density = context.getResources().getDisplayMetrics().density;

            if (action == MotionEvent.ACTION_DOWN) {
                lastSingleTapHandled = false; // Reset for new touch sequence
            }

            // --- Custom pinch tracking (replaces ScaleGestureDetector) ---
            // Pinch starts when a second finger touches down.
            // Pinch ends ONLY when a finger actually lifts (ACTION_POINTER_UP drops count to 1),
            // or the entire gesture is cancelled. Span distance never triggers pinch end.
            try {
                switch (action) {
                    case MotionEvent.ACTION_POINTER_DOWN:
                        if (pointerCount == MIN_POINTER_COUNT_FOR_PINCH) {
                            // Second finger just touched down: start pinch
                            terminateDragDueToPinch();
                            pinching = true;
                            lastPinchSpan = calculateSpan(event);
                            lastPinchSpanX = calculateSpanX(event);
                            lastPinchSpanY = calculateSpanY(event);
                            final float[] startFocus = calculateFocus(event);
                            lastRawFocusX = startFocus[0];
                            lastRawFocusY = startFocus[1];
                            onPinchStart(lastRawFocusX, lastRawFocusY,
                                    1.0f, 1.0f, 1.0f, density, nativePtr);
                        }
                        break;

                    case MotionEvent.ACTION_MOVE:
                        if (pinching && pointerCount >= MIN_POINTER_COUNT_FOR_PINCH) {
                            final float newSpan = calculateSpan(event);
                            final float newSpanX = calculateSpanX(event);
                            final float newSpanY = calculateSpanY(event);
                            // Scale factor relative to the previous frame.
                            // Guard against division by zero when fingers are at the same point.
                            final float scaleFactor = (lastPinchSpan > 1f) ? newSpan / lastPinchSpan : 1.0f;
                            final float rawScaleFactorX = (lastPinchSpanX > 1f) ? newSpanX / lastPinchSpanX : 1.0f;
                            final float rawScaleFactorY = (lastPinchSpanY > 1f) ? newSpanY / lastPinchSpanY : 1.0f;
                            // Suppress axis-specific scale when the pinch direction is perpendicular
                            // to that axis. ratioX = spanX/span = |cos(angle)|, which is near 0 for
                            // a vertical pinch and near 1 for a horizontal pinch (and vice versa for Y).
                            // Squaring the ratio makes the suppression stronger near the perpendicular
                            // direction while keeping the transition smooth and continuous.
                            // When span=0, both weights are 0, so scaleFactorX/Y stay at 1.0.
                            final float ratioX = (newSpan > 1f) ? Math.min(newSpanX / newSpan, 1.0f) : 0f;
                            final float ratioY = (newSpan > 1f) ? Math.min(newSpanY / newSpan, 1.0f) : 0f;
                            final float weightX = ratioX * ratioX;
                            final float weightY = ratioY * ratioY;
                            final float scaleFactorX = 1.0f + (rawScaleFactorX - 1.0f) * weightX;
                            final float scaleFactorY = 1.0f + (rawScaleFactorY - 1.0f) * weightY;
                            lastPinchSpan = newSpan;
                            lastPinchSpanX = newSpanX;
                            lastPinchSpanY = newSpanY;
                            final float[] moveFocus = calculateFocus(event);
                            lastRawFocusX = moveFocus[0];
                            lastRawFocusY = moveFocus[1];
                            onPinchScale(lastRawFocusX, lastRawFocusY,
                                    scaleFactor, scaleFactorX, scaleFactorY, density, nativePtr);
                        }
                        break;

                    case MotionEvent.ACTION_POINTER_UP:
                        // pointerCount is still the count BEFORE this pointer lifts.
                        // When it equals 2, the lift reduces active fingers to 1: end pinch.
                        if (pinching && pointerCount == MIN_POINTER_COUNT_FOR_PINCH) {
                            // Compute focus from the finger that is NOT lifting.
                            // getActionIndex() returns the index of the lifting pointer.
                            final int liftingIndex = event.getActionIndex();
                            final int remainingIndex = (liftingIndex == 0) ? 1 : 0;
                            final float endFocusX = event.getRawX(remainingIndex);
                            final float endFocusY = event.getRawY(remainingIndex);
                            onPinchEnd(endFocusX, endFocusY,
                                    1.0f, 1.0f, 1.0f, density, nativePtr);
                            pinching = false;
                            lastPinchSpan = 0f;
                            lastPinchSpanX = 0f;
                            lastPinchSpanY = 0f;
                        }
                        break;

                    case MotionEvent.ACTION_UP:
                    case MotionEvent.ACTION_CANCEL:
                        if (pinching) {
                            onPinchEnd(lastRawFocusX, lastRawFocusY,
                                    1.0f, 1.0f, 1.0f, density, nativePtr);
                            pinching = false;
                            lastPinchSpan = 0f;
                            lastPinchSpanX = 0f;
                            lastPinchSpanY = 0f;
                        }
                        break;

                    default:
                        break;
                }
            } catch (UnsatisfiedLinkError err) {
                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in pinch handling", err);
            } catch (Throwable t) {
                Log.e("NotifierGestureFromAndroid", "Throwable in pinch handling", t);
            }

            // --- GestureDetector: tap, long-tap, drag ---
            final boolean handledGesture = detector.onTouchEvent(event);

            // Determine if we should consume the event
            boolean handled = false;
            if (action == MotionEvent.ACTION_UP) {
                handled = lastSingleTapHandled;
            } else if (pinching) {
                handled = true;
            } else if (action != MotionEvent.ACTION_DOWN) {
                handled = handledGesture;
            }

            // Drag end on ACTION_UP / ACTION_CANCEL
            if ((action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) && dragging) {
                try {
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
