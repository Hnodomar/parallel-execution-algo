#include <iostream>
#include <vector>
#include <execution>
#include <numeric>
#include <chrono>
#include <cctype>
#include <string_view>
#include <string>

std::string setupTestString(const int& length) {
    std::string ret(length, 'a');
    for (int i = 0; i < length; ++i)
        if (i % 5 == 0 || i % 17 == 0)
            ret[i] = ' ';
    std::cout << "strlen: " << length << std::endl;
    return ret;
} 

template <typename ExecutionPolicy>
std::size_t countWords(std::string_view test_str, ExecutionPolicy policy) {
    if (test_str.empty())
        return 0;
    std::size_t word_count = !std::isspace(test_str.front()) ? 1 : 0;
    word_count +=
        std::transform_reduce(
            policy,
            test_str.begin(),
            test_str.end() - 1,
            test_str.begin() + 1,
            std::size_t(0),
            std::plus<std::size_t>(),
            [](char lhs, char rhs) {
                return std::isspace(lhs)
                    && !std::isspace(rhs);
            }
    );
    return word_count;
}

template<typename AlgoType>
void measureTest(const char* test_name, AlgoType run_test) {
    const auto start_time = std::chrono::steady_clock::now();
    std::size_t ret = run_test();
    const auto end_time = std::chrono::steady_clock::now();
    std::cout << test_name << ": " 
        << std::chrono::duration<double, std::milli>(end_time - start_time).count()
        << " ms, res " << ret << std::endl;
}

int main(int argc, char** argv) {
    const int len = argc > 1 ? atoi(argv[1]) : 1;
    if (len == 1) {
        printf("Usage: parallelWordCount <len>\n");
        return 1;
    }
    std::string test_str(setupTestString(len));
    measureTest("word_count seq", [&test_str]{
        return countWords(test_str, std::execution::seq);
    });
}
