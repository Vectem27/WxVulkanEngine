#ifndef ARRAY_H
#define ARRAY_H

#include <algorithm>  
#include <memory>
#include <stdexcept>
#include <initializer_list>

enum class ArrayOpt
{
    AO_FAST,
    AO_MEMORYSAVE,
    AO_BALANCED
};

template<typename T, ArrayOpt option = ArrayOpt::AO_BALANCED>
class Array
{
public:
    Array()
    {
        ReserveAndSwap(2);
    }

    Array(unsigned long size)
    {
        Resize(size);
    }

    Array(std::initializer_list<T> content)
    {
        for (const auto& val : content)
        {
            Add(val);
        }
    }

    Array(const Array& other)
    {
        Resize(other.GetSize());
        for (unsigned long i = 0; i < GetSize(); i++)
        {
            data[i] = other[i];
        }
    }

    ~Array()
    {
        delete[] data;
    }

    class iterator
    {
    private:
        T* it;
    public:
        iterator(T* p) : it(p) {}
        inline T& operator*() { return *it; }
        inline iterator& operator++() { ++it; return *this; }
        inline bool operator!=(const iterator& other) const { return it != other.it; }
    };

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + size); }

    iterator begin() const { return iterator(data); }
    iterator end() const { return iterator(data + size); }

    inline T& operator[](unsigned long index)
    {
        if (index < GetSize())
            return data[index];
        throw std::out_of_range("Index out of range");
    }

    inline T operator[](unsigned long index) const
    {
        if (index < GetSize())
            return data[index];
        throw std::out_of_range("Index out of range");
    }

    inline unsigned long GetCapacity() const { return capacity; }
    inline unsigned long GetSize() const { return size; }

    inline float GetCapacityMult() const {
        switch (option)
        {
            case ArrayOpt::AO_FAST: return 2.f;
            case ArrayOpt::AO_MEMORYSAVE: return 1.2f;
            default: return 1.5f;
        }
    }

    void SetCapacity(unsigned long requestedSize)
    {
        if (requestedSize > GetCapacity())
        {
            IncreaseCapacity();
            SetCapacity(requestedSize);
        }
        else if (requestedSize > 1 && requestedSize < GetCapacity() / GetCapacityMult())
        {
            DecreaseCapacity();
            SetCapacity(requestedSize);
        }
        else if (requestedSize <= 1)
        {
            SetCapacity(2);
        }
    }

    void IncreaseCapacity()
    {
        ReserveAndSwap(static_cast<unsigned long>(GetCapacity() * GetCapacityMult()));
    }

    void DecreaseCapacity()
    {
        ReserveAndSwap(static_cast<unsigned long>(GetCapacity() / GetCapacityMult()));
    }

    void Resize(unsigned long n)
    {
        SetCapacity(n);

        if (n <= GetSize())
        {
            size = n;
            return;
        }

        for (unsigned long i = GetSize(); i < n; i++)
        {
            data[i] = T{};
        }

        size = n;
    }

    void Add(const T& value)
    {
        Resize(GetSize() + 1);
        data[GetSize() - 1] = value;
    }

    void Add(T&& value)
    {
        Resize(GetSize() + 1);
        data[GetSize() - 1] = std::move(value);
    }

    void RemoveIndex(const unsigned long& index)
    {
        if (index >= GetSize())
            return;

        for (unsigned long i = index; i < GetSize() - 1; i++)
        {
            data[i] = data[i + 1];
        }

        Resize(GetSize() - 1);
    }

    bool RemoveValues(const T& value)
    {
        unsigned long step = 0;

        for (unsigned long i = 0; i + step < GetSize(); i++)
        {
            while (i + step < GetSize() && data[i + step] == value)
                ++step;

            if (i + step < GetSize())
                data[i] = data[i + step];
        }

        if (step > 0)
            Resize(GetSize() - step);

        return step > 0;
    }

    bool Insert(const T& value, unsigned long index, bool bSizeToFit = true)
    {
        if (index >= GetSize() && !bSizeToFit)
            return false;
        else if(index >= GetSize())
            Resize(index + 1);
        else
            Resize(GetSize() + 1);

        for (unsigned long i = GetSize() - 1; i > index; --i)
        {
            data[i] = data[i - 1];
        }

        data[index] = value;
        return true;
    }

    void Clear()
    {
        Resize(0);
    }

    T* GetData() { return data; }

private:
    T* data{ nullptr };
    unsigned long size{ 0 };
    unsigned long capacity{ 0 };

    void ReserveAndSwap(unsigned long n)
    {
        if (n < 2) n = 2;
        unsigned long newSize = std::min(n, GetSize());

        T* newDatas = (T*)malloc(n * sizeof(T));

        for (unsigned long i = 0; i < newSize; ++i)
            newDatas[i] = std::move(data[i]); // utilise l'opérateur =

        free(data);

        data = newDatas;
        size = newSize;
        capacity = n;
    }

};

#endif // ARRAY_H
