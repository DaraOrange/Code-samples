#include <deque>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
struct VectorBase {
    T* data;
    size_t cp;

    VectorBase(size_t n) {
        data = static_cast<T*>(::operator new(n * sizeof(T)));
        cp = n;
    }

    friend void swap(VectorBase& fir, VectorBase& sec) {
        std::swap(fir.data, sec.data);
        std::swap(fir.cp, sec.cp);
    }

    ~VectorBase() {
        ::operator delete(data);
    }
};

template <typename T>
class Vector {
private:
    VectorBase<T> base;
    size_t sz;

public:
    Vector(): base(0), sz(0) {}

    Vector(const Vector& oth) : base(oth.base) {
        size_t i = 0;
        try {
            for (; i != oth.sz; ++i)
                new(base.data + i) T(oth.base.data[i]);
        } catch(...) {
            for (size_t j = 0; j != i; ++j)
                base.data[j].~T();
            throw;
        }
        sz = oth.sz;
    }

    friend void swap(Vector& fir, Vector& sec) {
        std::swap(fir.sz, sec.sz);
        swap(fir.base, sec.base);
    }

    Vector& operator=(const Vector& oth) {
        Vector tmp(oth);
        swap(*this, tmp);
        return *this;
    }

    T& operator[] (int i) {
        return base.data[i];
    }

    const T& operator[] (int i) const {
        return base.data[i];
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return base.cp;
    }

    void reserve(size_t cp) {
        if (base.cp >= cp)
            return;
        size_t i = 0;
        VectorBase<T> tmp(cp);
        try {
            for (; i != sz; ++i)
                new(tmp.data + i) T(base.data[i]);
            swap(tmp, base);
            for (size_t j = 0; j != sz; ++j)
                tmp.data[j].~T();
        } catch(...) {
            for (size_t j = 0; j != i; ++j)
                tmp.data[j].~T();
            throw;
        }
    }

    void resize(size_t new_sz) {
        if (new_sz > base.cp) {
            VectorBase<T> tmp(new_sz);
            size_t i = 0;
            try {
                for (; i != min(sz, new_sz); ++i)
                    new(tmp.data + i) T(base.data[i]);
                for (; i != new_sz; ++i)
                    new(tmp.data + i) T();
                swap(tmp, base);
                for (size_t j = 0; j != sz; ++j)
                    tmp.data[j].~T();
            } catch(...) {
                for (size_t j = 0; j != i; ++j)
                    tmp.data[j].~T();
                throw;
            }
        } else {
            if (sz <= new_sz) {
                for (size_t i = sz; i != new_sz; ++i)
                    new(base.data + i) T();
            } else {
                for (int i = sz - 1; i >= new_sz; --i)
                    base.data[i].~T();
            }
        }
        sz = new_sz;
    }

    void push_back(const T& x) {
        if (base.cp == sz)
            reserve(base.cp ? base.cp * 2 : 1);
        new(base.data + sz) T(x);
        ++sz;
    }

    void push_back(T&& x) {
        if (base.cp == sz)
            reserve(base.cp ? base.cp * 2 : 1);
        new(base.data + sz) T(std::move(x));
        ++sz;
    }

    void pop_back() {
        --sz;
        base.data[sz].~T();
    }

    void clear() {
        for (size_t i = 0; i != sz; ++i)
            base.data[i].~T();
        sz = 0;
    }

    ~Vector() {
        clear();
    }

    T* begin() {
        return base.data;
    }

    T* end() {
        return base.data + sz;
    }
};
