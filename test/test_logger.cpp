#include "gtest/gtest.h"
#include "flog/flog.hpp"

#include <atomic>
#include <string>
#include <random>
#include <thread>
#include <iomanip>
#include <iostream>

using namespace flog;
using namespace std;

/* read last line of file */
vector<string> ReadLastN(const char *file, const int line = 1)
{
    ifstream read(file, ios_base::ate);//open file
    vector<string> ret;
    int length = 0;
    int lineCount = 0;
    
    char c = '\0';
    
    if(read.is_open())
    {
        length = read.tellg();
        // loop backward over the file
        auto skipLine = [&](int &i) {
            for (; i > 0; i--)
            {
                read.seekg(i);
                c = read.get();
                if (c != '\n')
                    break;
            }
        };
        int i = length - 1;
        skipLine(i);
        for(; i > 0; i--)
        {
            read.seekg(i);
            c = read.get();
            if (c == '\n')//new line?
            {
                if (++lineCount == line)
                    break;
                skipLine(i);
            }
        }
        string current;
        while (getline(read, current))
        {
            /* strip time */
            auto pos = current.find(']');
            pos = pos == string::npos ? 0 : 1 + pos;
            ret.push_back(string(current.begin() + pos, current.end()));
        }
    }
    return ret;
}

TEST(SingleThread, NormalOutput)
{
    string str = "Output";
    LOG() << str + "\n";
    EXPECT_EQ(ReadLastN("log").front(), str);
}

TEST(SingleThread, EndlManip)
{
    string str = "Endl Manipulator";
    LOG() << str << endl;
    EXPECT_EQ(ReadLastN("log").front(), str);
}

TEST(SingleThread, TwoLines)
{
    vector<string> strs = { "One", "Two" };
    LOG() << strs[0] << endl
          << strs[1] << endl;
    EXPECT_EQ(ReadLastN("log", strs.size()), strs);
}

TEST(SingleThread, ChineseCharacter)
{
    string str = "中文字符";
    LOG() << str << endl;
    EXPECT_EQ(ReadLastN("log").front(), str);
}

TEST(SingleThread, Number)
{
    static random_device rnd_dev;
    static mt19937 g(rnd_dev());
    uniform_int_distribution<int> uniform(numeric_limits<int>::min(), numeric_limits<int>::max());
    vector<int> nums(10);
    string concat;
    for (auto &n : nums)
    {
        n = uniform(g);
        concat += to_string(n);
    }
    LOG() << nums[0] << nums[1] << nums[2] << nums[3] << nums[4]
          << nums[5] << nums[6] << nums[7] << nums[8] << nums[9] << endl;
    EXPECT_EQ(ReadLastN("log").front(), concat);
}

