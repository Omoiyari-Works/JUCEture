# ベースイメージ: Eclipse Temurin JDK 17 (公式推奨のJDKディストリビューション)
FROM eclipse-temurin:17-jdk-noble

# メタデータ
LABEL maintainer="FlashMashApp Team"
LABEL description="Android build environment for JUCEture"

# ビルドに必要なツールをインストール
# JUCEのビルドには基本的なC++ビルドツールが必要な場合があるため build-essential を入れています
# gosuはentrypointスクリプトでユーザー切り替えに使用します
# ccacheはC++コンパイルを高速化するためのキャッシュツールです
RUN apt-get update && apt-get install -y \
    wget \
    unzip \
    curl \
    git \
    build-essential \
    ninja-build \
    gosu \
    ccache \
    && rm -rf /var/lib/apt/lists/*

# Gradleのインストール
ARG GRADLE_VERSION=8.13
RUN wget -q https://services.gradle.org/distributions/gradle-${GRADLE_VERSION}-bin.zip -O /tmp/gradle.zip \
    && unzip -q /tmp/gradle.zip -d /opt/gradle \
    && rm -f /tmp/gradle.zip

# 環境変数の設定
ENV JAVA_HOME=/opt/java/openjdk
ENV ANDROID_HOME=/opt/android-sdk
ENV GRADLE_HOME=/opt/gradle/gradle-${GRADLE_VERSION}
ENV PATH=${PATH}:${JAVA_HOME}/bin:${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${GRADLE_HOME}/bin

# ccacheの設定
# ccacheをコンパイラのラッパーとして使用
RUN ln -s /usr/bin/ccache /usr/local/bin/gcc \
    && ln -s /usr/bin/ccache /usr/local/bin/g++ \
    && ln -s /usr/bin/ccache /usr/local/bin/cc \
    && ln -s /usr/bin/ccache /usr/local/bin/c++ \
    && ln -s /usr/bin/ccache /usr/local/bin/clang \
    && ln -s /usr/bin/ccache /usr/local/bin/clang++

ENV CCACHE_DIR=/workspace/.ccache
ENV USE_CCACHE=1
ENV CCACHE_MAXSIZE=2G
ENV NDK_CCACHE=/usr/bin/ccache

# バージョン定義（build.gradleの内容に準拠）
# Command Line Toolsのバージョン (11076708 is typically the latest stable as of 2024/2025)
ARG CMDLINE_TOOLS_URL=https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip

# SDK Packages - プロジェクトの build.gradle に合わせています
ARG NDK_VERSION=29.0.14206865
ARG CMAKE_VERSION=3.22.1
ARG CMAKE_VERSION_2=4.1.2
ARG BUILD_TOOLS_VERSION=35.0.0
ARG COMPILE_SDK_VERSION=35

# Android SDK Command Line Tools のセットアップ
# cmdline-tools は $ANDROID_HOME/cmdline-tools/latest/bin/sdkmanager として配置する必要があります
RUN mkdir -p ${ANDROID_HOME}/cmdline-tools \
    && wget -q ${CMDLINE_TOOLS_URL} -O /tmp/cmdline-tools.zip \
    && unzip -q /tmp/cmdline-tools.zip -d ${ANDROID_HOME}/cmdline-tools \
    && mv ${ANDROID_HOME}/cmdline-tools/cmdline-tools ${ANDROID_HOME}/cmdline-tools/latest \
    && rm -f /tmp/cmdline-tools.zip

# ライセンス同意とパッケージインストール
# yes コマンドでライセンスに自動同意します
# cmakeは複数のバージョンを同時にインストール可能
RUN yes | sdkmanager --licenses \
    && sdkmanager \
    "platform-tools" \
    "platforms;android-${COMPILE_SDK_VERSION}" \
    "build-tools;${BUILD_TOOLS_VERSION}" \
    "ndk;${NDK_VERSION}" \
    "cmake;${CMAKE_VERSION}" \
    "cmake;${CMAKE_VERSION_2}" \
    && rm -rf ${ANDROID_HOME}/cmdline-tools/latest/.installationLog

# ユーザー作成
ARG USER_NAME=android
ARG USER_UID=1001
ARG USER_GID=1001

RUN groupadd --gid ${USER_GID} ${USER_NAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USER_NAME}

# SDKとGradleの権限変更
RUN chown -R ${USER_NAME}:${USER_NAME} ${ANDROID_HOME} \
    && chown -R ${USER_NAME}:${USER_NAME} /opt/gradle

# 作業ディレクトリの設定
WORKDIR /workspace
RUN chown -R ${USER_NAME}:${USER_NAME} /workspace

# Gradleホームディレクトリの作成
# ボリュームマウントされる前にディレクトリを作成し、権限を設定します
RUN mkdir -p /home/${USER_NAME}/.gradle \
    && chown -R ${USER_NAME}:${USER_NAME} /home/${USER_NAME}/.gradle

# ccacheディレクトリの準備
RUN mkdir -p /workspace/.ccache \
    && chown -R ${USER_NAME}:${USER_NAME} /workspace/.ccache

# デバッグ用キーストアの配置
# 一般ユーザーのホームディレクトリに配置します
RUN mkdir -p /home/${USER_NAME}/.android
COPY debug.keystore /home/${USER_NAME}/.android/debug.keystore
RUN chown -R ${USER_NAME}:${USER_NAME} /home/${USER_NAME}/.android

# NDKの場所を環境変数で設定（local.propertiesのndk.dirの代わり）
ENV ANDROID_NDK_HOME=${ANDROID_HOME}/ndk/${NDK_VERSION}

# Entrypointスクリプトのコピーと実行権限の付与
COPY entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh

# 環境変数の設定（entrypointスクリプトで使用）
ENV USER_NAME=${USER_NAME}
ENV USER_UID=${USER_UID}
ENV USER_GID=${USER_GID}

# Entrypointの設定
# コンテナ起動時にrootで実行され、entrypointスクリプトが権限を修正してから一般ユーザーに切り替えます
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]

# デフォルトコマンド
# コンテナ起動時に gradle のバージョンを表示して動作確認とします
CMD ["gradle", "-v"]
