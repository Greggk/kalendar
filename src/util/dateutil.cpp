#include "dateutil.h"

#include <QDebug>

//static string months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
static string week_days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

DateUtil::DateUtil()
{

}

string DateUtil::get_literal_month(int m) {
    QString Month;
    switch (m) {

    case 1:

        Month = QObject::tr("January");
        return Month.toStdString();

    case 2:

        Month = QObject::tr("February");
        return Month.toStdString();

    case 3:

        Month = QObject::tr("March");
        return Month.toStdString();

    case 4:

        Month = QObject::tr("April");
        return Month.toStdString();

    case 5:

        Month = QObject::tr("May");
        return Month.toStdString();

    case 6:

        Month = QObject::tr("June");
        return Month.toStdString();

    case 7:

        Month = QObject::tr("July");
        return Month.toStdString();

    case 8:

        Month = QObject::tr("August");
        return Month.toStdString();

    case 9:

        Month = QObject::tr("September");
        return Month.toStdString();

    case 10:

        Month = QObject::tr("October");
        return Month.toStdString();

    case 11:

        Month = QObject::tr("November");
        return Month.toStdString();

    case 12:

        Month = QObject::tr("December");
        return Month.toStdString();

    default:

        return string("");
    }
}

Date DateUtil::get_current_date() {
    return date_from_timestamp(static_cast <unsigned long> (time(NULL)));
}

string DateUtil::numeric2literal_day_of_week(int d) {
    QString Day;
    switch (d) {

    case 1:

        Day = QObject::tr("Sunday");
        return Day.toStdString();

    case 2:

        Day = QObject::tr("Monday");
        return Day.toStdString();

    case 3:

        Day = QObject::tr("Tuesday");
        return Day.toStdString();

    case 4:

        Day = QObject::tr("Wednesday");
        return Day.toStdString();

    case 5:

        Day = QObject::tr("Thursday");
        return Day.toStdString();

    case 6:

        Day = QObject::tr("Fridy");
        return Day.toStdString();

    case 7:

        Day = QObject::tr("Saturday");
        return Day.toStdString();

    default:

        return string("");
    }
}

int DateUtil::literal2numeric_day_of_week(const string &d) {
    int i = 0;
    for (i = 0; i < 7; i++) {
        if (d == week_days[i])
            return i+1;
    }
    return -1;
}

bool DateUtil::is_leap(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int DateUtil::get_days_in_month(int month, int year) {
    int numberOfDays;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        numberOfDays = 30;
    else if (month == 2)
    {
        if (is_leap(year))
            numberOfDays = 29;
        else
            numberOfDays = 28;
    }
    else
      numberOfDays = 31;
    return numberOfDays;
}

/* Assume to receive a valid timestamp */
Date DateUtil::date_from_timestamp(time_t timestamp) {
    const time_t t = timestamp;
    struct tm *tm = localtime(&t);
#ifdef WEEK_START_MONDAY    
    Date date(tm->tm_mday, tm->tm_wday ?: 7, tm->tm_mon + 1, tm->tm_year + 1900);
#else
    Date date(tm->tm_mday, tm->tm_wday + 1 , tm->tm_mon + 1, tm->tm_year + 1900);
#endif
    //free(tm); segfault?
    return date;
}

Date DateUtil::get_first_day_of_month(Date &date) {
    int wday = date.getWeekDay() - (date.getMonthDay() % 7) + 1;
    //Normalization
    if (wday <= 0) wday += 7;
    else if (wday > 7) wday -= 7;
    return Date(1, wday, date.getMonth(), date.getYear());
}

Date DateUtil::get_last_day_of_month(Date &date) {
    Date first_day = get_first_day_of_month(date);
    int tot_days = get_days_in_month(date.getMonth(), date.getYear());
    int wday = ((tot_days % 7) ?: 7) + first_day.getWeekDay() - 1;
    return Date(tot_days, wday, date.getMonth(), date.getYear());
}

//Attention: each month has a different number of days (e.g. 31 doesn't exist in february)
//Assume to get a valid time (i.e. no negative numbers, etc.)
Date DateUtil::increase_month(Date date) {
    Date last_day_curr_month = get_last_day_of_month(date);
    Date first_day_next_month;
    if (date.getMonth() < 12) {
        first_day_next_month = Date(1, (last_day_curr_month.getWeekDay() % 7) + 1, date.getMonth() + 1, date.getYear());
    } else { //Go to next year
        first_day_next_month = Date(1, (last_day_curr_month.getWeekDay() % 7) + 1, 1, date.getYear() + 1);
    }
    return Date(date.getMonthDay(), ((date.getMonthDay() % 7) ?: 7) + first_day_next_month.getWeekDay() - 1, first_day_next_month.getMonth(), first_day_next_month.getYear());
}

//Assume to get a valid time (i.e. no negative numbers, etc.)
Date DateUtil::decrease_month(Date date) {
    Date first_day_curr_month = get_first_day_of_month(date);
    Date last_day_prev_month;
    if (date.getMonth() > 1) {
        last_day_prev_month = Date(get_days_in_month(date.getMonth() - 1, date.getYear()), (first_day_curr_month.getWeekDay() - 1) ?: 7, date.getMonth() - 1, date.getYear());
    } else { //Go to previous year
        last_day_prev_month = Date(get_days_in_month(12, date.getYear() - 1), (first_day_curr_month.getWeekDay() - 1) ?: 7, 12, date.getYear() - 1);
    }
    Date first_day_prev_month = get_first_day_of_month(last_day_prev_month);
    return Date(date.getMonthDay(), ((date.getMonthDay() % 7) ?: 7) + first_day_prev_month.getWeekDay() - 1, last_day_prev_month.getMonth(), last_day_prev_month.getYear());
}

Date DateUtil::increase_day(Date date) {
    Date last_day_curr_month = get_last_day_of_month(date);
    if (date.getMonthDay() < last_day_curr_month.getMonthDay())
        return Date(date.getMonthDay() + 1, ((date.getWeekDay() + 1) % 7) ?: 7, date.getMonth(), date.getYear());
    else { //This is the last day of the current month
        Date next_month = increase_month(date);
        return get_first_day_of_month(next_month);
    }
}

Date DateUtil::decrease_day(Date date) {
    if (date.getMonthDay() > 1)
        return Date(date.getMonthDay() - 1, (date.getWeekDay() - 1) ?: 7, date.getMonth(), date.getYear());
    else { //This is the first day of the current month
        Date previous_month = decrease_month(date);
        return get_last_day_of_month(previous_month);
    }
}

Date DateUtil::increase_year(Date date) {
    if ((date.getMonthDay() == 29) && (date.getMonth() == 2)) /* Leap year */
        return Date(1, ((date.getWeekDay() + 2) % 7) ?: 7, date.getMonth() + 1, date.getYear() + 1);
    else {
        int offset = 1;
        if (is_leap(date.getYear() + 1))
            offset = 2;
        return Date(date.getMonthDay(), ((date.getWeekDay() + offset) % 7) ?: 7, date.getMonth(), date.getYear() + 1);
    }
}

Date DateUtil::decrease_year(Date date) {
    int weekday = ((date.getWeekDay() - 1) % 7) ?: 7;
    if (is_leap(date.getYear()))
        weekday = ((weekday - 1) % 7) ?: 7;
    return Date(date.getMonthDay(), weekday, date.getMonth(), date.getYear() - 1);
}
