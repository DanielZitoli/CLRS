#pragma once

#include "helpers/printing_helpers.h"

#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <stdexcept>


class Profiler {
public:
  Profiler() {}

  ~Profiler() {}

  void start(const std::string& id) {
    startTimes[id].push_back(std::chrono::high_resolution_clock::now());
  }

  void end(const std::string& id) {
    auto endTime = std::chrono::high_resolution_clock::now();
    auto startTime = startTimes[id].back();
    startTimes[id].pop_back();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    TimingInfo& info = timings[id];
    info.totalTime += duration;
    info.totalOccurrences++;
    if (duration > info.maxTime) {
      info.maxTime = duration;
    }
  }

  void results(const std::string& testName = std::string()) {
    if (!testName.empty()) {
      std::cout << "RESULTS for test: " << testName;
    } else {
      std::cout << "RESULTS";
    }
    std::cout << std::endl << std::endl;

    TableDisplay table{30, 15, 15, 15}; 
    table.printHeader("Functions", "Occurences", "Total Time", "Max Time"); 
    for (auto& entry : timings) {
      table.printRow(entry.first,
                    entry.second.totalOccurrences,
                    formatFloatPrecision(static_cast<double>(entry.second.totalTime.count()) / 1000, 2),
                    formatFloatPrecision(static_cast<double>(entry.second.maxTime.count()) / 1000, 2));
    }
    std::cout << std::endl;

    // check if a START was called without a matching END
    if (!std::all_of(startTimes.begin(), startTimes.end(), [](auto const& x) { return x.second.empty(); })) {
      throw std::logic_error("Not every start time is matched with an end time.");
    }

    // if start/end time logic is correct, reset profiler
    timings.clear();
    startTimes.clear();
  }

private:
  struct TimingInfo {
    std::chrono::microseconds totalTime{0};
    std::chrono::microseconds maxTime{0};
    int totalOccurrences{0};
  };

  std::unordered_map<std::string, std::vector<std::chrono::high_resolution_clock::time_point>> startTimes;
  std::unordered_map<std::string, TimingInfo> timings;
};

// Singleton pattern to ensure only one profiler instance exists.
extern Profiler profiler;

#define PROFILE_START(id) profiler.start(id)
#define PROFILE_END(id) profiler.end(id)
#define PROFILE_RESULTS(test_name) profiler.results(test_name)