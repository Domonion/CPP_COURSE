#include <vector.h>
#include <cstring>

fast32Vector::fast32Vector(size_t n, uint value) : sz(n), mode(true) {
    if (n > SZ) {
        becomeBig(ensureCapacity(sz), SZ);
    }
    std::fill(getPtr(), getPtr() + sz, value);
}

fast32Vector::fast32Vector(fast32Vector const &other) : sz(other.sz), mode(other.mode) {
    if (mode) {
        memcpy(data.arr, other.data.arr, sz * sizeof(uint));
    } else {
        new(&data.vec) vector(other.data.vec);
    }
}

fast32Vector &fast32Vector::operator=(fast32Vector other) {
    swap(other);
    return *this;
}

fast32Vector::~fast32Vector() {
    if (!mode) {
        data.vec.~vector();
    }
}

void fast32Vector::push_back(uint value) {
    check();
    if (sz == getCapacity()) {
        becomeBig(ensureCapacity(sz + 1), sz);
    }
    operator[](sz++) = value;
}

void fast32Vector::pop_back() {
    check();
    --sz;
}

uint &fast32Vector::back() {
    return operator[](sz - 1);
}

uint const &fast32Vector::back() const {
    return operator[](sz - 1);
}

uint &fast32Vector::operator[](size_t ind) {
    check();
    return *(getPtr() + ind);
}

uint const &fast32Vector::operator[](size_t ind) const {
    return *(getPtr() + ind);
}

size_t fast32Vector::size() const {
    return sz;
}

bool operator==(fast32Vector const &a, fast32Vector const &b) {
    if (a.size() != b.size()) {
        return false;
    }
    return memcmp(a.getPtr(), b.getPtr(), sizeof(uint) * a.size()) == 0;
}

void fast32Vector::swap(fast32Vector &other) {
    std::swap(this->sz, other.sz);
    std::swap(this->mode, other.mode);
    static char buffer[sizeof(Data)];
    memcpy(buffer, &data, sizeof(Data));
    memcpy(&data, &other.data, sizeof(Data));
    memcpy(&other.data, buffer, sizeof(Data));
}

size_t fast32Vector::getCapacity() {
    return (mode ? SZ : data.vec.cap);
}

void fast32Vector::check() {
    if (mode || data.vec.ptr.unique()) {
        return;
    }
    becomeBig(data.vec.cap, data.vec.cap);
}

uint *fast32Vector::getPtr() const {
    return (mode ? const_cast<uint *>(data.arr) : data.vec.ptr.get());
}

size_t fast32Vector::ensureCapacity(size_t n) {
    return (n < CAP ? CAP : n * 2);
}

void fast32Vector::becomeBig(size_t capacity, size_t amountCopy) {
    auto tmp = new uint[capacity];
    memcpy(tmp, getPtr(), amountCopy * sizeof(uint));
    if (!mode)
        data.vec.~vector();
    else
        data.~Data();
    new(&data.vec) vector(capacity, std::shared_ptr<uint>(tmp, std::default_delete<uint[]>()));
    mode = false;
}