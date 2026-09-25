#include "GesturePeerWatcher.h"
#if JUCE_ANDROID
#include <jni.h>

namespace juce {
// juce_core/native/juce_JNIHelpers_android.h で定義済み（GesturePeerWatcher.h
// のコメント参照。unity build内での重複includeを避けるため、宣言のみ借用する）
JNIEnv* getEnv() noexcept;
}  // namespace juce

namespace {
constexpr const char* kHelperClassName       = "com/juceture/android/NotifierGestureFromAndroid";
constexpr const char* kAttachMethodName      = "attach";
constexpr const char* kDetachMethodName      = "detach";
constexpr const char* kAttachDetachSignature = "(Landroid/view/View;J)V";

bool isTemporaryWindow(const juce::ComponentPeer& peer)
{
    return (peer.getStyleFlags() & juce::ComponentPeer::windowIsTemporary) != 0;
}
}  // namespace

GesturePeerWatcher& GesturePeerWatcher::getInstance()
{
    static GesturePeerWatcher instance;
    return instance;
}

GesturePeerWatcher::GesturePeerWatcher()
{
}

GesturePeerWatcher::~GesturePeerWatcher()
{
    stopTimer();

    if (auto* env = juce::getEnv())
    {
        for (auto& [peer, viewObj] : attachedPeerViews)
        {
            juce::ignoreUnused(peer);
            env->DeleteGlobalRef(static_cast<jobject>(viewObj));
        }
    }
}

bool GesturePeerWatcher::reconcile()
{
    auto* env = juce::getEnv();
    if (env == nullptr)
        return false;

    const jclass helperClass = env->FindClass(kHelperClassName);
    if (helperClass == nullptr)
        return false;

    const jmethodID attachMethod = env->GetStaticMethodID(helperClass, kAttachMethodName, kAttachDetachSignature);
    const jmethodID detachMethod = env->GetStaticMethodID(helperClass, kDetachMethodName, kAttachDetachSignature);
    if (attachMethod == nullptr || detachMethod == nullptr)
    {
        env->DeleteLocalRef(helperClass);
        return false;
    }

    // 現在生存している、一時的でないピアを集める
    std::map<juce::ComponentPeer*, jobject> livePeers;
    const int numPeers = juce::ComponentPeer::getNumPeers();
    for (int i = 0; i < numPeers; ++i)
    {
        auto* peer = juce::ComponentPeer::getPeer(i);
        if (peer == nullptr || isTemporaryWindow(*peer))
            continue;

        auto* viewObj = static_cast<jobject>(peer->getNativeHandle());
        if (viewObj == nullptr)
            continue;

        livePeers[peer] = viewObj;
    }

    // 新規ピアへアタッチする
    for (const auto& [peer, viewObj] : livePeers)
    {
        if (attachedPeerViews.find(peer) != attachedPeerViews.end())
            continue;

        env->CallStaticVoidMethod(helperClass, attachMethod, viewObj, static_cast<jlong>(0));
        attachedPeerViews.emplace(peer, static_cast<void*>(env->NewGlobalRef(viewObj)));
    }

    // 破棄されたピアをデタッチする
    for (auto it = attachedPeerViews.begin(); it != attachedPeerViews.end();)
    {
        if (livePeers.find(it->first) == livePeers.end())
        {
            auto* globalViewObj = static_cast<jobject>(it->second);
            env->CallStaticVoidMethod(helperClass, detachMethod, globalViewObj, static_cast<jlong>(0));
            env->DeleteGlobalRef(globalViewObj);
            it = attachedPeerViews.erase(it);
        }
        else
        {
            ++it;
        }
    }

    env->DeleteLocalRef(helperClass);

    return !attachedPeerViews.empty();
}

void GesturePeerWatcher::startWatching()
{
    if (!isTimerRunning())
        startTimer(kPeerWatchIntervalMs);
}

void GesturePeerWatcher::timerCallback()
{
    reconcile();
}
#endif
