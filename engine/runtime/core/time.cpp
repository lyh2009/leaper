#include "time.h"
#include <GLFW/glfw3.h>

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

float Leaper::Time::m_delta_time = 0.0f;

float current_frame = 0.0f;
float last_frame = 0.0f;

const std::string Leaper::Time::GetTime(const std::string &format) {
  std::chrono::system_clock::system_clock::time_point tp =
      std::chrono::system_clock::system_clock::now();

  time_t raw_time = std::chrono::system_clock::system_clock::to_time_t(tp);

  struct tm *timeinfo = std::localtime(&raw_time);

  std::stringstream ss;
  ss << std::put_time(timeinfo, format.c_str());

  return ss.str();
}

void Leaper::Time::Begin() { current_frame = glfwGetTime(); }

void Leaper::Time::End() {
  m_delta_time = current_frame - last_frame;
  last_frame = current_frame;
}