# c-language-practice

- [c-language-practice](#c-language-practice)
  - [VScode Extension](#vscode-extension)
  - [基本概念](#基本概念)
    - [執行程式前的步驟](#執行程式前的步驟)
    - [Link Error](#link-error)
    - [Header File](#header-file)
    - [gcc 指令](#gcc-指令)
    - [檔案類型](#檔案類型)
      - [靜態庫](#靜態庫)
      - [動態庫](#動態庫)
      - [依賴檔](#依賴檔)
  - [Variable and Data Types](#variable-and-data-types)
  - [Makefile 和 makefile.inc](#makefile-和-makefileinc)
    - [語法](#語法)
    - [makefile.inc](#makefileinc)
    - [Makefile](#makefile)
  - [Pointer](#pointer)
  - [Function](#function)
    - [Function Pointer](#function-pointer)
    - [Normal Function](#normal-function)
  - [const](#const)

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
  4. **連結（linking）**：連結器 (linker) 將 .o 目標檔案和必要的標準函式庫連結，生成最終的可執行檔案 main。

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

### gcc 指令

- `-o <執行檔名>`：指定輸出檔案名稱 (省略 `-o` 時預設為 `a.out`)
  - ex : `gcc file.c -o program` -> 編譯 + 連結（預設行為）時的輸出
  - ex : `gcc -c file.c -o file.o` -> 只編譯（-c）時的輸出
  - ex : `gcc file.o -o program` -> 只連結物件檔時的輸出
- `-c <filename.c>`：生成 `.o` 目標文件，不進行鏈接（Link）
  - ex: `gcc -c filename.c`
- `-Wall`: 啟用大多數警告
  - ex: `gcc -Wall filename.c -o output` -> 編譯並連接
  - ex: `gcc -Wall filename.o -o output` -> 直接連接
  - ex: `gcc -Wall filename.c filename.o -o output` -> 部分編譯並連接,部分直接連接
- `-I`: 指定標頭檔案的路徑
  - ex: `gcc -I ./include -o main ./src/main.c`

    ```text
    project/
    ├── include/
    │   ├── myheader.h
    ├── src/
    │   ├── main.c
    ```

- `-L`：定義庫的搜尋順序，連結器將使用它找到的第一個匹配項
- `-Wl,-Bstatic -l<庫名稱> -l<庫名稱>`：靜態連結（Static Link）（將庫文件包含在執行檔案中），靜態庫（.a 檔）
- `-Wl,-Bdynamic -l<庫名稱> -l<庫名稱>`：動態連結（Dynamic Link）（將庫文件包含在執行檔案中），動態庫（.so 檔）

### 檔案類型

在 C 語言專案中，常見的重要檔案類型包括：

| 檔案類型 | 副檔名                                    | 描述                                                                   |
| -------- | ----------------------------------------- | ---------------------------------------------------------------------- |
| 源代碼檔 | .c                                        | 包含 C 語言撰寫的源代碼，是程式的主要部分。                            |
| 標頭檔   | .h                                        | 包含函數原型、宏定義、結構體定義等，用於在多個源代碼檔之間共享資訊。   |
| 物件檔   | .o                                        | 經過編譯器編譯後生成的中間檔案，每個 .c 檔案通常對應一個 .o 檔。       |
| 靜態庫   | .a（Unix/Linux）</br> .lib（Windows）       | 包含多個物件檔（.o 檔），通過 ar 工具打包，用於靜態鏈結。              |
| 動態庫   | .so（Unix/Linux）</br>.dll（Windows）      | 動態共享庫，在運行時被載入使用。                                       |
| 可執行檔 | 無副檔名（Unix/Linux）</br>.exe（Windows） | 最終生成的可執行程式。                                                 |
| Makefile | Makefile                                  | 定義編譯和建構規則的文件，指導 Make 工具如何編譯和連結程式。           |
| 包含檔   | makefile.inc（或其他名稱）                | 包含共用變數和規則的 Makefile 包含檔，用於模組化和重用 Makefile 規則。 |
| 依賴檔   | .d   | 記錄了每個源文件（.c 檔案）所依賴的標頭文件（.h 檔案）的關係，用於自動化管理依賴關係。 |

#### 靜態庫

- 當您需要生成獨立的可執行檔，無需外部依賴。
- 當專案較小，且不需要頻繁更新庫。
- 當您希望簡化部署和分發過程。
- ex:
  1. 建立靜態庫
       - 編譯源文件為物件檔

         ```sh
         gcc -c utils.c -o utils.o
         gcc -c helpers.c -o helpers.o
         ```

       - 打包物件檔成靜態庫

         ```sh
         ar -rs libutils.a utils.o helpers.o
         ```

  2. 使用靜態庫

      ```sh
      gcc -o myprogram main.c -L. -lutils
      ```

#### 動態庫

- 當多個應用程式需要使用相同的庫，通過共享動態庫來節省系統資源。
- 當庫需要頻繁更新，而不希望重新編譯所有依賴的應用程式。
- 當專案規模較大，且需要靈活的模組化設計。
- ex:
  1. 建立動態庫
       - 編譯源文件為位置無關的物件檔

         ```sh
         gcc -fPIC -c utils.c -o utils.o
         gcc -fPIC -c helpers.c -o helpers.o
         ```

       - 打包物件檔成動態庫

         ```sh
         gcc -shared -o libutils.so utils.o helpers.o
         ```

  2. 使用動態庫

      ```sh
      gcc -o myprogram main.c -L. -lutils
      ```

  - 並確保動態庫在運行時的搜索路徑中

      ```sh
      export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
      ./myprogram
      ```

#### 依賴檔

- 自動化依賴管理：.d 檔案通過自動記錄這些依賴關係，幫助 Make 系統正確決定哪些文件需要重新編譯
- 減少手動維護，提高編譯效率：透過 .d 檔案，Make 只會重新編譯那些實際受影響的源文件，而不會重複編譯所有文件，從而節省編譯時間
- ex:
  - `gcc -Wall -Wno-strict-aliasing -Wno-uninitialized -g -rdynamic -std=gnu99 -MMD -MF utils.d -c utils.c -I ../network`
    - `-MMD`：生成依賴檔案，但不包含系統標頭文件的依賴。
    - `-MF utils.d`：指定依賴檔案的名稱為 utils.d。
    - `-I ../network`：指定標頭文件(.h)的搜索路徑。

## Variable and Data Types

- [Variable and Basic Data Types](variable_and_basic_data_types/README.md)

## Makefile 和 makefile.inc

### 語法

- Makefile 和 makefile.inc 的語法是相同的
- 一個基本的 Makefile 包含以下幾個部分：
  - **變數定義**：使用 `:=` 來定義變數。
  - **目標**（Targets）：要生成的檔案，如可執行檔或庫檔。
  - **依賴**（Dependencies）：目標檔案所依賴的源文件或其他目標。
  - **命令**（Commands）：生成目標所需執行的命令。
- **依賴** 都執行完，才會執行 **命令**
- **.PHONY** 目標是特殊命令而不是實際的文件目標 (下面範例可用 `make clean` 清除目標)
- **include** 指定要包含的檔案， **-include** 同 **include** 但，檔案不在時不會報錯
  - ex:

    ```makefile
    include makefile.inc
    ```

    ```makefile
    # 創建 obj 目錄
    OBJ_DIR = obj

    # 獲取所有 .c 文件
    SRCS = $(wildcard *.c)
    OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

    # 添加 .h 文件依赖
    DEPS = $(OBJS:.o=.d)
    -include $(DEPS)
    ```

- `make` 命令的預設目標是第一個目標
- `$(shell <linux 命令>)`: 執行 shell 命令，會在執行任何指令前執行
- 相關函數
  - `wildcard <pattern>`: 用於匹配多個文件
  - `patsubst <pattern> <replacement> <text>`: 與 `<pattern>` 相符的 `<text>` 並將其替換為 `<replacement>`
    - 同 `$(<text>:<pattern>=<replacement>)`
    - ex: `$(patsubst %.c, %.o, $(SOURCES))` 同 `$(SOURCES:%.c=%.o)`
- 自動變數
  - `$@`: 目標檔案
  - `$<`: 第一個依賴檔案（來源檔案）
  - `$^`: 所有依賴檔案
- 後綴規則（suffix rules）：用來定義特定後綴的檔案之間如何轉換的規則，是一種較舊的寫法，現在多數情況會使用更靈活的模式規則（pattern rules）替代
  - ex:

    ```makefile
    # 同 .c.o: (suffix rules)
    # 以下是 pattern rules
    %.o: %.c
        gcc -c $< -o $@
    ```

### makefile.inc

- 同樣是 Makefile，只是用於存放專案中多個 Makefile 可能會共用的變數和規則。這樣的設計有助於模組化和重用，減少重複代碼，並使 Makefile 更易於維護和管理。
- 在 `Makefile` 中引用 `makefile.inc` 檔案，以便在多個 Makefile 中共用相同的變數和規則。

```makefile
# 搜集所有 .c 檔案
SOURCE  := $(wildcard *.c)
# 將 .c 檔案轉換為 .o 檔案（不會直接編譯，而是名稱列表）
OBJS    := $(patsubst %.c, %.o, $(SOURCE))
# 變數定義
CC := gcc
CFLAGS := -Wall -Wno-strict-aliasing -Wno-uninitialized -g -rdynamic -std=gnu99
LFLAGS := -g -rdynamic

# 虛擬目標
.PHONY: all clean install

# 預設目標
all: $(TARGET)

# 清理目標
clean:
  rm -rf *.d *.o $(TARGET)

# 生成目標
$(TARGET): $(OBJS)
  $(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

# 後綴規則，當 make 確定需要從某個 .c 檔案生成對應的 .o 檔案時(執行 OBJS 中 patsubst 時 )，會執行這條規則，也就是從 .c 編譯到 .o ($<：即 .c 檔案）
.c.o:
  # 同 `$(CC) $(CFLAGS) -c $< -o $@ $(INCS)`
  $(CC) $(CFLAGS) -c -o $@ $< $(INCS)

# 安裝目標（留空，具體在主 Makefile 中定義）
install:
```

### Makefile

```makefile
TARGET  := target.a
INCS = -I ../network
include ../makefile.inc
# 覆寫 makefile.inc
$(TARGET) : $(OBJS)
  # 使用 ar 工具打包物件檔成靜態庫( $@ 是 $(TARGET) 代表目標, $^ 是 $(OBJS) 代表所有依賴)
  # -r：（Replace or Insert） 如果檔案已存在於靜態庫中，則用新版本替換，否則則插入新的檔案
  # -s：（Create or Update Symbol Table） 確保靜態庫在修改後的符號表是最新的
  ar -rs $@ $^
```

## Pointer

- 指標的定義：指標是一種變數，用於存儲其他變數的記憶體地址
- 指標操作符
  - `&`：取得變數的地址。
  - `*`：訪問指標指向的記憶體地址中的值

    ```c
    int x = 10;
    int *p = &x; // p 是一個指向 x 的指標
    ```

- `int *p1 = NULL;` 同 `int *p2 = 0;` 都是初始化，但前者較有可讀性

## Function

### Function Pointer

- 用途: 用於需要 **自定義邏輯** 或 **動態行為** 的場景，尤其是當操作的數據類型（例如 key、value）可能根據需求而變化時。
- 特點:
  - **動態行為**: 可以在運行時決定具體執行的函數。
  - **靈活性**: 可用於不同類型的數據結構，無需修改核心邏輯。
  - **可插拔**: 通過指向不同函數，實現對應的行為。
- 適用場合:
  - 哈希函數、自定義比較、複製或銷毀對象等需要針對特定類型的實現。

    ```c
    typedef struct dict_entry {
      void *key;
      void *val;
      uint64_t id;
      struct dict_entry *next;  // 必須使用 struct，因為 dict_entry 尚未完成定義
    } dict_entry;

    typedef struct dict_types {
        uint32_t (*hash_function)(const void *key);
        void *(*key_dup)(const void *key); // key_dup 是 function pointer，並回傳一個指向 void (任何型別)的指標
        void *(*val_dup)(const void *val);
        int (*key_compare)(const void *key1, const void *key2);
        void (*key_destructor)(void *key);
        void (*val_destructor)(void *val);
    } dict_types;

    typedef struct dict_t {
      dict_entry **table;
      dict_types type;
      uint32_t size;
      uint32_t mask;
      uint32_t used;
      uint64_t id_start;
      uint64_t id_clear;
    } dict_t;
    ```

### Normal Function

- 用途: 處理核心業務邏輯，這些邏輯與具體數據類型無關。
- 特點:
  - **固定行為**: 適用於所有場景的通用操作。
  - **結構化**: 實現數據結構的基本功能，例如初始化、添加、查找等操作。
- 適用場合:
  - 操作固定數據結構的功能，例如創建字典、添加條目、查找條目等。

    ```c
    dict_t *dict_create(dict_types *type, uint32_t init_size); // dict_craate 是 noraml function，並回傳一個指向 dict_t 的指標
    dict_entry *dict_add(dict_t *dt, void *key, void *val);
    dict_entry *dict_find(dict_t *dt, const void *key);
    int dict_expand(dict_t *dt, uint32_t size);
    int dict_replace(dict_t *dt, void *key, void *val);
    int dict_delete(dict_t *dt, const void *key);
    void dict_release(dict_t *dt);
    ```

## const

| 變數宣告 | 指針可修改？ | 值可修改？ | 是否需要初始化？ |
|------|------------|-----------|----------------|
| const int *a | 是 | 否 | 否，但建議初始化為 NULL 如果尚未有可用的地址。若未初始化就解引用指針，會導致未定義行為，可能引發程式崩潰、記憶體損壞或不預期的結果。 |
| int *const b | 否 | 是 | 是，因為指針不可改變，因此必須在宣告時初始化。 |
| const int *const c | 否 | 否 | 是，因為指針和指向的值都不可改變，因此必須在宣告時初始化。 |

範例：

```c
const int *a = NULL; // Safe initialization
int x = 10;
a = &x; // Assign a valid address
printf("%d\n", *a); // Valid: prints 10
```

```c
int x = 20;
int *const b = &x; // Must initialize b
*b = 30; // Valid: modify the value at b
b = NULL; // Error: cannot change the address stored in b
```

```c
const int x = 40;
const int *const c = &x; // Must initialize c
*c = 50; // Error: cannot modify the value pointed to by c
c = NULL; // Error: cannot change the address stored in c
```
