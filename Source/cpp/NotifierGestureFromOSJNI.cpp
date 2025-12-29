#include "SingleTap/SingleTapDetector.h"
#include "Drag/DragDetector.h"
#include "Pinch/PinchDetector.h"
#include "GestureMediator.h"
#include <JuceHeader.h>
#if JUCE_ANDROID
#include <jni.h>
#include <juce_core/native/juce_JNIHelpers_android.h>
#include <mutex>
#include <memory>

// Detectorインスタンス（デフォルトでGestureMediator::getInstance()を使用）
static std::unique_ptr<SingleTapDetector> gSingleTapDetector;
static std::unique_ptr<DragDetector> gDragDetector;
static std::unique_ptr<PinchDetector> gPinchDetector;

// Viewへのattach状態管理
static bool gViewAttached = false;
static std::mutex gAttachMutex;

// Detector初期化状態管理
static bool gDetectorsInitialized = false;
static std::mutex gDetectorInitMutex;

// 初期化処理：Viewにgesture listenerをアタッチ
bool attachGestureListenerOnce()
{
    std::lock_guard<std::mutex> lock(gAttachMutex);
    if (gViewAttached)
    {
        return true;
    }

    auto* peer = juce::ComponentPeer::getPeer(0);
    if (peer == nullptr)
    {
        return false;
    }

    if (auto* env = juce::getEnv())
    {
        auto viewObj = static_cast<jobject>(peer->getNativeHandle());
        if (viewObj == nullptr)
        {
            return false;
        }

        const jclass helperClass = env->FindClass("com/juceture/android/NotifierGestureFromAndroid");
        if (helperClass == nullptr)
        {
            return false;
        }

        const jmethodID attachMethod = env->GetStaticMethodID(helperClass, "attach", "(Landroid/view/View;J)V");
        if (attachMethod == nullptr)
        {
            return false;
        }

        // nativePtrは0で良い（実際には使われていない）
        env->CallStaticVoidMethod(helperClass, attachMethod, viewObj, static_cast<jlong>(0));
        gViewAttached = true;
        return true;
    }
    return false;
}

void initializeDetectors()
{
    std::lock_guard<std::mutex> lock(gDetectorInitMutex);
    if (gDetectorsInitialized)
        return;

    // デフォルト引数（nullptr）により、GestureMediator::getInstance()が使用される
    gSingleTapDetector = std::make_unique<SingleTapDetector>();
    gDragDetector = std::make_unique<DragDetector>();
    gPinchDetector = std::make_unique<PinchDetector>();
    gDetectorsInitialized = true;
}

extern "C"
{
    JNIEXPORT jboolean JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onSingleTap(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat rawX, jfloat rawY,
        jfloat density)
    {
        juce::ignoreUnused(density);
        if (gSingleTapDetector != nullptr)
        {
            bool result = gSingleTapDetector->onRawInput(rawX, rawY);
            return result ? JNI_TRUE : JNI_FALSE;
        }
        return JNI_FALSE;
    }

    JNIEXPORT void JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onDragStart(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat startRawX, jfloat startRawY,
        jfloat currentRawX, jfloat currentRawY, jfloat deltaX, jfloat deltaY,
        jfloat density, jlong /*ownerPtrLong*/)
    {
        juce::ignoreUnused(density);

        if (gDragDetector != nullptr)
        {
            gDragDetector->onDragStartRaw(startRawX, startRawY, currentRawX,
                                          currentRawY, deltaX, deltaY);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onDragMove(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat startRawX, jfloat startRawY,
        jfloat currentRawX, jfloat currentRawY, jfloat deltaX, jfloat deltaY,
        jfloat density, jlong /*ownerPtrLong*/)
    {
        juce::ignoreUnused(density);

        if (gDragDetector != nullptr)
        {
            gDragDetector->onDragMoveRaw(startRawX, startRawY, currentRawX,
                                         currentRawY, deltaX, deltaY);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onDragEnd(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat startRawX, jfloat startRawY,
        jfloat currentRawX, jfloat currentRawY, jfloat deltaX, jfloat deltaY,
        jfloat density, jlong /*ownerPtrLong*/)
    {
        juce::ignoreUnused(density);

        if (gDragDetector != nullptr)
        {
            gDragDetector->onDragEndRaw(startRawX, startRawY, currentRawX,
                                        currentRawY, deltaX, deltaY);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onPinchStart(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat focusXInView,
        jfloat focusYInView, jfloat scaleFactorStep, jfloat density,
        jlong /*ownerPtrLong*/)
    {
        juce::ignoreUnused(density);

        if (gPinchDetector != nullptr)
        {
            gPinchDetector->onPinchStartRaw(focusXInView, focusYInView, scaleFactorStep);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onPinchScale(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat focusXInView,
        jfloat focusYInView, jfloat scaleFactorStep, jfloat density,
        jlong /*ownerPtrLong*/)
    {
        juce::ignoreUnused(density);

        if (gPinchDetector != nullptr)
        {
            gPinchDetector->onPinchScaleRaw(focusXInView, focusYInView, scaleFactorStep);
        }
    }

    JNIEXPORT void JNICALL
    Java_com_juceture_android_NotifierGestureFromAndroid_onPinchEnd(
        JNIEnv* /*env*/, jclass /*clazz*/, jfloat focusXInView,
        jfloat focusYInView, jfloat scaleFactorStep, jfloat density,
        jlong /*ownerPtrLong*/)
    {
        juce::ignoreUnused(density);

        if (gPinchDetector != nullptr)
        {
            gPinchDetector->onPinchEndRaw(focusXInView, focusYInView, scaleFactorStep);
        }
    }
}
#endif
