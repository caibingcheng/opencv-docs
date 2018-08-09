//author: cai bingcheng, 2018

#pragma once

#include <iostream>
#include <chrono>

class GetCostTime
{
private:
    std::chrono::steady_clock::time_point m_time_pre;
    std::chrono::steady_clock::time_point m_time_now;
    std::chrono::steady_clock::time_point m_time_line;
    std::chrono::duration<double> m_time_cost;
    std::chrono::steady_clock::time_point m_time_run_begin;
    std::chrono::steady_clock::time_point m_time_run_end;
    std::chrono::duration<double> m_time_run_cost;
public:

    GetCostTime();
    ~GetCostTime();
    //init
    void GetCostTimeInit();
    //calculate cost time after init, refreshing after using
    double CalCostTime();
    //calculate cost time without refreshing
    double CalTimeLine();
    //init
    void RunTime();
    //calculate cost time after init without refreshing
    double GetRunTime();
    //sleep for m ms
    bool WaitTimeMs(double ms);
};

namespace tim{
    extern GetCostTime run_time;
}
