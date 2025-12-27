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
    private static native void onSingleTap(float xPx, float yPx, float density);

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
        if (view == null)
            return;

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
                                onSingleTap(e.getRawX(), e.getRawY(), density);
                            } catch (UnsatisfiedLinkError err) {
                                Log.e("NotifierGestureFromAndroid", "UnsatisfiedLinkError in onSingleTap", err);
                            } catch (Throwable t) {
                                Log.e("NotifierGestureFromAndroid", "Throwable in onSingleTap", t);
                            }
                            return true;
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
                                onPinchStart(lastRawFocusX, lastRawFocusY, detector.getScaleFactor(), density,
                                        nativePtr);
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
                                final float density = context.getResources().getDisplayMetrics().density;
                                onPinchScale(lastRawFocusX, lastRawFocusY, detector.getScaleFactor(), density,
                                        nativePtr);
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
                                final float density = context.getResources().getDisplayMetrics().density;
                                onPinchEnd(lastRawFocusX, lastRawFocusY, detector.getScaleFactor(), density, nativePtr);
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
            final boolean handled = handledGesture || handledScale;

            final int action = event.getActionMasked();
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
