# Factorial
入力された値nの階乗をマルチスレッドで高速に計算するプログラム

# アルゴリズム
指定されたスレッド数分に入力値を分割して、それぞれで階乗を計算後、統合して出力しています

# Use Library
gmpを使っているため、コンパイル前に以下のコマンドを実行し、ライブラリをインストールしてください

```bash
apt install  libgmp3-dev
```