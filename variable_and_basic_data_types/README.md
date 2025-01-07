# Variable And Basic Data Types

- [Variable And Basic Data Types](#variable-and-basic-data-types)
  - [基本（原始）資料型](#基本原始資料型)
    - [整理](#整理)
      - [類型修飾符的整數類型](#類型修飾符的整數類型)
      - [帶有類型修飾符的浮點類型](#帶有類型修飾符的浮點類型)
  - [Enum](#enum)
  - [static 宣告的變數特性](#static-宣告的變數特性)
  - [Command Line Arguments](#command-line-arguments)
  - [補充](#補充)
    - [變數的初始化](#變數的初始化)
      - [自動初始化為零值](#自動初始化為零值)
      - [初始值是未定義的](#初始值是未定義的)
      - [結論](#結論)

## 基本（原始）資料型

- 基本資料類型。
  - int：整數型
  - char：字元類型
  - float：單精度浮點
  - double：雙精準度浮點
- 類型修飾符（Type Modifiers），可以改變這些基本類型的大小和範圍：
  - short：更小的整數
  - long：更大的整數
  - signed：是非負數
  - unsigned：是負數

### 整理

#### 類型修飾符的整數類型

| Basic Type | Modifiers | Modified Type | Size (Bytes) | Range (Typical on 32-bit/64-bit Systems) |
|------------|-----------|---------------|--------------|------------------------------------------|
| char | signed | signed char | 1 | -128 to 127 |
| char | unsigned | unsigned char | 1 | 0 to 255 |
| short | (None) | short int or short | 2 | -32,768 to 32,767 |
| short | signed | signed short int | 2 | -32,768 to 32,767 |
| short | unsigned | unsigned short int | 2 | 0 to 65,535 |
| int | (None) | int | 4 | -2,147,483,648 to 2,147,483,647 |
| int | signed | signed int | 4 | -2,147,483,648 to 2,147,483,647 |
| int | unsigned | unsigned int | 4 | 0 to 4,294,967,295 |
| long | (None) | long int or long | 4 (32-bit) / 8 (64-bit) / 8 | -2,147,483,648 to 2,147,483,647 (32-bit)<br>-9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 (64-bit) |
| long | signed | signed long int | 4 (32-bit)  / 8 (64-bit) | Similar to long int |
| long | unsigned | unsigned long int | 4 (32-bit)  / 8 (64-bit) | 0 to 4,294,967,295 (32-bit)<br>0 to 18,446,744,073,709,551,615 (64-bit) |
| long long | (None) | long long int or long long | 8 | -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 |
| long long | signed | signed long long int | 8 | Similar to long long int |
| long long | unsigned | unsigned long long int | 8 | 0 to 18,446,744,073,709,551,615 |

#### 帶有類型修飾符的浮點類型

| Basic Type | Modifiers | Modified Type | Size (Bytes) | Precision |
|------------|-----------|---------------|--------------|-----------|
| float | (None) | float | 4 | 約 7 decimal digits |
| double | (None) | double | 8 | 約 15 decimal digits |
| double | long | long double | 12 / 16 | More precise than double; system-dependent |


## Enum

- enum 是一種使用者定義的資料型別，由整數常數組成
- enum 可以自訂數字，沒另外定義會依序數字下去

  ```c
  enum ErrorCode {
      SUCCESS = 0,
      ERROR_FILE_NOT_FOUND = 404,
      ERROR_ACCESS_DENIED,    // 405
      ERROR_NETWORK,          // 406
      ERROR_UNKNOWN = 1000
  };
  ```

## static 宣告的變數特性

1. 變數的壽命 (Lifetime)：
   - static變數的壽命等同於整個程式執行期間。
   - 它們只會在程式執行時初始化一次，並且會保留值直到程式結束。
2. 作用域 (Scope)：
   - 函式內的static變數：
     - 作用域僅限於該函式 (local scope)。
     - 即使函式多次被呼叫，static變數仍然會保留上一次的值。
   - 檔案內的static變數 (Global static)：
     - 作用域僅限於定義它的檔案 (file scope)。
     - 無法被其他檔案存取，即使透過extern宣告也不行。
3. 使用情境：
   - 函式內static變數：
     - 適合用於需要在函式呼叫間保留狀態的情況，例如計數器、開關、緩存等。
   - 檔案內static變數：
     - 適合用於限制變數的可見性，避免全域變數命名衝突，強化模組化設計。

## Command Line Arguments

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
  int numberOfArgs = argc;
  char *arg1 = argv[0];
  char *arg2 = argv[1];

  printf("Number of arguments: %d\n", numberOfArgs);
  printf("First argument is the name of the program: %s\n", arg1);
  printf("Second argument: %s\n", arg2);
  return 0;
}

// gcc -Wall main.c -o main
// ./main Hello
//
// Output:
// Number of arguments: 2
// First argument is the name of the program: ./main
// Second argument: Hello
```

## 補充

### 變數的初始化

#### 自動初始化為零值

- 兩種變數定義情形
  - 靜態變數(static)
  - 全域變數

```c
#include <stdio.h>
#include <stdbool.h>

// 全域變數
int globalInt;
float globalFloat;
char globalChar;
int* globalPtr;
bool globalBool;

// 結構體
typedef struct {
    int a;
    float b;
    char c;
    int* ptr;
    bool flag;
} MyStruct;

// 全域結構體變數
MyStruct globalStruct;

int main() {
    // 靜態變數
    static int staticInt;
    static float staticFloat;
    static char staticChar;
    static int* staticPtr;
    static bool staticBool;

    // 靜態結構體變數
    static MyStruct staticStruct;

    // 輸出全域變數和靜態變數的預設值
    printf("全域變數:\n");
    printf("globalInt = %d\n", globalInt);
    printf("globalFloat = %.1f\n", globalFloat);
    printf("globalChar = '%c'\n", globalChar);
    printf("globalPtr = %p\n", (void*)globalPtr);
    printf("globalBool = %d\n", globalBool);

    printf("\n靜態變數:\n");
    printf("staticInt = %d\n", staticInt);
    printf("staticFloat = %.1f\n", staticFloat);
    printf("staticChar = '%c'\n", staticChar);
    printf("staticPtr = %p\n", (void*)staticPtr);
    printf("staticBool = %d\n", staticBool);

    printf("\n全域結構體變數:\n");
    printf("globalStruct.a = %d\n", globalStruct.a);
    printf("globalStruct.b = %.1f\n", globalStruct.b);
    printf("globalStruct.c = '%c'\n", globalStruct.c);
    printf("globalStruct.ptr = %p\n", (void*)globalStruct.ptr);
    printf("globalStruct.flag = %d\n", globalStruct.flag);

    printf("\n靜態結構體變數:\n");
    printf("staticStruct.a = %d\n", staticStruct.a);
    printf("staticStruct.b = %.1f\n", staticStruct.b);
    printf("staticStruct.c = '%c'\n", staticStruct.c);
    printf("staticStruct.ptr = %p\n", (void*)staticStruct.ptr);
    printf("staticStruct.flag = %d\n", staticStruct.flag);

    return 0;
}

// Output:
// 全域變數:
// globalInt = 0
// globalFloat = 0.0
// globalChar = ''
// globalPtr = (nil)
// globalBool = 0

// 靜態變數:
// staticInt = 0
// staticFloat = 0.0
// staticChar = ''
// staticPtr = (nil)
// staticBool = 0

// 全域結構體變數:
// globalStruct.a = 0
// globalStruct.b = 0.0
// globalStruct.c = ''
// globalStruct.ptr = (nil)
// globalStruct.flag = 0

// 靜態結構體變數:
// staticStruct.a = 0
// staticStruct.b = 0.0
// staticStruct.c = ''
// staticStruct.ptr = (nil)
// staticStruct.flag = 0
```

#### 初始值是未定義的

- 局部變數

```c
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int id;
    float score;
    bool passed;
} Student;

int main() {
    // 自動變數
    int localInt;
    float localFloat;
    int* localPtr;
    bool localBool;

    // 自動結構體變數
    Student localStruct;

    // 印出未初始化的自動變數
    printf("未初始化的自動變數:\n");
    printf("localInt = %d\n", localInt);
    printf("localFloat = %.2f\n", localFloat);
    printf("localPtr = %p\n", (void*)localPtr);
    printf("localBool = %d\n", localBool);

    printf("\n未初始化的自動結構體變數:\n");
    printf("localStruct.id = %d\n", localStruct.id);
    printf("localStruct.score = %.2f\n", localStruct.score);
    printf("localStruct.passed = %d\n", localStruct.passed);

    return 0;
}

// Output: (每次結果會不一樣)
// 未初始化的自動變數:
// localInt = -1942945319
// localFloat = 0.00
// localPtr = 0x64
// localBool = 0

// 未初始化的自動結構體變數:
// localStruct.id = 0
// localStruct.score = -40084037002407907279503360.00
// localStruct.passed = 212
```

#### 結論

- **靜態變數** 和 **全域變數** 自動初始化為 0，但仍建議在宣告時明確地初始化，這樣程式碼更直觀。
- **局部變數** 未初始化可能會導致未定義行為，因此應該在宣告時進行初始化，即使只初始化為 0 或 NULL，這樣可以避免許多潛在的錯誤，增強程式的安全性與穩定性
