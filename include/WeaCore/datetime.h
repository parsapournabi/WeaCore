#ifndef DATETIME_H
#define DATETIME_H

#include "init.h"

WEACORE_NAMESPACE_START

class DateTimePrivate;
class DateTime
{
        Q_DECLARE_PRIVATE(DateTime)
    public:
        enum DateTimeFormat
        {
            NoFormat = 0,
            Milli = 1 << 0,
            Second = 1 << 1,
            Minute = 1 << 2,
            Hour = 1 << 3,
            Day = 1 << 4,
            Month = 1 << 5,
            Year = 1 << 6,

            YMD = Year | Month | Day,
            HMS = Hour | Minute | Second,
            HMSZ = HMS | Milli,

            All = YMD | HMSZ,
        };
        enum MonthFormat
        {
            Number = 0,
            ShortName,
            CompleteName
        };

        DateTime();
        DateTime(int datetimeFormat, int monthFormat = 1);
        DateTime(DateTimePrivate* d);
        DateTime(const DateTime& other);

        ~DateTime();

        /** Operators **/
        DateTime& operator=(const DateTime& other);

        /** Getters & Setters **/
        QString currentDateTimeStr() const;

        QString formatStr() const;
        int format() const;
        void setFormat(int fmt);

        QString monthFormatStr() const;
        int monthFormat() const;
        void setMonthFormat(int fmt);

    private:
        QScopedPointer<DateTimePrivate> d_ptr;
};

WEACORE_NAMESPACE_END

#endif // DATETIME_H
