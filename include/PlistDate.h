//
// Created by alex on 13.11.2021.
//

#ifndef PARSER_PLISTDATE_H
#define PARSER_PLISTDATE_H


#include <ctime>
#include <string>

class Date
{
public:
    Date();

    Date(int month, int day, int year, int hour24, int minute, int second, bool UTC);

    void set(int month, int day, int year, int hour24, int minute, int second, bool UTC);

    void setToCurrentTime();

    time_t secondsSinceDate(const Date& startDate) const;

    // returns -1 : first < second
    //          0 : first = second
    //          1 : first > second

    static int compare(const Date& first, const Date& second);

    bool operator > (const Date& rhs) const;

    bool operator < (const Date& rhs) const;

    bool operator == (const Date& rhs) const;

    // iso 8601 date string convention
    std::string timeAsXMLConvention() const;

    // iso 8601 date string convention
    void setTimeFromXMLConvention(const std::string& timeString);

    // Apple epoch is # of seconds since  01-01-2001. So we need to add the
    // number of seconds since 01-01-1970 which is proper unix epoch

    void setTimeFromAppleEpoch(double appleTime);

    time_t timeAsEpoch() const;

    // We need to subtract the number of seconds between 01-01-2001 and
    // 01-01-1970 to get Apple epoch from unix epoch

    double timeAsAppleEpoch() const;


private:

    // stored as unix epoch, number of seconds since 01-01-1970
    time_t _time;
};


#endif //PARSER_PLISTDATE_H
