#include "iostream"
#include <x86intrin.h>
#include <vector>

using namespace std;

template <typename T, size_t N>
class StaticVector
{
    alignas(T) std::byte buffer[N * sizeof(T)];
    size_t count = 0;

public:
    StaticVector() = default;

    ~StaticVector()
    {
        for (size_t i = 0; i < count; i++)
        {
            reinterpret_cast<T *>(&buffer[i * sizeof(T)])->~T();
        }
    }

    template <typename... Args>
    void emplace_back(Args &&...args)
    {
        if (count >= N)
        {
            throw std::out_of_range("StaticVector is full");
        }
        new (&buffer[count * sizeof(T)]) T(std::forward<Args>(args)...);
        count++;
    }

    void pop_back()
    {

        if (count == 0)
        {
            throw std::out_of_range("StaticVector is empty");
        }
        count--;
    }

    T &operator[](size_t index)
    {
        if (index >= count)
        {
            throw std::out_of_range("Element is no present");
        }
        return *reinterpret_cast<T *>(&buffer[index * sizeof(T)]);
    }

    size_t size() const
    {
        return count;
    }

    T &back()
    {
        if (count == 0)
        {
            throw std::out_of_range("StaticVector is empty");
        }
        return *reinterpret_cast<T *>(&buffer[sizeof(T) * (count - 1)]);
    }
};

int main()
{
    unsigned aux;
    auto start = __rdtscp(&aux);
    StaticVector<int, 5> vec;
    // vec.emplace_back(1);
    // vec.emplace_back(2);
    // vec.emplace_back(3);

    // for (size_t i = 0; i < vec.size(); i++)
    // {
    //     cout << "Element at index " << i << ": " << vec[i] << endl;
    // }

    // try
    // {
    //    std::cout << vec.back() << std::endl;
    //     vec.pop_back();

    //     std::cout << vec.back() << std::endl;
    //     vec.pop_back();

    //     vec.emplace_back(4);
    //     vec.emplace_back(5);
    //     std::cout << vec.back() << std::endl;
    //     vec.pop_back();

    //     std::cout << vec.size() << std::endl;
    // }
    // catch (const std::out_of_range &e)
    // {
    //     cout << e.what() << endl;
    // }
    for (int i = 0; i < 1000000; ++i)
    {
        vec.emplace_back(i % 5);
        vec.pop_back();
    }

    auto end = __rdtscp(&aux);
    std::cout << "No of Cycles for Custom Vector this is " << end - start << std::endl;

    // traditional std vector
    start = __rdtscp(&aux);
    std::vector<int> std_vector{5};
    // std_vector.emplace_back(1);
    // std_vector.emplace_back(2);
    // std_vector.emplace_back(3);

    // for (size_t i = 0; i < std_vector.size(); i++)
    // {
    //     cout << "Element at index " << i << ": " << std_vector[i] << endl;
    // }

    // try
    // {
    //     std::cout << std_vector.back() << std::endl;
    //     std_vector.pop_back();

    //     std::cout << std_vector.back() << std::endl;
    //     std_vector.pop_back();

    //     std_vector.emplace_back(4);
    //     std_vector.emplace_back(5);
    //     std::cout << std_vector.back() << std::endl;
    //     std_vector.pop_back();

    //     std::cout << std_vector.size() << std::endl;
    // }
    // catch (const std::out_of_range &e)
    // {
    //     cout << e.what() << endl;
    // }
    for (int i = 0; i < 1000000; ++i)
    {
        std_vector.emplace_back(i % 5);
        std_vector.pop_back();
    }
    end = __rdtscp(&aux);
    std::cout << "No of Cycles for std vector this is " << end - start << std::endl;

    return 0;
}

// pop_back