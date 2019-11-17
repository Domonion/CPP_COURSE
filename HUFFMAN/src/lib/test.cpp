#include <chrono>
#include <generator.h>
#include <OccurrencyMap.h>
#include <gtest.h>
#include <string>
#include <random>
#include <ctime>

std::string archieve(std::string const &a, generator &b) {
    std::string ans;
    size_t now = 0;
    uchar out = 0;
    uchar ind = 0;
    while (now != a.size()) {
        std::vector<bool> code = b.getCode(a[now++]);
        for (size_t i = 0; i < code.size(); i++) {
            out <<= 1;
            out += code[i];
            ind++;
            if (ind == 8) {
                ans.push_back(out);
                ind = 0;
                out = 0;
            }
        }
    }
    if (ind != 0)
        ans.push_back(out << (8 - ind));
    return ans;
}

std::string unarchieve(std::string const &a, generator &b) {
    std::string ans;
    size_t ind = 0;
    uchar now;
    char res;
    uint64_t cnt = 0;
    while (ind != a.size()) {
        now = a[ind++];
        uchar lol = 0;
        for (int i = 0; i < 8; i++) {
            lol *= 2;
            lol += now % 2;
            now /= 2;
        }
        now = lol;
        for (int i = 0; i < 8; i++) {
            if (cnt == b.getTextSize())
                break;
            if (b.getReverseCode(now % 2, res)) {
                ans.push_back(res);
                cnt++;
            }
            now /= 2;
        }
    }
    return ans;
}

bool checkThis(std::string const &a, generator &b) {
    std::string codedString = archieve(a, b);
    std::string encodedString = unarchieve(codedString, b);
    bool ans1 = (encodedString == a);
    uint16_t n = b.getTreeSize();
    std::vector<uchar> treeEncode = b.encodeTree();
    std::vector<uchar> treeLeaves = b.encodeTreeLeaves();
    uint64_t text = b.getTextSize();
    generator test;
    test.generate(n, treeEncode, treeLeaves, text);
    encodedString = unarchieve(codedString, test);
    bool ans2 = (encodedString == a);
    return ans1 && ans2;
}

TEST(correctness, empty) {
    std::string test = "";
    OccurrencyMap freq;
    for (auto x : test)
        freq.add(static_cast<uchar>(x));
    generator testGenerator(freq);
    ASSERT_TRUE(checkThis(test, testGenerator));
}

const int AMOUNT = 1024 * 1024;
const int ITERATIONS = 100;

TEST(correctness, one_char) {
    for (char i = 'a'; i <= 'z'; i++) {
        std::string test;
        test.push_back(i);
        OccurrencyMap freq;
        for (auto x : test)
            freq.add(static_cast<uchar>(x));
        generator testGenerator(freq);
        ASSERT_TRUE(checkThis(test, testGenerator));
    }
    for (char i = 'a'; i <= 'z'; i++) {
        std::string test(AMOUNT, i);
        OccurrencyMap freq;
        for (auto x : test)
            freq.add(static_cast<uchar>(x));
        generator testGenerator(freq);
        ASSERT_TRUE(checkThis(test, testGenerator));
    }
}

TEST(correctness, test) {
    std::mt19937 randomer(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
    for (int temp = 0; temp < ITERATIONS; temp++) {
        std::string test;
        size_t range = randomer() % 100 + 1;
        for (int i = 0; i < AMOUNT; i++) {
            test.push_back(static_cast<char>(randomer() % range));
        }
        OccurrencyMap freq;
        for (auto x : test)
            freq.add(static_cast<uchar>(x));
        generator testGenerator(freq);
        ASSERT_TRUE(checkThis(test, testGenerator));
    }
}

TEST(incorrect, test) {
    generator test;
    ASSERT_ANY_THROW(test.generate(100500, std::vector<uchar>(2), std::vector<uchar>(16), 0));
    ASSERT_ANY_THROW(test.generate(15, std::vector<uchar>(2), std::vector<uchar>(10), 0));
    ASSERT_NO_THROW(test.generate(15, std::vector<uchar>(2), std::vector<uchar>(8), 0));
    ASSERT_ANY_THROW(test.generate(15, std::vector<uchar>(2), std::vector<uchar>(8), 10050));
    std::vector<uchar> testVec(2);
    testVec[0] = uchar(1);
    testVec[1] = uchar(254);
    std::vector<uchar> leaves(8);
    for(int i = 0; i < 8; i++)
        leaves[i] = char(i);
    ASSERT_NO_THROW(test.generate(15, testVec,leaves, 0));
}
