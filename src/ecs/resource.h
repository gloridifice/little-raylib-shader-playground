#pragma once
#include "baleine_type/functional.h"
#include "baleine_type/memory.h"

using namespace baleine;

namespace HORiz {
template<typename T>
class Res<T> {
private:
    Fn<Unique<T>()> initializer;
    bool isInitialized = false;
    Unique<T> instance;

    void TryInit() {
        if (!isInitialized) {
            instance = initializer();
            isInitialized = true;
        }
    }

public:
    Res(Fn<Unique<T>()> initializer, bool initAtStart):
        initializer(initializer) {

        if (initAtStart) {
            TryInit();
        }
    }

    T& Instance() {
        TryInit();
        return *instance;
    }
};
}