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

# C++98 基本型とサイズ保証 (2026-06-12)

## 規格上のサイズ保証 vs LP64 (x86-64 Linux) 実測値

| 型          | 規格の最低保証       | LP64 実測              | 閾値ヘッダ |
| ----------- | -------------------- | ---------------------- | ---------- |
| char        | 1 byte               | 1 byte (8 bit)         | <climits>  |
| short       | >= 16 bit            | 2 bytes                | <climits>  |
| int         | >= 16 bit            | 4 bytes (32 bit)       | <climits>  |
| long        | >= 32 bit            | 8 bytes (64 bit)       | <climits>  |
| float       | >= 6 decimal digits  | 4 bytes (FLT_DIG=6)    | <cfloat>   |
| double      | >= 10 decimal digits | 8 bytes (DBL_DIG=15)   | <cfloat>   |
| long double | >= 10 decimal digits | 16 bytes (LDBL_DIG=18) | <cfloat>   |

規格の順序保証: `1 == sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long)`

注意: LP64 (Linux/macOS) では long=8bytes だが LLP64 (Windows) では long=4bytes。

## <climits> 主要定数 (LP64 実測値)

| 定数                | 値                                         | 意味                  |
| ------------------- | ------------------------------------------ | --------------------- |
| CHAR_BIT            | 8                                          | 1 byte のビット数     |
| CHAR_MIN / CHAR_MAX | -128 / 127                                 | char の範囲           |
| SHRT_MIN / SHRT_MAX | -32768 / 32767                             | short の範囲          |
| INT_MIN / INT_MAX   | -2147483648 / 2147483647                   | int の範囲            |
| UINT_MAX            | 4294967295                                 | unsigned int の最大値 |
| LONG_MIN / LONG_MAX | -9223372036854775808 / 9223372036854775807 | long の範囲           |

## <cfloat> 主要定数

| 定数       | float    | double    | 意味                         |
| ---------- | -------- | --------- | ---------------------------- |
| *_DIG      | 6        | 15        | 正確に往復変換できる十進桁数 |
| *_MANT_DIG | 24       | 53        | 仮数部のビット数             |
| *_MIN      | 1.18e-38 | 2.23e-308 | 最小の正規化正数             |
| *_MAX      | 3.40e+38 | 1.80e+308 | 最大の有限値                 |
| *_EPSILON  | 1.19e-07 | 2.22e-16  | 1.0 に足して区別できる最小値 |

## 浮動小数点 <-> 整数変換 (static_cast)

### double -> int

- 小数部はゼロ方向へ切り捨て (3.99 -> 3, -3.99 -> -3)
- 範囲外は未定義動作 (UB): INT_MAX+1.0 を cast すると壊れた値が出る
- ガード必須: `v != v || v > INT_MAX || v < INT_MIN`

### int -> double

- 32-bit int は全て double (53-bit 仮数部) に正確に収まる
- 精度ロスなし。ex00 で double を中間表現として使える根拠

### int -> float

- float の仮数部は 24 bit (2^24 = 16,777,216)
- 16777217 以上の int は float で正確に表現できない
- static_cast<float>(INT_MAX) = 2147483648.0f (切り上げで INT_MAX を超える!)

### double -> float

- 精度ロス: pi は 15桁 -> 6桁に丸まる
- 範囲外: FLT_MAX を超える double は float で inf になる

## IEEE 754 特殊値と ex00 の防御

### 5種の特殊値

| 特殊値       | 生成方法               | 性質                                    | ex00 の防御                      |
| ------------ | ---------------------- | --------------------------------------- | -------------------------------- |
| NaN          | 0/0, sqrt(-1), Inf-Inf | 全比較 false (自分自身とも不一致)       | M_isSpecialLiteral で先に分岐    |
| +Inf/-Inf    | 1/0, DBL_MAX*2         | 比較は正常動作 (+Inf > 任意の有限値)    | 同上 + makeIntStr の v > INT_MAX |
| -0.0         | -0.0 リテラル          | -0.0 == +0.0 は true, -0.0 < 0 は false | 特別な処理不要                   |
| 非正規化数   | DBL_MIN / 2            | 0ではないが極小、int cast = 0           | 問題なし                         |
| overflow境界 | INT_MAX+1              | int cast = UB                           | makeIntStr のガード              |

### NaN の罠: 比較の穴

```
NaN > 0      -> false
NaN < 0      -> false
NaN > INT_MAX -> false
NaN < INT_MIN -> false
NaN == NaN   -> false (唯一 v != v が true)
```

`v > INT_MAX || v < INT_MIN` だけでは NaN を弾けない。`v != v` を最初にチェックする。

### ex00 のガード条件

```cpp
// makeIntStr: NaN + overflow を全て弾く
if (v != v || v > INT_MAX || v < INT_MIN) return "impossible";

// makeCharStr: NaN はここに到達しない (M_isSpecialLiteral で先に分岐)
// ただし直接呼ばれた場合は v < 0 も 127 < v も false になり UB
if (v < 0 || 127 < v) return "impossible";
```

具体的な検証方法

- `std:;numeric_limits<(型名)>::max()`という方法で最大値は取得できる。
    - max() ・・・最大値
    - min() ・・・最小値
    - `INT_MAX ` とかはそこまで記述したりしなくても良さそう。

自分が考慮しているケース

- 正規の引数、特殊な文字。閾値、特別な値

# 一連の流れ`(convert関数)`
## 引数の型判定ロジック`(M_detectType)`
### 特別な文字か判定ロジック
- strが予め決め打ちしてある特別な文字か否か
### char型か判定ロジック
- 文字列長が1かつ数字じゃないか
  - 数字なら、intで持ちたい。
### int型か判定ロジック
- 符号スキップ
- 文字列が "-" か "+"だけじゃないか
- str内の要素が全て数字か
### float型か判定ロジック
- 符号スキップ
- 整数部の数字を読む
- 途中、"."が含まれるか
- 小数点部の数字を読む
- 指数部判定
- 末尾のf判定

### double型か判定ロジック
- 符号スキップ
- 整数部の数字を読む
- 途中、"."が含まれるか
- 小数点部の数字を読む
- 指数部判定

## strからの型変換ロジック`(M_convertToFormats)`
- 変換は`str`を`double`に変換して狭めてく。
- この4型の中では`double`が一番広く、どの型の情報も失われないため。
### char型への変換
- 0 - 127かつ表示可能か

### int型への変換
- `INT_MAXからINT_MIN`の間にあるか。

### float型への変換とdouble型への変換
- suffixの有無の差異しかない。
- ここに至るまで、vの値だけをみてもDouble型とFloat型の差はない。（ENUMで定義した型判定はあるけど）
- float型として表現するには一度floatにキャストして丸めた上でDoubleに戻すと丸められた部分が反映されて正規のfloat型の値として返される。


## 最終的な結果の出力`(M_printConversionResult)`
### 最終的な出力結果を`std::string`型で受け取る理由
- 変換した値もしくは特別な出力（Non printable, impossibleなど）をまとめて一元的に出力するため
- これにより、各タイプごとに出力する内容を分岐させたりせず「最終的な出力はここだけ」と定義する
- 平時の値と例外の値をまとめて管理・出力できるから`std::string`型を使用している。

## その他
- `std::isdigit()`, `std::isprint()`など、<cctype>系の関数は、引数の値が`unsigned char`で表現できない場合は未定義動作になるので使用前は`unsigned char`にキャスト