#pragma once
#include "cdmt.hpp"

#include <chrono>

namespace ch  = std::chrono;

class Timer {
  using clk = std::chrono::high_resolution_clock;
  private:
    string_t               sid;
    ch::time_point<clk>    start;
    ch::time_point<clk>    stop;
    float                  dur;
    float                  dur_sec;
    float                  dur_msec;

  public:
    Timer ( string_t _s = string_t("unnamed") ) : sid(_s) {}
    ~Timer () = default;

    void Start();
    void Stop();
    void StopPrint(std::ostream&);

    friend std::ostream& operator<< (std::ostream& os, const Timer& t);
};

