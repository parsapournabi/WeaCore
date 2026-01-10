#ifndef DATETIME_P_H
#define DATETIME_P_H

#include "WeaCore/init.h"

WEACORE_NAMESPACE_START

class DateTime;
class DateTimePrivate
{
        Q_DECLARE_PUBLIC(DateTime)
    public:
        class DateTimeParameters
        {
            public:
                int format = 127; // Equal with DateTimeFormat::All enum.
                int monthFormat = 1; // Equal with MonthFormat::ShortName.

        };

        DateTimePrivate(DateTime* q);
        DateTimePrivate(DateTime* q, int datetimeFormat, int monthFormat);

        /** Logics **/
        const QString monthFormatToString() const;
        const QString datetimeFormatToString() const;

    private:
        DateTime* q_ptr = nullptr;
        DateTimeParameters params;
};

WEACORE_NAMESPACE_END

#endif // DATETIME_P_H
