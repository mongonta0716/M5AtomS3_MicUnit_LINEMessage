# M5AtomS3 MicUnit LINE Message

M5AtomS3を使用して、GPIO2に割り込みが入った場合にLINEへメッセージを送信するプロジェクトです。

## 概要

このプロジェクトは、M5AtomS3デバイスでGPIO2ピンの割り込みを監視し、割り込みが検出されるとLINE Messaging APIを使用してLINEにメッセージを送信します。マイクユニットなどのセンサーをGPIO2に接続して、音や振動などの検出時に通知を受け取ることができます。

## 必要なもの

- M5AtomS3本体
- マイクユニットまたはGPIO2に接続可能なセンサー
- WiFi環境
- LINE Messaging APIのチャネルアクセストークン

## セットアップ方法

### 1. LINE Developers Consoleでの設定

1. [LINE Developers Console](https://developers.line.biz/console/)にアクセスします
2. 新しいプロバイダーを作成（または既存のものを選択）
3. 「Messaging API」チャネルを作成
4. チャネルアクセストークン（長期）を発行

### 2. コードの設定

`src/main.cpp`ファイル内の以下の部分を編集します：

```cpp
// WiFiとLINEの設定
const char* ssid = "YOUR_SSID";              // WiFiのSSID
const char* password = "YOUR_PASS";      // WiFiのパスワード

// 以下からLINEチャネルアクセストークンを取得する
// https://developers.line.biz/ja/
const char* accessToken = "YOUR_LINE_CHANNEL_ACCESS_TOKEN";  // LINEチャネルアクセストークン
```

### 3. ビルドとアップロード

PlatformIOを使用してプロジェクトをビルドし、M5AtomS3にアップロードします：

```bash
pio run -e m5atoms3 -t upload
```

## 使用方法

1. M5AtomS3にプログラムをアップロードします
2. マイクユニットまたはセンサーをGPIO2に接続します
3. M5AtomS3を起動すると、WiFiに接続し、起動メッセージがLINEに送信されます
4. GPIO2に割り込みが入ると（例：大きな音や振動を検出）、「Interrupt detected on GPIO2!」というメッセージがLINEに送信されます

## ハードウェア接続

- マイクユニットやセンサーのGND → M5AtomS3のGND
- マイクユニットやセンサーのVCC → M5AtomS3の3.3V/5V（センサーの仕様に合わせて）
- マイクユニットやセンサーの信号線 → M5AtomS3のGPIO2

## カスタマイズ

- 送信するメッセージの内容を変更したい場合は、`line.sendMessage()`の引数を編集します
- 割り込みの条件を変更したい場合は、`attachInterrupt()`の第3引数を変更します（FALLING, RISING, CHANGEなど）

## トラブルシューティング

- WiFiに接続できない場合は、SSIDとパスワードが正しいか確認してください
- LINEメッセージが送信されない場合は、アクセストークンが正しいか、有効期限が切れていないか確認してください
- 割り込みが検出されない場合は、センサーの接続とGPIO2の設定を確認してください

## ライセンス

MITライセンスの下で公開されています。詳細はLICENSEファイルを参照してください。