/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include <time.h>
#include "../Game.h"
#include "../core/Math.hpp"
#include "Date.h"
#include "StringIds.h"

uint16_t gDateMonthTicks;
uint16_t gDateMonthsElapsed;

// rct2: 0x00993988
const int16_t days_in_month[MONTH_COUNT] = { 31, 30, 31, 30, 31, 31, 30, 31 };

// clang-format off
const rct_string_id DateFormatStringIds[] = {
    STR_DATE_FORMAT_DAY_MONTH_YEAR,
    STR_DATE_FORMAT_MONTH_DAY_YEAR,
    STR_DATE_FORMAT_YEAR_MONTH_DAY,
    STR_DATE_FORMAT_YEAR_DAY_MONTH
};

const rct_string_id DateFormatStringFormatIds[] = {
    STR_DATE_FORMAT_DMY,
    STR_DATE_FORMAT_MDY,
    STR_DATE_FORMAT_YMD,
    STR_DATE_FORMAT_YDM
};
// clang-format on

openrct_timeofday gRealTimeOfDay;

int32_t date_get_day_of_month(int32_t month)
{
    return ((gDateMonthTicks * days_in_month[month]) >> 16) & 0xFF;
}

int32_t date_get_month(int32_t months)
{
    return months % MONTH_COUNT;
}

int32_t date_get_year(int32_t months)
{
    return months / MONTH_COUNT;
}

int32_t date_get_total_months(int32_t month, int32_t year)
{
    return (year - 1) * MONTH_COUNT + month;
}

int32_t date_get_elapsed_days(int32_t months, int32_t days) {
    int32_t sum = 0;
    for (int i = 0; i < months; i++) {
        sum += days_in_month[i % MONTH_COUNT];
    }
    return sum + days;
}

/**
 *
 *  rct2: 0x006C4494
 */
void date_reset()
{
    gDateMonthsElapsed = 0;
    gDateMonthTicks = 0;
    gCurrentTicks = 0;
}

void date_set(int32_t year, int32_t month, int32_t day)
{
    year = Math::Clamp(1, year, 8192);
    month = Math::Clamp(1, month, (int)MONTH_COUNT);
    day = Math::Clamp(1, day, (int)days_in_month[month - 1]);
    gDateMonthsElapsed = (year - 1) * MONTH_COUNT + month - 1;
    gDateMonthTicks = 0x10000 / days_in_month[month - 1] * (day - 1) + 4;
}

void date_update()
{
    int32_t monthTicks = gDateMonthTicks + 4;
    if (monthTicks >= 0x10000)
    {
        gDateMonthTicks = 0;
        gDateMonthsElapsed++;
    }
    else
    {
        gDateMonthTicks = floor2((uint16_t)monthTicks, 4);
    }
}

void date_update_real_time_of_day()
{
    time_t timestamp = time(nullptr);
    struct tm *now = localtime(&timestamp);

    gRealTimeOfDay.second = now->tm_sec;
    gRealTimeOfDay.minute = now->tm_min;
    gRealTimeOfDay.hour = now->tm_hour;
}

bool date_is_day_start(int32_t monthTicks)
{
    if (monthTicks < 4)
    {
        return false;
    }
    int32_t prevMonthTick = monthTicks - 4;
    int32_t currentMonth = date_get_month(gDateMonthsElapsed);
    int32_t currentDaysInMonth = days_in_month[currentMonth];
    return ((currentDaysInMonth * monthTicks) >> 16 != (currentDaysInMonth * prevMonthTick) >> 16);
}

bool date_is_week_start(int32_t monthTicks)
{
    return (monthTicks & 0x3FFF) == 0;
}

bool date_is_fortnight_start(int32_t monthTicks)
{
    return (monthTicks & 0x7FFF) == 0;
}

bool date_is_month_start(int32_t monthTicks)
{
    return (monthTicks == 0);
}
