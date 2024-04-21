#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <sstream>

using time_point = std::chrono::steady_clock::time_point;
typedef unsigned int uint;

enum NumberType
{
    Float,
    Int,
};

struct DataPoint
{
    NumberType type;
    union
    {
        float floatValue;
        int intValue;
    };
    time_point timestamp;

    DataPoint(float _floatValue) : type(NumberType::Float), floatValue(_floatValue), timestamp(std::chrono::steady_clock::now()){};
    DataPoint(int _intValue) : type(NumberType::Int), intValue(_intValue), timestamp(std::chrono::steady_clock::now()){};

    size_t ByteSize() const
    {
        switch (type)
        {
        case NumberType::Float:
            return sizeof(float);
        case NumberType::Int:
            return sizeof(int);
        }
        return 0;
    }
};

class Statistics
{
private:
    static std::unordered_map<std::string, std::vector<DataPoint>> m_targets;
    static time_point m_initTimestamp;
    static uint m_fileCounter;

public:
    static void Initalize() { m_initTimestamp = std::chrono::steady_clock::now(); }

    static size_t MemorySize()
    {
        size_t total = 0, arraySize = 0;
        for (const auto &[key, array] : m_targets)
        {
            arraySize = array.size();
            if (arraySize > 0)
                total += array[0].ByteSize() * arraySize;
        }

        return total;
    }

    static void Report(const std::string &target, int value) { m_targets[target].emplace_back(DataPoint(value)); }
    static void Report(const std::string &target, float value) { m_targets[target].emplace_back(DataPoint(value)); }

    static void Export(const std::string &basename)
    {
        if (MemorySize() == 0)
            return;

        time_point start = std::chrono::steady_clock::now();

        std::string filename = basename + std::to_string(m_fileCounter) + ".txt";
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Error: Failed to open file for writing." << std::endl; // FIXME: SDL logging
            return;
        }

        for (auto &[key, array] : m_targets)
        {
            // Export target values
            file << key;
            for (const auto &dataPoint : array)
            {
                file << ";";
                switch (dataPoint.type)
                {
                case NumberType::Float:
                    file << dataPoint.floatValue;
                    break;
                case NumberType::Int:
                    file << dataPoint.intValue;
                    break;
                }
            }
            file << "\n";

            // Export target timestamps
            file << key + "_Time";
            for (const auto &dataPoint : array)
            {
                std::chrono::duration<float, std::milli> timeSinceInit = dataPoint.timestamp - m_initTimestamp;
                file << ";" << timeSinceInit.count();
            }
            file << "\n";
        }

        // file.flush();
        // file.close();

        for (auto &[key, array] : m_targets)
            array.clear();
        ++m_fileCounter;

        time_point end = std::chrono::steady_clock::now();
        m_initTimestamp += end - start;
    }
};

std::unordered_map<std::string, std::vector<DataPoint>> Statistics::m_targets{};
time_point Statistics::m_initTimestamp{};
uint Statistics::m_fileCounter = 1;