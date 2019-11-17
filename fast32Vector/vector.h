#ifndef HOMEWORK_VECTOR_H
#define HOMEWORK_VECTOR_H

#include <cstdlib>
#include <memory>
#include <cstring>

const size_t CAP = 16;
const size_t SZ = 8;

typedef unsigned int uint;

class fast32Vector {
public:

    fast32Vector() = default;

    fast32Vector(size_t n, uint val);

    fast32Vector(fast32Vector const &other);

    fast32Vector &operator=(fast32Vector other);

    ~fast32Vector();

    void push_back(uint val);

    void pop_back();

    uint &back();

    uint const &back() const;

    uint &operator[](size_t ind);

    uint const &operator[](size_t ind) const;

    size_t size() const;

    friend bool operator==(fast32Vector const &a, fast32Vector const &b) ;

    void swap(fast32Vector & other);

    size_t getCapacity();
private:
    size_t sz = 0;
    bool mode = true;

    void check();

    uint *getPtr() const;

    size_t ensureCapacity(size_t n);

    void becomeBig(size_t amountEnsure, size_t amountCopy);

    struct vector {
        size_t cap = CAP;
        std::shared_ptr<uint> ptr = nullptr;

        vector(size_t capacity, std::shared_ptr<uint> pointer) : cap(capacity), ptr(pointer) {}
    };

    union Data {
        uint arr[SZ];
        vector vec;

        Data() {}

        ~Data() {}
    } data;
};

#endif //HOMEWORK_VECTOR_H