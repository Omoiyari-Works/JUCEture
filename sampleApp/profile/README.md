# プロファイルをとるためにやること

ここではAndroidアプリのプロファイルをとるためにやることを説明する。

## 事前準備

### プロファイル設定ファイル(`*.pbtxt`)ファイルを準備する

ここでは`profileConfig.pbtxt`を一貫して使用する。
記述の仕方などはあまり詳しくないのでここでは詳述は避ける。
ネットや生成AIに聞けばそれなりの答えがもらえる。

### トレースを仕込む

これをすると、トレースビュー(CPUやプロセス、スレッドごとに占有してるところがグラフィカルにわかるあのチャート)に、
仕込んだ場所が表示される。
自分で実装した場所の調査には有用だが、グラフィック関連は特に何も仕込まなくてもスライスが出たりするので、
仕込まなくても良かったりするので、要否は一度考えよう。

仕込む場合は仕込む前に調査が必要。トレースの仕込み方が変わるので。

#### スマホ本体がatrace がappに対応しているか調べる

```shell
$ adb shell atrace --list_categories
        gfx  - Graphics                 
       input - Input
        view - View System
     webview - WebView
        (略)
```
結果の出力の左側に`app`があるか確認する。
appがあれば、appに対応している。

#### appに対応している場合

##### プロジェクトに依存関係追加

`app/build.gradle`に以下を追加する。

```
        implementation "androidx.core:core:1.12.0"
        implementation "androidx.tracing:tracing-perfetto:1.0.0"
        implementation "androidx.tracing:tracing-perfetto-binary:1.0.0"
        implementation "androidx.tracing:tracing:1.3.0"
```

`tracing-perfetto`, `tracing-perfetto-binary`はなくても良いかも知れない。

##### ソースの修正

`Trace.beginSection()`と`Trace.endSection()`を、
解析したい箇所の前後に追加する。

**こちらはJavaであることに注意。**

```java
    Trace.beginSection("任意の名前");
    //解析したい処理。
    Trace.endSection();
```

#### appに対応してない場合
perfetto SDK(C++)を組み込む必要がある。

##### perfettoのソースを取得

なんでも良いが、`git submodule`コマンドが良いだろう。git cloneでも良いよ。
```shell
git submodule add -b releases/v53.x https://github.com/google/perfetto.git ${任意のディレクトリ}
git submodule update --init --recursive
```

##### CMakeLists.txt を変更
以下をアプリのターゲットの記述の前に入れる。

```cmake
include_directories("../../../external/perfetto/sdk")
add_library(perfetto STATIC ../../../external/perfetto/sdk/perfetto.cc)

# (略)

target_link_libraries( ${BINARY_NAME}
    # (略)
    perfetto # 末尾じゃなくても良いと思う
)
```

##### アプリのソースを変更

**こちらはJavaではなくC++であることに注意。**

初期化処理
```cpp
PERFETTO_DEFINE_CATEGORIES(
    perfetto::Category("view").SetDescription("NotifierGestureFromOS events")
);

//メインのコンストラクタとか
    perfetto::TracingInitArgs args;
    args.backends |= perfetto::kInProcessBackend;
    args.backends |= perfetto::kSystemBackend;
    perfetto::Tracing::Initialize(args);
    perfetto::TrackEvent::Register();
```

仕込みたいところ
```cpp
PERFETTO_DEFINE_CATEGORIES(
    perfetto::Category("view").SetDescription("NotifierGestureFromOS events")
);
PERFETTO_TRACK_EVENT_STATIC_STORAGE();

//どこかの関数
    TRACE_EVENT("view", "onSingleTap");
```

こうすることで、仕込んだところのスコープを抜けるまでトレースしてくれる。

```cpp
PERFETTO_DEFINE_CATEGORIES(
    perfetto::Category("view").SetDescription("NotifierGestureFromOS events")
);
```
この部分は共通ヘッダがあればそこに定義するのが吉。
あと``perfetto::Category()``の引数と`TRACE_EVENT()`の第一引数は、
前述の`adb shell atrace --list_categories`の結果に載ってる、
かつ前述のプロファイル設定ファイルで記述がないとダメらしい。
正確かつ詳細にどう書けばよいかは不明。（未調査）

### 仕込んだソフトをインストール

ソフトをインストールする。ここでは詳細は説明しない。

### プロファイル設定ファイルをスマホにコピーする

以下コマンドでコピーする。
```shell
adb push profileConfig.pbtxt /data/misc/perfetto-configs/
```

### スマホでシステムトレースを有効にする

システムトレース内にトレース許可するような項目があるので、
それを許可する。
Xiaomi Mi lite 5gの場合は「トレースを記録」という項目を許可した。

## キャプチャ実施

### キャプチャ開始する

以下コマンドでキャプチャ開始。
```shell
adb shell perfetto --txt -c /data/misc/perfetto-configs/sample.pbtxt -o /data/misc/perfetto-traces/mytrace.perfetto-trace
```

うまくいくとこんなログが表示される。

```shell
C:\project\AndroidGUISampleProject [main ↑6 +4 ~4 -0 !]> adb shell perfetto --txt -c /data/misc/perfetto-configs/sample.pbtxt -o /data/misc/perfetto-traces/mytrace.perfetto-trace
* daemon not running; starting now at tcp:5037
* daemon started successfully
Warning: No PTY. CTRL+C won't gracefully stop the trace. If you are running perfetto via adb shell, use the -tt arg (adb shell -t perfetto ...) or consider using the helper script tools/record_android_trace from the Perfetto repository.

[110.186]     perfetto_cmd.cc:999 Connected to the Perfetto traced service, TTL: 10s
```

今の設定ファイルだと、10秒後にキャプチャが自動的に終了するので、それまでに次のアプリを動作させるを実施すること。

### アプリを動作させる

アプリを起動し、プロファイルしたいところを通るように動作させる。

### キャプチャをスマホからコピーする

以下コマンドでコピーする。

```shell
adb pull /data/misc/perfetto-traces/mytrace.perfetto-trace
```

コピー先を指定したいときは、以下のように引数を追加する。

```shell
adb pull /data/misc/perfetto-traces/mytrace.perfetto-trace /path/to
```

## 解析

WEB状に解析用のサイトがある。
[ui.perfetto.dev](https://ui.perfetto.dev/) を開き、  
そのページに`*.perfetto-trace`ファイルをドラッグアンドドロップか、  
`Open trace file`で渡す。

あとは中身を見るだけ。

