//
// Created by alex on 13.11.2021.
//

#include "../include/PlistDate.h"
#include <stdexcept>
#include <stdio.h>

Date::Date()
{
    setToCurrentTime();
}

Date::Date(int month, int day, int year, int hour24, int minute, int second, bool UTC)
{
    set(month, day, year, hour24, minute, second, UTC);
}

void Date::set(int month, int day, int year, int hour24, int minute, int second, bool UTC)
{
    struct tm tmTime;
    tmTime.tm_hour = hour24;
    tmTime.tm_mday = day;
    tmTime.tm_year = year - 1900;
    tmTime.tm_sec = second;
    tmTime.tm_mon = month - 1;
    tmTime.tm_min = minute;

    //get proper day light savings.

    time_t loc = time(NULL);
    struct tm tmLoc = *localtime(&loc);
    //std::cout<<"tmLoc.tm_isdst = "<<tmLoc.tm_isdst<<std::endl;
    tmTime.tm_isdst = tmLoc.tm_isdst;

    if(UTC)
    {
        //_time = timegm(&tmTime);

        tmTime.tm_isdst = 0;
        _time = mktime(&tmTime);
        if(_time < -1){
            // error
        }

        // don't have timegm for all systems so here's a portable way to do it.

        struct tm tmTimeTemp;
#if defined(_WIN32) || defined(_WIN64)
        gmtime_s(&tmTimeTemp, &_time);
#else
        gmtime_r(&_time, &tmTimeTemp);
#endif

        time_t timeTemp = mktime(&tmTimeTemp);

        time_t diff = _time - timeTemp;
        _time += diff;
    }
    else
    {
        _time = mktime(&tmTime);
        if(_time < -1) {
            // error
        }
    }
}

void Date::setToCurrentTime()
{
    _time = time(nullptr);
}

time_t Date::secondsSinceDate(const Date& startDate) const
{
    return _time - startDate.timeAsEpoch();
}

// returns -1 : first < second
//          0 : first = second
//          1 : first > second

int Date::compare(const Date& first, const Date& second)
{
    if(first.timeAsEpoch() < second.timeAsEpoch())
        return -1;
    else if (first.timeAsEpoch() == second.timeAsEpoch())
        return 0;
    else
        return 1;
}

bool Date::operator > (const Date& rhs) const
{
    if(compare(*this, rhs) == 1)
        return true;
    else
        return false;
}

bool Date::operator < (const Date& rhs) const
{
    if(compare(*this, rhs) == -1)
        return true;
    else
        return false;
}

bool Date::operator == (const Date& rhs) const
{
    if(compare(*this, rhs) == 0)
        return true;
    else
        return false;
}

// iso 8601 date string convention
std::string Date::timeAsXMLConvention() const
{
    char result[21];
    struct tm tmTime;

    // use thread safe versions here.  Notice that arguments
    // are reversed for windows version
#if defined(_WIN32) || defined(_WIN64)
    gmtime_s(&tmTime, &_time);
#else
    gmtime_r(&_time, &tmTime);
#endif
    // %F and %T not portable so using %Y-%m-%d and %H:%M:%S instead
    strftime(&result[0], 21, "%Y-%m-%dT%H:%M:%SZ", &tmTime);
    return std::string(&result[0]);
}

// iso 8601 date string convention
void Date::setTimeFromXMLConvention(const std::string& timeString)
{
    int month, day, year, hour24, minute, second;

    // parse date string.  E.g.  2011-09-25T02:31:04Z
    sscanf(timeString.c_str(), "%4d-%2d-%2dT%2d:%2d:%2dZ", &year, &month, &day, &hour24, &minute, &second);
    set(month, day, year, hour24, minute, second, true);

}

// Apple epoch is # of seconds since  01-01-2001. So we need to add the
// number of seconds since 01-01-1970 which is proper unix epoch

void Date::setTimeFromAppleEpoch(double appleTime)
{
    _time = time_t(978307200 + appleTime);
}

time_t Date::timeAsEpoch() const
{
    return _time;
}

// We need to subtract the number of seconds between 01-01-2001 and
// 01-01-1970 to get Apple epoch from unix epoch

double Date::timeAsAppleEpoch() const
{
    return ((double) _time - 978307200);
}
