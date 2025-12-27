package com.yourcompany.androidguisampleproject;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.graphics.Color;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsCompat;

/**
 * JUCEアプリケーション用のカスタムActivity
 * エッジツーエッジ対応とシステムバーのインセット情報をネイティブ側に渡します
 */
public class EdgeToEdgeActivity extends Activity {

    // ネイティブメソッド（C++で実装）
    // 注意：JUCEのネイティブライブラリが既にロードされているので、
    // ここでSystem.loadLibrary()を呼ぶ必要はありません
    private static native void nativeSetSystemBarInsets(int top, int bottom, int left, int right);

    private boolean isEdgeToEdgeSetup = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // エッジツーエッジの設定はonWindowFocusChanged()で行う
        // （DecorViewが初期化されてから）
    }

    /**
     * エッジツーエッジを設定し、システムバーのインセットをネイティブ側に通知
     */
    private void setupEdgeToEdge() {
        Window window = getWindow();

        // ウィンドウをエッジツーエッジに設定
        WindowCompat.setDecorFitsSystemWindows(window, false);

        // API 30以降の場合、システムバーの動作を調整
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowInsetsController controller = window.getInsetsController();
            if (controller != null) {
                // システムバーを透過的に表示
                controller.setSystemBarsBehavior(
                        WindowInsetsController.BEHAVIOR_DEFAULT);
            }
        }

        // ステータスバーとナビゲーションバーを透過
        window.setStatusBarColor(Color.TRANSPARENT);
        window.setNavigationBarColor(Color.TRANSPARENT);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            window.setNavigationBarContrastEnforced(false);
            window.setStatusBarContrastEnforced(false);
        }

        // WindowInsetsのリスナーを設定
        View decorView = window.getDecorView();
        ViewCompat.setOnApplyWindowInsetsListener(decorView, (v, windowInsets) -> {
            // 各種インセットを個別に取得
            androidx.core.graphics.Insets systemInsets = windowInsets.getInsets(
                    WindowInsetsCompat.Type.systemBars());
            androidx.core.graphics.Insets statusInsets = windowInsets.getInsets(
                    WindowInsetsCompat.Type.statusBars());
            androidx.core.graphics.Insets navInsets = windowInsets.getInsets(
                    WindowInsetsCompat.Type.navigationBars());
            androidx.core.graphics.Insets cutoutInsets = windowInsets.getInsets(
                    WindowInsetsCompat.Type.displayCutout());

            // デバッグ：各インセット値をログに出力
            float density = getResources().getDisplayMetrics().density;
            android.util.Log.i("EdgeToEdgeActivity",
                    String.format("SystemBars (px): T:%d B:%d | (dp): T:%d B:%d",
                            systemInsets.top, systemInsets.bottom,
                            (int) (systemInsets.top / density), (int) (systemInsets.bottom / density)));

            // StatusBarのみを使用（DisplayCutoutを除外）
            androidx.core.graphics.Insets insets = androidx.core.graphics.Insets.of(
                    systemInsets.left,
                    systemInsets.top,
                    systemInsets.right,
                    systemInsets.bottom);

            int topDp = (int) (insets.top / density);
            int bottomDp = (int) (insets.bottom / density);
            int leftDp = (int) (insets.left / density);
            int rightDp = (int) (insets.right / density);

            android.util.Log.i("EdgeToEdgeActivity",
                    String.format("Final Insets: (px) T:%d B:%d | (dp) T:%d B:%d | Density: %.2f",
                            insets.top, insets.bottom, topDp, bottomDp, density));

            // DP値に変換してネイティブ側に通知
            try {
                nativeSetSystemBarInsets(
                        topDp,
                        bottomDp,
                        leftDp,
                        rightDp);
            } catch (UnsatisfiedLinkError e) {
                // ネイティブライブラリがまだロードされていない可能性
                android.util.Log.w("EdgeToEdgeActivity", "Native method not available: " + e.getMessage());
            }

            // インセットを消費せずに返す
            return windowInsets;
        });

        // 初回のインセット適用を要求
        decorView.post(() -> {
            decorView.requestApplyInsets();
        });
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if (hasFocus) {
            // 初回のみエッジツーエッジを設定
            if (!isEdgeToEdgeSetup) {
                setupEdgeToEdge();
                isEdgeToEdgeSetup = true;
            } else {
                // フォーカスが戻った時にインセットを再取得
                getWindow().getDecorView().requestApplyInsets();
            }
        }
    }
}
