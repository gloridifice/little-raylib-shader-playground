#pragma once

#define DECLARE_MEYERS_SINGLETON(ClassName)                      \
public:                                                          \
    static ClassName& Instance() {                               \
        static ClassName instance; /* Guaranteed thread-safe */  \
        return instance;                                         \
    }                                                            \
    ClassName(const ClassName&) = delete;                        \
    ClassName(ClassName&&) = delete;                             \
    ClassName& operator=(const ClassName&) = delete;             \
    ClassName& operator=(ClassName&&) = delete;                  \
private:                                                         \
    ClassName() = default;                                       \
    ~ClassName() = default;