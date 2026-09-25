#pragma once

#if JUCE_ANDROID
#include <juce_gui_basics/juce_gui_basics.h>
#include <map>

/**
 * addToDesktop()で作られる全てのウィンドウ（ComponentPeer）に対して、
 * Java側のジェスチャー検出用OnTouchListener（NotifierGestureFromAndroid）が
 * アタッチされている状態を維持するクラス。
 *
 * JUCEにはピアの生成・破棄を通知する公開フックが無いため、Timerで定期的に
 * 生存ピア一覧と照合する（reconcile）方式を取る。一時的なウィンドウ
 * （PopupMenu等、windowIsTemporary）はアタッチ対象から除外する。
 *
 * 注意: juce_core/native/juce_JNIHelpers_android.h はインクルードガードが
 * 無く、JUCEtureのunity build（juceture.cpp）内で他の.cpp（例えば
 * NotifierGestureFromOSJNI.cpp）と重複includeするとマクロ再展開で
 * コンパイルエラーになる。そのためこのヘッダ・実装ファイルでは同ヘッダを
 * includeせず、アタッチ済みViewはjobjectの生ポインタ（void*として保持）を
 * 自前でグローバル参照管理する。
 */
class GesturePeerWatcher : public juce::Timer
{
  public:
    static GesturePeerWatcher& getInstance();

    // 現在生存している全ピアへのアタッチ状態を同期的に1回更新する。
    // 戻り値はアタッチ済みピアが1つ以上存在するかどうか。
    bool reconcile();

    // 以降、新規ピアの出現・破棄をポーリングで監視し続ける
    void startWatching();

    void timerCallback() override;

  private:
    GesturePeerWatcher();
    ~GesturePeerWatcher() override;

    // ピアごとにアタッチ済みJava View（jobjectへのグローバル参照）を保持
    std::map<juce::ComponentPeer*, void*> attachedPeerViews;

    static constexpr int kPeerWatchIntervalMs = 250;
};
#endif
