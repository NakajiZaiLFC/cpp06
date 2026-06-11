# Cpp06について。

## これはなに

- C++のCast手法を学ぶ課題
    - Ex00はstatic_cast
    - Ex01はReinterpret_cast
    - Ex02はDynamic_cast

## static_cast

### 変換対象

関連する型同士で値を変換する。

### 何を見るか

この型変換は文法的に意味があるかどうか。\
→実行時の値が型定義の範囲内かは検証しない。

```cpp
int n = 300;
char c = static_cast<char>(n);　// charは-128〜127しか入らない
```

このケースでは、`n`をcharにするとchar型の範囲は超えてしまうが文法的には問題ないので実行できてしまう。

#### 結果どうなるのか\

→nの下半分の8ビットだけ残して壊れる。

300を二進数にすると`100101100`になるが、下位8ビットだけなので`1`と`00101100`で分割された`44`が最終的に`c`に格納される。

### つまりどういうことか

`static_cast`は別に安全確認をしてくれるわけではない。\
→overflowやunderflowしたら自分で対処しろ（＝impossibleと表示する）ということ。

### ケーススタディ

```C++
./convert "42.0f" 
```

このように、文字列1つから出力はchar, int, float, doubleの4種類の出力をするために何段階の処理をする必要があるか。

1. 文字列の型判定
2. 型変換
3. 変換した数値を残りの3つに`static_cast`して出力\
   →この時、表示できない、intに入りきらないならimpossibleを出力

今回のエクササイズの仕様

- `ScalarConverter`クラスを書いて、convertという静的メソッドをその中に記述する。
- コマンドライン引数が2じゃなければエラー
- なんの文字があること＝その型と特定できるか？
    - Char
        - -128から127の間
    - Int
        - 小数点以下は切り捨て
        - 32ビットの数字だけ
        - キャストして、キャストする前の文字列と一致しなければそれはInt型以外の値ってことでは？
    - Float→ドットとf
        - これもキャストして、キャストする前の文字列と一致しなければそれはInt型以外の値ってことでは？
    - Double→ドット
- とりあえずよくわからなかったのでテスト駆動開発することにした。

# Claude Codeとの思考フレーム
1. まず、入力文字列（argv[1]の値）を判定する。
2. もしこれが、"NaN"や"+inf"などの特殊文字が来た場合はそれ専用の処理してReturn。
3. 判定した型に変換する。
4. 出力整形

このとき、判定と変換を混ぜない。

# 実装まとめ (2026-06-11)

## 設計パターン: パイプライン

```
detect(string) -> parse(native type) -> static_cast(4 types) -> print
```

- `M_detectType` で enum `LiteralType` を返す (T_CHAR, T_INT, T_FLOAT, T_DOUBLE, T_SPECIAL_LITERAL, T_INVALID)
- `M_convertToFormats` が switch で型別の変換関数にディスパッチ
- 各変換関数は `buildResult(double)` を呼ぶだけ (統一インターフェース)

## 統一インターフェース: makeXStr(double) -> string

全ての型変換を一度 double に落とし、4つのヘルパーで出力文字列を生成:

- `makeCharStr(double)` -- 範囲チェック(0-127) + isprint判定
- `makeIntStr(double)` -- NaN/overflow判定 (v != v || v > INT_MAX || v < INT_MIN)
- `makeFloatStr(double)` -- static_cast<float>してから文字列化
- `makeDoubleStr(double)` -- そのまま文字列化

`buildResult(double)` がこの4つを呼んで `ConversionResult` 構造体に詰める。
計算と出力の分離 -- 変換ロジックは文字列を返すだけ、出力は `M_printConversionResult` が担当。

## 踏んだバグと教訓

### 1. ポインタ比較 vs 文字列比較
`str.c_str() == "nan"` はポインタ比較。`str == "nan"` が正しい (std::string::operator==)。

### 2. M_isFloat の size_t vs char 比較
`str.size() - 1 == 'f'` は size_t と char('f' = 102) の比較。
正しくは `str[str.size() - 1] == 'f'`。

### 3. 精度の固定
`std::fixed << std::setprecision(1)` は常に小数1桁。3.14 が 3.1 になってしまう。
修正: デフォルト精度で出力し、ドットがなければ ".0" を付加。

### 4. 科学記法への誤った ".0" 付加
`1e-30` にドットがないので `.0` を付けて `1e-30.0` になった。
修正: `'e'` を含む場合は ".0" を付加しない。

### 5. 空文字/符号のみの通過
`""` や `"+"` が M_isInt を通過 (for ループが回らず true)。
修正: empty チェック + 符号後に文字がなければ false。

### 6. 数字なしリテラルの通過
`.f` が M_isFloat を通過 (ドットと f はあるが数字がない)。
修正: `hasDigit` フラグを追加。

## テストカテゴリ

1. char: 'a', '*'
2. int: 0, 42, -42
3. float: 42.0f, 3.14f, -4.2f
4. double: 42.0, 3.14, -4.2
5. 特殊リテラル: nan, nanf, +inf, -inf, +inff, -inff
6. 境界値: INT_MAX, INT_MAX+1, INT_MIN, INT_MIN-1
7. ASCII境界: 0(NUL), 31(ctrl), 32(space), 126(~), 127(DEL), 128(over)
8. 不正入力: "", "hello", "4.2.0", ".f", "42f"
