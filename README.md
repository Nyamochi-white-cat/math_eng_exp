# 数理工学実験

数理工学実験で使用するプログラムと実験結果をまとめるリポジトリです。

## 環境

- C++14 に対応したコンパイラ（Makefile の既定値は `clang++`）
- Eigen 3
- 現在の Makefile は Eigen のヘッダを `/opt/homebrew/include/eigen3` から読み込みます。別の場所にインストールされている場合は、`nla/Makefile` の `CXXFLAGS` を環境に合わせて変更してください。