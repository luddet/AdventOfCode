#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>

const std::string EXAMPLE(
    "35\n"
    "20\n"
    "15\n"
    "25\n"
    "47\n"
    "40\n"
    "62\n"
    "55\n"
    "65\n"
    "95\n"
    "102\n"
    "117\n"
    "150\n"
    "182\n"
    "127\n"
    "219\n"
    "299\n"
    "277\n"
    "309\n"
    "576\n"
);

constexpr size_t EXAMPLE_PREAMBLE_LENGTH(5);

template<typename T, size_t SIZE>
class CircularBuffer 
{
public:
    CircularBuffer()
        :m_indexOffset(0)
    {
        std::fill(std::begin(m_data), std::end(m_data), 0);
    }

    void push(T n)
    {
        m_data[m_indexOffset] = n;
        m_indexOffset = (m_indexOffset + 1) % SIZE;
    }

    T& operator[](size_t index)
    {
        return m_data[(index + m_indexOffset) % SIZE];
    }

    const T& operator[](size_t index) const
    {
        return m_data[(index + m_indexOffset) % SIZE];
    }

private:
    std::array<T, SIZE> m_data;
    size_t m_indexOffset;
};

template<typename T, size_t SIZE>
bool checkNumber(const CircularBuffer<T, SIZE>& preamble, T number)
{
    for (size_t i = 0; i < SIZE - 1; ++i)
        for (size_t j = i + 1; j < SIZE; ++j)
            if (preamble[i] + preamble[j] == number)
                return true;
    return false;
}

std::tuple<size_t, size_t> findRange(std::vector<int32_t> numbers, int32_t number)
{
    for (size_t i = 0; i < numbers.size() - 1; ++i)
    {
        int32_t sum(numbers[i]);
        for (size_t j = i + 1; j < numbers.size() && sum < number; ++j)
        {
            sum += numbers[j];
            if (sum == number)
                return { i, j };
        }
    }
    return { 0,0 };
}

int main()
{

#if false
    std::istringstream is(EXAMPLE);
    constexpr size_t PREAMBLE_LENGTH(EXAMPLE_PREAMBLE_LENGTH);
#else
    std::ifstream is("input.txt");
    constexpr size_t PREAMBLE_LENGTH(25);
#endif

    CircularBuffer<int32_t, PREAMBLE_LENGTH> preamble;
    std::vector<int32_t> sequence;
    int32_t n;

    for (size_t i = 0; i < PREAMBLE_LENGTH; ++i)
    {
        is >> n;
        preamble.push(n);
        sequence.push_back(n);
    }

    while (is >> n)
        sequence.push_back(n);

    for (size_t i = PREAMBLE_LENGTH; i < sequence.size(); ++i)
    {
        n = sequence[i];
        if (!checkNumber(preamble, n))
            break;
        preamble.push(n);
    }

    auto part1 = n;
    auto [start, end] = findRange(sequence, part1);
    auto [min, max] = std::minmax_element(begin(sequence) + start, begin(sequence) + end);
    auto part2 = *min + *max;

    std::cout << "Day09 Part 1: " << part1 << std::endl;
    std::cout << "Day09 Part 2: " << part2<< std::endl;
}

