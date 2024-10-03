# JSON Expression Parser

This is a C++ console application that allows evaluating expressions on a JSON file.

## Features

- **JSON Path Expressions**: Supports accessing JSON elements using dot notation and array indices.
- **Expressions in Subscripts**: Allows using expressions within subscript operators.
- **Intrinsic Functions**: 
  -  `min(args...)`: Returns the minimum value among the arguments.
  -  `max(args...)`: Returns the maximum value among the arguments.
  -  `size(arg)`: Returns the size of an object, array, or string.
  -  `average(args...)`: Returns the average of numeric arguments or numbers within arrays.

- **Arithmetic Operations**: Supports arithmetic binary operators: `+`, `-`, `*`, `/`.
- **Number Literals**: Can use number literals within expressions.
- **Error Handling**: Provides reasonable error reporting for invalid JSON or expressions.
- **Automated Tests**: Includes unit tests to verify functionality.

## Requirements

- **Compiler**: Clang (with C++17 support)
- **Build System**: CMake
- **Testing Framework**: GTest

## Getting Started

### Prerequisites

- **Clang Compiler**: Ensure that Clang is installed on your system.

  ```bash
  clang --version
  ```

- **CMake**: Ensure that CMake is installed.

  ```bash
  cmake --version
  ```

### Clone the Repository

```bash
git clone https://github.com/rienath/json_eval.git
cd json_eval
```

### Building the Project

1. **Create a Build Directory**

   ```bash
   mkdir build
   cd build
   ```

2. **Configure the Project with CMake**

   ```bash
   cmake -DCMAKE_CXX_COMPILER=clang++ ..
   ```

3. **Build the Project**

   ```bash
   make
   ```

   This will compile the source code and generate the `json_eval` executable.

### Running the Application

The application takes a JSON file and an expression as arguments.

**Usage:**

```bash
./json_eval <json_file> <expression>
```

**Example JSON File (`test.json`):**

```json
{
  "a": {
    "b": [
      1,
      2,
      {
        "c": "test"
      },
      [
        11,
        12
      ]
    ]
  }
}
```

**Examples:**

Here are the examples formatted as requested:

- **Accessing JSON Elements:**

  ```bash
  ./json_eval test.json "a.b[1]"
  2
  ```

  ```bash
  ./json_eval test.json "a.b[2].c"
  test
  ```

  ```bash
  ./json_eval test.json "a.b"
  [ 1, 2, { "c": test }, [ 11, 12 ] ] and not [ 1, 2, { "c": "test" } ]
  ```

- **Using Expressions in Subscripts:**

  ```bash
  ./json_eval test.json "a.b[a.b[1]].c"
  test
  ```

- **Using Intrinsic Functions:**

    - **max Function:**

      ```bash
      ./json_eval test.json "max(a.b[0], a.b[1])"
      2
      ```

    - **min Function:**

      ```bash
      ./json_eval test.json "min(a.b[3])"
      11
      ```

    - **size Function:**

      ```bash
      ./json_eval test.json "size(a)"
      1
      ```

      ```bash
      ./json_eval test.json "size(a.b)"
      4
      ```

      ```bash
      ./json_eval test.json "size(a.b[a.b[1]].c)"
      4
      ```

    - **average Function:**
        ```bash
      ./json_eval test.json "average(a.b[0], a.b[1], 5)"
      2.66667
      ```

      ```bash
      ./json_eval test.json "average(a.b[3])"
      11.5
      ```

      ```bash
      ./json_eval test.json "average(a.b[3], 5)"
      9.33333
      ```

      ```bash
      ./json_eval test.json "average(max(a.b[0], a.b[1]), min(a.b[3]))"
      6.5
      ```

- **Using Number Literals:**

  ```bash
  ./json_eval test.json "max(a.b[0], 10, a.b[1], 15)"
  15
  ```

- **Using Arithmetic Operations:**
    
    ```bash
    ./json_eval test.json "min(a.b[3], 5) + a.b[1]"
    7
    ```
    
    ```bash
    ./json_eval test.json "min(a.b[3], 5) - a.b[1]"
    3
    ```
    
    ```bash
    ./json_eval test.json "min(a.b[3], 5) * a.b[1]"
    10
    ```
    
    ```bash
    ./json_eval test.json "min(a.b[3], 5) / a.b[1]"
    2.5
    ```

    ```bash
    ./json_eval test.json "min(a.b[3], 5) % a.b[1]"
    1
    ```

### Running the Tests

The project includes automated tests using the Catch2 framework.

1. **Configure the Project with Tests Enabled**

   From the `build` directory:

   ```bash
   cmake -DCMAKE_CXX_COMPILER=clang++ -DBUILD_TESTS=ON ..
   ```

2. **Build the Tests**

   ```bash
   make
   ```

   This will compile the tests and create an executable named `tests`.

3. **Run the Tests**

   ```bash
   ./tests
   ```

   The tests will run and display the results.