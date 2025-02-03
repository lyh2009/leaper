#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

class LeapProfiler
{
public:
    static LeapProfiler& GetInstance()
    {
        static LeapProfiler s_instance;
        return s_instance;
    }

    void Start(const std::string& name)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        m_start_times[name].push_back(start_time);
    }
    void End(const std::string& name)
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto& starts  = m_start_times[name];
        if (!starts.empty())
        {
            auto start = starts.back();
            starts.pop_back();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start).count();
            m_total_times[name] += duration;
            m_call_counts[name]++;
        }
    }

    void Rest()
    {
        for (auto& pair : m_total_times) { pair.second = 0; }
        for (auto& pair : m_call_counts) { pair.second = 0; }
    }

    /// In microseconds
    std::unordered_map<std::string, long long> GetTotalTimes()
    {
        return m_total_times;
    }

    std::unordered_map<std::string, int> GetCallCounts()
    {
        return m_call_counts;
    }

private:
    std::unordered_map<std::string, std::vector<std::chrono::high_resolution_clock::time_point>> m_start_times;
    std::unordered_map<std::string, long long> m_total_times;
    std::unordered_map<std::string, int> m_call_counts;
};

class LeapScopedProfiler
{
public:
    LeapScopedProfiler(const std::string& name) : m_name(name)
    {
        LeapProfiler::GetInstance().Start(m_name);
    }

    ~LeapScopedProfiler()
    {
        LeapProfiler::GetInstance().End(m_name);
    }

private:
    std::string m_name;
};

#define LEAP_PROFILER_SCOPE(name) LeapScopedProfiler __profiler##__LINE__(name)
