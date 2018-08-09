//author: cai bingcheng, 2018

#include "GetCostTime.h"

using namespace std;

GetCostTime::GetCostTime()
{

}

GetCostTime::~GetCostTime()
{
  // std::cout << "-- GetCostTime Destructor successfully!" << std::endl;
}

void GetCostTime::GetCostTimeInit()
{
  m_time_now = std::chrono::steady_clock::now();
}

double GetCostTime::CalCostTime()
{
  m_time_pre = m_time_now;
  m_time_now = std::chrono::steady_clock::now();
  m_time_cost = std::chrono::duration_cast<chrono::duration<double>>(m_time_now - m_time_pre);
  return m_time_cost.count();
}

double GetCostTime::CalTimeLine()
{
  m_time_line = std::chrono::steady_clock::now();
  m_time_cost = std::chrono::duration_cast<chrono::duration<double>>(m_time_line - m_time_now);
  return m_time_cost.count();
}

void GetCostTime::RunTime()
{
  m_time_run_begin = std::chrono::steady_clock::now();
}

double GetCostTime::GetRunTime()
{
  m_time_run_end = std::chrono::steady_clock::now();
  m_time_run_cost = std::chrono::duration_cast<chrono::duration<double>>(m_time_run_end - m_time_run_begin);
  return m_time_run_cost.count();
}

//do not init while use this function
bool GetCostTime::WaitTimeMs(double ms)
{
  GetCostTimeInit();
  while(CalTimeLine() * 1000 < ms);
  return true;
}

namespace tim{
  GetCostTime run_time;
}
