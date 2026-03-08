#!/bin/bash
set -e

# 環境変数からユーザー情報を取得（デフォルト値: android:1001:1001）
USER_NAME=${USER_NAME:-android}
USER_UID=${USER_UID:-1001}
USER_GID=${USER_GID:-1001}

# rootで実行されている場合のみ権限を修正
if [ "$(id -u)" = "0" ]; then
    # /workspaceの権限を修正
    if [ -d /workspace ]; then
        chown -R ${USER_UID}:${USER_GID} /workspace || true
    fi

    # /home/android/.gradleの権限を修正（ボリュームマウントされている場合）
    # ディレクトリが存在しない場合は作成し、存在する場合は権限を修正
    mkdir -p /home/${USER_NAME}/.gradle
    chown -R ${USER_UID}:${USER_GID} /home/${USER_NAME}/.gradle || true

    # 一般ユーザーに切り替えてコマンドを実行
    exec gosu ${USER_NAME} "$@"
else
    # 既に一般ユーザーで実行されている場合はそのまま実行
    exec "$@"
fi
