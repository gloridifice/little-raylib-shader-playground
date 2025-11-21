

namespace hello {
namespace born {
    struct Foo {
        int aInt;
        float bFloat;
        bool cBool;
    };
} // namespace born
} // namespace hello

class Bar {
    hello::born::Foo aFoo;
    int bInt;
};
