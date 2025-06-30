
## 🔐 C++17 Key-Value Store CLI 🗃️

A clean, beginner-friendly, and thread-safe key-value store written in modern C++17, built with:

* 🧵 Mutex locks for thread safety
* 💾 Save/load functionality using JSON-like file format
* 📜 Clean command-line interface (CLI)
* 🧪 Self-tests with assertions
* ✅ Designed to be fun, readable, and extendable!

---

### 📦 Features

* `set <key> <value>`: Add or update key-value pairs
* `get <key>`: Fetch value by key
* `remove <key>`: Delete a key-value pair
* `list`: Print all key-value pairs
* `clear`: Delete everything
* `save <filename>`: Save to file (e.g. `data.json`)
* `load <filename>`: Load from file and auto-display
* `exit`: Exit the app
* 🧪 Runs internal unit tests at startup

---

### 🚀 Build & Run

#### 1. Clone or copy the `main.cpp` file

#### 2. Compile using `g++` or `clang++`

```bash
# GCC
g++ -std=c++17 -Wall -Wextra -o kvstore main.cpp

# or Clang with sanitizers (recommended for dev)
clang++ -std=c++17 -g -O0 -Wall -Wextra -fsanitize=address,undefined -o kvstore main.cpp
```

#### 3. Run the program

```bash
./kvstore
```

---

### 💻 Example CLI Session

```txt
>> set name Abhishek
>> set lang C++
>> get name
name = Abhishek
>> save data.json
>> clear
>> list
[STORE DUMP]
(empty)
>> load data.json
[STORE DUMP]
- name: Abhishek
- lang: C++
>> remove name
>> list
- lang: C++
>> exit
```

---

### 📁 Sample JSON Output (`data.json`)

```json
{
  "name": "Abhishek",
  "lang": "C++"
}
```

---

### 🔒 Thread Safety & Extensibility

* This uses `std::mutex` for all data operations
* Clean design, easy to extend with `export`, `import`, JSON libs like `nlohmann/json`, encryption, etc.

(I just build this, only for fun, hahaha, and it becomes literal fun..)

