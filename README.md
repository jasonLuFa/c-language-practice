# c-language-practice

- [c-language-practice](#c-language-practice)
  - [VScode Extension](#vscode-extension)
  - [基本概念](#基本概念)
    - [執行程式前的步驟](#執行程式前的步驟)
    - [Link Error](#link-error)
    - [Header File](#header-file)
  - [Variable and Data Types](#variable-and-data-types)

## VScode Extension

- clangd
- code-runner

## 基本概念

### 執行程式前的步驟

- GCC 階段的預設行為，當你直接執行:

  ```bash
  gcc -Wall main.c -o main
  ```

  1. **預處理（preprocessing）**：預處理器處理 `#include` 和 `#define` 等 `#` 開頭的指令，移除所有註解，並生成 .i 檔案。
  2. **編譯（compiling）**：將預處理後的程式碼轉換為組合語言，生成 .s 檔案。
  3. **組譯（assembling）**：將 .s 檔案轉換為目標檔案 .o。
  4. **連結（linking）**：將 .o 目標檔案和必要的標準函式庫連結，生成最終的可執行檔案 main。

- 這些中間檔案（.i、.s）不會保留，因為 GCC 預設會在完成後刪除這些檔案以節省空間，並僅保留 .o 和可執行檔案。

### Link Error

在連結步驟出問題，常見原因：

1. 缺少定義的函式或變數，如果程式中宣告了某個函式或變數，但卻沒有提供具體的定義，連結器將無法找到它。

    ```c
    void func();  // 函式宣告
    int main() {
        func();    // 呼叫 func
        return 0;
    }
    // 若沒有 func() 的具體定義，連結時會報錯 Undefined reference to 'func'
    ```

2. 忘記連結必要的外部庫（Library）
   - 若在程式中使用了外部庫，但忘記在連結階段連結這些庫，導致找不到所需符號。

3. 重複定義（Multiple Definition）

   - 如果同一個函式或變數在多個檔案中有重複的定義，連結器會報錯。

     ```c
     int x = 10;  // 檔案 a.c
     int x = 20;  // 檔案 b.c
     // 編譯時會報錯 Multiple definition of 'x'
     ```

4. 函式原型與定義不匹配
   - 若函式的宣告和定義不一致，例如參數類型或返回值不同，連結階段也會出現問題。

5. 靜態連結與動態連結的問題
   - 使用靜態或動態庫時，若連結庫的路徑錯誤或庫檔案不存在，連結器無法找到所需符號。

### Header File

- 必須小寫開頭作為檔名，以 `.h` 為副檔名
- 執行 `.c` 檔案時，會使用 `#include` 先將 `.h` 檔案包含進來也就是 [預處理](#執行程式前的步驟)
  - 使用 `include <XXX.h>` 代表是用標準函式庫
  - 使用 `include "XXX.h"` 代表是用自定義函式庫
- 主要功能是聲明：它通常不包含實作，只提供函式、變數或型別的宣告。
- 特殊情況可以包含實作：
  - inline 函式，這些函式的實作通常很簡單，適合用於性能優化
  - 靜態函式的實作
  - 泛型功能（ c 不支援泛型會以  `巨集` 的形式實現，但 c++ 支援 ）
    - 巨集是一種低階的泛型替代方法，是一種文字替換方式，但是缺乏型別檢查容易引發難以追蹤的問題

      ```c
      #define SWAP(a, b, type) { type temp = a; a = b; b = temp; }
      ```

- 在每個標頭檔案中使用包含防護( protect multiple inclusions )是良好的編程習慣，可以避免多次包含導致的重複定義錯誤

  ```c
  // file1.h
  #ifndef FILE1_H
  #define FILE1_H
  /* .. 定義更多 .. */
  #endif
  ```

  or

  ```c
  // file1.h
  #pragma once  // 更加簡潔，但可能會遇到舊兼容問題

  void myFunction();  // 函式宣告
  /* .. 定義更多 .. */
  ```

  ```c
  // file1.c
  #include <stdio.h>
  #include "file1.h"

  void myFunction() {
      printf("Hello\n");
  }
  ```

  ```c
  // main.c
  #include "file1.h"
  #include "file1.h"  // 重複包含

  int main() {
      myFunction();
      return 0;
  }
  ```

## Variable and Data Types

- [2. Variable and Data Types](2.%20variable_and_data_types/README.md)
