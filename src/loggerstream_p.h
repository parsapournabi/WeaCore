#ifndef LOGGERSTREAM_P_H
#define LOGGERSTREAM_P_H

#include "WeaCore/datetime.h"
#include <QDebug>

WEACORE_NAMESPACE_START

class LoggerStream;
class LoggerStreamPrivate
{
        Q_DISABLE_COPY(LoggerStreamPrivate)
        Q_DECLARE_PUBLIC(LoggerStream)

    public:
        LoggerStreamPrivate(LoggerStream* q,
                            int level,
                            const QObject* obj,
                            const char* functionName,
                            int line);

        /** Configurations **/
        const QString logLevelToString(int level) const;
        bool debugIsEnabled();
        bool debugIsEnabled(const QObject* obj);

        /** Logics **/
        void puzzleLogFilters();
        void printLoggedMessage() const;

        /** Statics **/
        inline static bool logLevelShort = false; // Make logLevelToString result with First 'Char'.
        inline static int globalLogFilter = 31; // Default Value is LogFilter::All.
        inline static DateTime datetime; // Default datetime has All with Short Month value.

    private:
        LoggerStream* q_ptr = nullptr;
        const QObject* m_obj = nullptr;
        const char* m_function;
        int m_line;
        QString m_buffer;
        QDebug m_debug;
        int m_level;
        int m_logFilter;
        DateTime m_datetime;
};

WEACORE_NAMESPACE_END

#endif // LOGGERSTREAM_P_H
