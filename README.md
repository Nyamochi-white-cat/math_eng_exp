# 数理工学実験

数理工学実験で使用するプログラムと実験結果をまとめるリポジトリです。

## 環境

- C++14 に対応したコンパイラ（Makefile の既定値は `clang++`）
- Eigen 3
- 現在の Makefile は Eigen のヘッダを `/opt/homebrew/include/eigen3` から読み込みます。別の場所にインストールされている場合は、`make EIGEN3_INCLUDE=...` のように指定してください。

## nla のビルドと実行

`nla/` ディレクトリで次のコマンドを使います。

```sh
make                    # build/にassignment1とassignment2をビルド
make run                # 実行済みでなければ両方を実行（build/.cacheで管理）
make run-assignment1    # assignment1を直接実行
make run-assignment2    # assignment2を直接実行
```

実行ファイルは `build/` に出力されます。`make run` は `build/.cache/` の印で実行済みかを管理し、ソースや実行ファイルが更新されるまで再実行しません。`make run-assignment1` と `make run-assignment2` は毎回直接実行します。Eigen 3 のヘッダが標準の場所にない場合は、たとえば `make EIGEN3_INCLUDE=/usr/include/eigen3` のように指定できます。実験結果は `data/` に出力されます。`make clean` は `build/`（実行ファイルとキャッシュ）と生成データを削除します。
