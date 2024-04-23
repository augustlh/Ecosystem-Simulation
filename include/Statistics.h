#pragma once

#include "Ecosim.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <sstream>

using time_point = std::chrono::steady_clock::time_point;

/// @brief A enum describing the datatypes avaliable when storing data
enum NumberType
{
    Float,
    Int,
};

/// @brief A single datapoint containing either a float or an int, and a timestamp for when the value was stored
struct DataPoint
{
    /// @brief The type of the union data
    NumberType type;

    /// @brief A union, which stores its properties in the same space in memory
    union
    {
        float floatValue;
        int intValue;
    };

    /// @brief The time when the datapoint was stored
    time_point timestamp;

    /// @brief Create a float-datapoint
    /// @param _floatValue The float-value
    DataPoint(float _floatValue) : type(NumberType::Float), floatValue(_floatValue), timestamp(std::chrono::steady_clock::now()){};

    /// @brief Create an int-datapoint
    /// @param _intValue The int-value
    DataPoint(int _intValue) : type(NumberType::Int), intValue(_intValue), timestamp(std::chrono::steady_clock::now()){};
};

/// @brief A static class used to repport datavalues across the simulation
class Statistics
{
private:
    /// @brief A dictionary containing all of the data
    static std::unordered_map<std::string, std::vector<DataPoint>> m_targets;

    /// @brief The timestamp when the `Statistics`-class was initialized
    static time_point m_initTimestamp;

    /// @brief A counter to keep track of the number of exported files, in order to not override previous data
    static uint m_fileCounter;

public:
    /// @brief Sets the timepoint to which all other timepoints will be relative to
    static void Initalize() { m_initTimestamp = std::chrono::steady_clock::now(); }

    /// @brief Calculates the total memory size of the current stored data
    /// @return The size in bytes
    static size_t MemorySize()
    {
        size_t total = 0, arraySize = 0;
        for (const auto &[key, array] : m_targets)
        {
            arraySize = array.size();
            if (arraySize > 0)
                total += arraySize * sizeof(DataPoint);
        }

        return total;
    }

    /// @brief Report an int-datavalue
    /// @param target The target to add the value to. When exporting the data this will be the header
    /// @param value The value to store
    static void Report(const std::string &target, int value) { m_targets[target].emplace_back(DataPoint(value)); }

    /// @brief Report a float-datavalue
    /// @param target The target to add the value to. When exporting the data this will be the header
    /// @param value The value to store
    static void Report(const std::string &target, float value) { m_targets[target].emplace_back(DataPoint(value)); }

    /// @brief Export all data
    /// @param basename The base filename
    static void Export(const std::string &basename)
    {
        if (MemorySize() == 0)
            return;

        time_point start = std::chrono::steady_clock::now();

        std::string filename = basename + std::to_string(m_fileCounter) + ".txt";
        std::ofstream file(filename);
        if (!file.is_open())
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Error: Failed to open file for writing.");
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

        for (auto &[key, array] : m_targets)
            array.clear();
        ++m_fileCounter;

        time_point end = std::chrono::steady_clock::now();
        m_initTimestamp += end - start;
    }
};

// Set default values of the static class
std::unordered_map<std::string, std::vector<DataPoint>> Statistics::m_targets{};
time_point Statistics::m_initTimestamp{};
uint Statistics::m_fileCounter = 1;