#pragma once

#include <chrono>

namespace NHTV {
template <typename Clock> struct Timer;

using SysTimer = Timer<std::chrono::system_clock>;
using SteadyTimer = Timer<std::chrono::steady_clock>;
using HiResTimer = Timer<std::chrono::high_resolution_clock>;

using Nano = std::nano;
using Micro = std::micro;
using Milli = std::milli;
using Sec = std::ratio<1>;
using Min = std::ratio<60>;
using Hour = std::ratio<3600>;

template <typename Res, typename Ratio>
using Time = std::chrono::duration<Res, Ratio>;

using NanoTime = Time<double, Nano>;
using MicroTime = Time<double, Micro>;
using MilliTime = Time<double, Milli>;
using SecTime = Time<double, Sec>;
using MinTime = Time<double, Min>;
using HourTime = Time<double, Hour>;

template <typename Clock> using TimePoint = std::chrono::time_point<Clock>;

template <typename Clock> struct Timer {
  Timer() : m_tpoint(Clock::now()) {}

  void reset();

  double seconds() const;
  double milliseconds() const;
  double microseconds() const;

  template <typename Ratio> Time<double, Ratio> elapsed() const;

private:
  TimePoint<Clock> m_tpoint;
};

template <typename Clock> inline void Timer<Clock>::reset() {
  m_tpoint = Clock::now();
}

template <typename Clock> inline double Timer<Clock>::seconds() const {
  return SecTime(elapsed<Sec>()).count();
}

template <typename Clock> inline double Timer<Clock>::milliseconds() const {
  return MilliTime(elapsed<Milli>()).count();
}

template <typename Clock> inline double Timer<Clock>::microseconds() const {
  return MicroTime(elapsed<Micro>()).count();
}

template <typename Clock>
template <typename Ratio>
inline Time<double, Ratio> Timer<Clock>::elapsed() const {
  return Clock::now() - m_tpoint;
}

/*bool operator ==(Time left, Time right);
bool operator !=(Time left, Time right);
bool operator <(Time left, Time right);
bool operator >(Time left, Time right);
bool operator <=(Time left, Time right);
bool operator >=(Time left, Time right);
Time operator -(Time right);
Time operator +(Time left, Time right);
Time& operator +=(Time& left, Time right);
Time operator -(Time left, Time right);
Time& operator -=(Time& left, Time right);
Time operator *(Time left, float right);
Time operator *(Time left, s64 right);
Time operator *(float left, Time right);
Time operator *(s64 left, Time right);
Time& operator *=(Time& left, float right);
Time& operator *=(Time& left, s64 right);
Time operator /(Time left, float right);
Time operator /(Time left, s64 right);
Time& operator /=(Time& left, float right);
Time& operator /=(Time& left, s64 right);
float operator /(Time left, Time right);
Time operator %(Time left, Time right);
Time& operator %=(Time& left, Time right);*/

/*bool operator ==(Time left, Time right)
{
    return left.asMicroseconds() == right.asMicroseconds();
}

bool operator !=(Time left, Time right)
{
    return left.asMicroseconds() != right.asMicroseconds();
}

bool operator <(Time left, Time right)
{
    return left.asMicroseconds() < right.asMicroseconds();
}

bool operator >(Time left, Time right)
{
    return left.asMicroseconds() > right.asMicroseconds();
}

bool operator <=(Time left, Time right)
{
    return left.asMicroseconds() <= right.asMicroseconds();
}

bool operator >=(Time left, Time right)
{
    return left.asMicroseconds() >= right.asMicroseconds();
}

Time operator -(Time right)
{
    return microseconds(-right.asMicroseconds());
}

Time operator +(Time left, Time right)
{
    return microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time& operator +=(Time& left, Time right)
{
    return left = left + right;
}

Time operator -(Time left, Time right)
{
    return microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time& operator -=(Time& left, Time right)
{
    return left = left - right;
}

Time operator *(Time left, float right)
{
    return seconds(left.asSeconds() * right);
}

Time operator *(Time left, s64 right)
{
    return microseconds(left.asMicroseconds() * right);
}

Time operator *(float left, Time right)
{
    return right * left;
}

Time operator *(s64 left, Time right)
{
    return right * left;
}

Time& operator *=(Time& left, float right)
{
    return left = left * right;
}

Time& operator *=(Time& left, s64 right)
{
    return left = left * right;
}

Time operator /(Time left, float right)
{
    return seconds(left.asSeconds() / right);
}

Time operator /(Time left, s64 right)
{
    return microseconds(left.asMicroseconds() / right);
}

Time& operator /=(Time& left, float right)
{
    return left = left / right;
}

Time& operator /=(Time& left, s64 right)
{
    return left = left / right;
}

float operator /(Time left, Time right)
{
    return left.asSeconds() / right.asSeconds();
}

Time operator %(Time left, Time right)
{
    return microseconds(left.asMicroseconds() % right.asMicroseconds());
}

Time& operator %=(Time& left, Time right)
{
    return left = left % right;
}*/
}
