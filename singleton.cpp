#include <iostream>

// Singleton class
template <typename T>
class Singleton {
protected:
    Singleton() = default;
    ~Singleton() = default;

public:
    static T& Instance() {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;
};

// Another class inherit from Singleton
class Logger : public Singleton<Logger> {
    friend class Singleton;
    Logger() = default;

public:
    void Log(const char* message) {
        std::cout << "[LOG] " << message << '\n';
    }
};

int main() {
    Logger::Instance().Log("Hello, world!");
    return 0;
}