TEST(SingleThread, Vector)
{
    vector<int> container = { 1, 2, 3 };
    LOG() << "Print out vector" << endl << container;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1, 2, 3");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, VectorOfFloat)
{
    vector<float> container = { 1.f, 2.f, 3.f };
    LOG() << "Print out vector of float" << endl
          << fixed << setprecision(1) << container;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1.0, 2.0, 3.0");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, Deque)
{
    deque<int> container = { 1, 2, 3 };
    LOG() << "Print out deque" << endl << container;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1, 2, 3");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, List)
{
    list<int> container = { 1, 2, 3 };
    LOG() << "Print out list" << endl << container;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1, 2, 3");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, Set)
{
    set<int> container = { 1, 2, 3 };
    LOG() << "Print out set" << endl << container;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1, 2, 3");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, Multiset)
{
    multiset<int> container = { 1, 1, 1 };
    LOG() << "Print out multiset" << endl << container;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1, 1, 1");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, Array)
{
    array<int, 3> arr = { 1, 2, 3 };
    LOG() << "Print out array" << endl << arr;
    auto log = ReadLastN("log", 3);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    1, 2, 3");
    EXPECT_EQ(log[2], "}");
}

TEST(SingleThread, VectorTwoDim)
{
    vector<vector<int>> vec = { {1, 2, 3}, {4, 5}, {6} };
    LOG() << "Print out 2d vector" << endl << vec;
    auto log = ReadLastN("log", 11);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    {");
    EXPECT_EQ(log[2], "        1, 2, 3");
    EXPECT_EQ(log[3], "    }");
    EXPECT_EQ(log[4], "    {");
    EXPECT_EQ(log[5], "        4, 5");
    EXPECT_EQ(log[6], "    }");
    EXPECT_EQ(log[7], "    {");
    EXPECT_EQ(log[8], "        6");
    EXPECT_EQ(log[9], "    }");
    EXPECT_EQ(log[10],"}");
}

TEST(SingleThread, Map)
{
    map<string, int> dict = {
        make_pair("one", 1),
        make_pair("two", 2),
        make_pair("three", 3)
    };

    LOG() << "Print out map" << endl << dict;
    auto log = ReadLastN("log", 5);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[4], "}");
}

TEST(SingleThread, Multimap)
{
    multimap<string, int> dict = {
        make_pair("one", 1),
        make_pair("one", 2),
        make_pair("one", 3)
    };

    LOG() << "Print out multimap" << endl << dict;
    auto log = ReadLastN("log", 5);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[4], "}");
}

TEST(SingleThread, MapNested)
{
    map<string, map<string, int>> dict = {
        make_pair(
            "ONE", 
            map<string, int>{
                make_pair("one", 1)
            }
        ),
        make_pair(
            "TWO", 
            map<string, int>{
                make_pair("two", 2)
            }
        ),
        make_pair(
            "VAR", 
            map<string, int>{
                make_pair("three", 3),
                make_pair("four", 4)
            }
        )
    };

    LOG() << "Print out nested map" << endl << dict;
    auto log = ReadLastN("log", 15);
    EXPECT_EQ(log[0],  "{");
    EXPECT_EQ(log[2],  "    {");
    EXPECT_EQ(log[4],  "    }");
    EXPECT_EQ(log[6],  "    {");
    EXPECT_EQ(log[8],  "    }");
    EXPECT_EQ(log[10], "    {");
    EXPECT_EQ(log[13], "    }");
    EXPECT_EQ(log[14], "}");
}

TEST(SingleThread, UnorderedMap)
{
    unordered_map<string, int> dict = {
        make_pair("one", 1),
        make_pair("two", 2),
        make_pair("three", 3)
    };

    LOG() << "Print out unordered map" << endl << dict;
    auto log = ReadLastN("log", 5);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[4], "}");
}

TEST(SingleThread, UnorderedMapNested)
{
    unordered_map<string, unordered_map<string, int>> dict = {
        make_pair(
            "ONE", 
            unordered_map<string, int>{
                make_pair("one", 1)
            }
        ),
        make_pair(
            "TWO", 
            unordered_map<string, int>{
                make_pair("two", 2)
            }
        ),
        make_pair(
            "VAR", 
            unordered_map<string, int>{
                make_pair("three", 3),
                make_pair("four", 4)
            }
        )
    };

    LOG() << "Print out nested unordered map" << endl << dict;
    auto log = ReadLastN("log", 15);
    EXPECT_EQ(log[0],  "{");
    for (auto it = log.begin() + 1; it != log.begin() + 14; it++)
    {
        if (*it == "    {")
            EXPECT_TRUE(*(it + 2) == "    }" || *(it + 3) == "    }");
    }
    EXPECT_EQ(log[14], "}");
}

TEST(SingleThread, MapOfVector)
{
    map<string, vector<int>> dict = {
        make_pair(
            "one", 
            vector<int>{1,2,3}
        ),
        make_pair(
            "two",
            vector<int>{4,5}
        )
    };
    LOG() << "Print out map of vector of int" << endl << dict;
    auto log = ReadLastN("log", 10);
    EXPECT_EQ(log[0], "{");
    EXPECT_EQ(log[1], "    one: ");
    EXPECT_EQ(log[2], "    {");
    EXPECT_EQ(log[3], "        1, 2, 3");
    EXPECT_EQ(log[4], "    }");
    EXPECT_EQ(log[5], "    two: ");
    EXPECT_EQ(log[6], "    {");
    EXPECT_EQ(log[7], "        4, 5");
    EXPECT_EQ(log[8], "    }");
    EXPECT_EQ(log[9], "}");
}

TEST(SingleThread, Fusion)
{
    string str0 = "This is a int: ";
    int inum = -128;
    string str1 = "This is a float";
    float fnum = -256.2f;
    string concat = str0 + to_string(inum) + str1 + to_string(fnum);
    /* std::to_string guarentee fix 6 digits per c++ standard */
    LOG() << str0 << inum << str1 << fixed << setprecision(6) << fnum << endl;
    EXPECT_EQ(ReadLastN("log").front(), concat);
}

TEST(SingleThread, Manipulator)
{
    string stars(10, '*');
    LOG() << setfill('*') << setw(stars.size()) << '*' << endl;
    EXPECT_EQ(ReadLastN("log").front(), stars);
}

TEST(SingleThread, ConditionalLog)
{
    string on = "This line exists";
    string off = "This line disappears";
    LOG(true) << on << endl;
    LOG(false) << off << endl;
    EXPECT_EQ(ReadLastN("log").front(), on);
}

TEST(SingleThread, Filter)
{
    vector<string> traces = { "trace1", "trace2", "trace3" };
    vector<string> warnings = { "warning1", "warning2", "warning3" };
    vector<string> fatals = { "fatal1", "fatal2" };
    LOG_CONFIG_FILTER(warning);
    LOG(trace) << traces[0] << endl
               << traces[1] << endl
               << traces[2] << endl;
    auto LastThree = ReadLastN("log", traces.size());
    for (auto &line : LastThree)
        EXPECT_TRUE(std::find(traces.begin(), traces.end(), line) == traces.end());
    LOG(warning) << warnings[0] << endl
                 << warnings[1] << endl
                 << warnings[2] << endl;
    EXPECT_EQ(ReadLastN("log", warnings.size()), warnings);
    LOG(fatal) << fatals[0] << endl
               << fatals[1] << endl;
    EXPECT_EQ(ReadLastN("log", fatals.size()), fatals);
    /* restore filter level to all */
    LOG_CONFIG_FILTER(trace);
}

TEST(MultiThread, SingleStreamConsistency)
{
    string str = "Multi thread validation";
    int num_thread = 10;
    vector<thread> threads(10);
    for (auto &t : threads)
        t = thread([&str]() {
            LOG() << str << endl;
        });
    for (auto &t : threads)
        if (t.joinable())
            t.join();
    auto LastTenLines = ReadLastN("log", num_thread);
    EXPECT_EQ(LastTenLines.size(), num_thread);
    for (auto &line : LastTenLines)
        EXPECT_EQ(line, str);
}

TEST(MultiThread, AtomicConsistency)
{
    vector<string> strs = { "One", "Two", "Three" };
    int num_thread = 10;
    vector<thread> threads(10);
    for (auto &t : threads)
        t = thread([&strs]() {
            LOG() << strs[0] << endl
                  << strs[1] << endl
                  << strs[2] << endl;
        });
    for (auto &t : threads)
        if (t.joinable())
            t.join();
    auto Last = ReadLastN("log", num_thread * strs.size());
    EXPECT_EQ(Last.size(), num_thread * strs.size());
    for (unsigned i = 0; i < Last.size(); i += strs.size())
        for (unsigned j = 0; j < strs.size(); j++)
            EXPECT_EQ(strs[j], Last[i + j]);
}

TEST(MultiThread, Performance)
{
    auto thread_count = thread::hardware_concurrency();
    int num_lines = 1000000;
    atomic<int> counter = { 0 };
    vector<thread> threads(thread_count);
    /* disable timestamp */
    //LOG_CONFIG_FILTER(hide_all);
    LOG_CONFIG_TIMESTAMP(false);
    LOG_CONFIG_FORCE_FLUSH(true);
    for (auto &t : threads)
        t = thread([&]() {
        while (true)
        {
            int line_counter = ++counter;
            if (line_counter > num_lines) break;
            LOG() << "This is some text for your pleasure" << line_counter << endl;
        }
    });
    for (auto &t : threads)
        if (t.joinable())
            t.join();

    EXPECT_TRUE(true);
    LOG_CONFIG_TIMESTAMP(true);
    LOG_CONFIG_FORCE_FLUSH(true);
}

TEST(MultiThread, NoFlushPerformance)
{
    auto thread_count = thread::hardware_concurrency();
    int num_lines = 1000000;
    atomic<int> counter = { 0 };
    vector<thread> threads(thread_count);
    /* disable timestamp */
    LOG_CONFIG_TIMESTAMP(false);
    LOG_CONFIG_FORCE_FLUSH(false);
    for (auto &t : threads)
        t = thread([&]() {
        while (true)
        {
            int line_counter = ++counter;
            if (line_counter > num_lines) break;
            LOG() << "This is some text for your pleasure" << line_counter << "\n";
        }
    });
    for (auto &t : threads)
        if (t.joinable())
            t.join();

    EXPECT_TRUE(true);
    LOG_CONFIG_TIMESTAMP(true);
    LOG_CONFIG_FORCE_FLUSH(true);
}

TEST(MultiThread, FootprintPerformance)
{
    auto thread_count = thread::hardware_concurrency();
    int num_lines = 1000000;
    atomic<int> counter = { 0 };
    vector<thread> threads(thread_count);
    /* disable timestamp */
    LOG_CONFIG_FILTER(hide_all);
    for (auto &t : threads)
        t = thread([&]() {
        while (true)
        {
            int line_counter = ++counter;
            if (line_counter > num_lines) break;
            LOG() << "This is some text for your pleasure" << line_counter << "\n";
        }
    });
    for (auto &t : threads)
        if (t.joinable())
            t.join();

    EXPECT_TRUE(true);
    LOG_CONFIG_FILTER(trace);
}
