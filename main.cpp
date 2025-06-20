// This is only for fun, it's C++17, I am just trying to understand Key-value store in C++17
// Explained with full comments for maximum clarity and clean, understandable code.

#include <iostream>
#include <string>
#include <optional>
#include <unordered_map>
#include <mutex>
#include <sstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cassert>
#include <fstream>
#include <ctime>

// ========== Logger ==========
// Provides timestamped info and error logs
class Logger {
public:
    static void info(const std::string& msg) {
        std::cout << timestamp() << " [INFO] " << msg << "\n";
    }

    static void error(const std::string& msg) {
        std::cerr << timestamp() << " [ERROR] " << msg << "\n";
    }

private:
    static std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

// ========== KeyValueStore ==========
// Provides thread-safe key-value storage
class KeyValueStore {
public:
    void set(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        store_[key] = value;
        Logger::info("Set: {" + key + ": " + value + "}");
    }

    std::optional<std::string> get(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = store_.find(key);
        if (it != store_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    void remove(const std::string& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        store_.erase(key);
        Logger::info("Removed key: " + key);
    }

    void print_all() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "\n[STORE DUMP]\n";
        for (const auto& [key, value] : store_) {
            std::cout << "- " << key << ": " << value << "\n";
        }
        std::cout << std::endl;
    }

    bool exists(const std::string& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return store_.find(key) != store_.end();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        store_.clear();
        Logger::info("Store cleared");
    }

    void save_to_file(const std::string& filename) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream ofs(filename);
        for (const auto& [key, value] : store_) {
            ofs << key << "=" << value << "\n";
        }
        Logger::info("Data saved to " + filename);
    }

    void load_from_file(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ifstream ifs(filename);
        if (!ifs) {
            Logger::error("Could not open file: " + filename);
            return;
        }
        std::string line;
        while (std::getline(ifs, line)) {
            auto delim = line.find('=');
            if (delim != std::string::npos) {
                std::string key = line.substr(0, delim);
                std::string val = line.substr(delim + 1);
                store_[key] = val;
            }
        }
        Logger::info("Data loaded from " + filename);
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> store_;
};

// ========== Utilities ==========
// Trims whitespace from both ends of a string
std::string trim(const std::string& str) {
    auto first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    auto last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}

// ========== CLI ==========
// Runs interactive prompt and handles commands
void run_cli(KeyValueStore& kv) {
    std::string input;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);
        std::istringstream iss(input);
        std::string cmd, key, value;

        iss >> cmd;
        if (cmd == "exit") break;

        if (cmd == "set") {
            iss >> key;
            std::getline(iss, value);
            value = trim(value);
            if (key.empty() || value.empty()) {
                Logger::error("Usage: set <key> <value>");
                continue;
            }
            kv.set(key, value);
        } else if (cmd == "get") {
            iss >> key;
            if (auto val = kv.get(key)) {
                std::cout << key << " = " << *val << "\n";
            } else {
                std::cout << "Key not found\n";
            }
        } else if (cmd == "remove") {
            iss >> key;
            kv.remove(key);
        } else if (cmd == "list") {
            kv.print_all();
        } else if (cmd == "clear") {
            kv.clear();
        } else if (cmd == "save") {
            iss >> key;
            kv.save_to_file(key);
        } else if (cmd == "load") {
            iss >> key;
            kv.load_from_file(key);
        } else {
            Logger::error("Unknown command: " + cmd);
            std::cout << "Available commands: set, get, remove, list, clear, save <file>, load <file>, exit\n";
        }
    }
}

// ========== Tests ==========
void run_tests() {
    KeyValueStore kv;
    kv.set("username", "abhishek");
    kv.set("lang", "C++17");
    assert(kv.get("username").value() == "abhishek");
    assert(kv.exists("lang"));
    kv.remove("lang");
    assert(!kv.exists("lang"));
    kv.clear();
    assert(!kv.exists("username"));
    Logger::info("All tests passed");
}

// ========== Main ==========
int main() {
    Logger::info("Running self-tests...");
    run_tests();

    Logger::info("Welcome to the Key-Value CLI Store");
    Logger::info("Type 'exit' to quit");

    KeyValueStore store;
    run_cli(store);
    return 0;
}

/*
Example session:
>> set name Abhishek
>> set lang C++
>> get name
>> get age
>> list
>> save data.txt
>> clear
>> list
>> load data.txt
>> list
>> remove name
>> list
>> exit
*/

