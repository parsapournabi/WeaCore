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
        class LoggerStreamParameters
        {
            public:
                const QObject* obj = nullptr;
                int level;
                int line;
                int logFilter;
                const char* function;
                DateTime datetime;
        };

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
        inline static DateTime globalDateTime; // Default datetime has All with Short Month value.

    private:
        LoggerStream* q_ptr = nullptr;
        LoggerStreamParameters params;
        QString m_buffer;
        QDebug m_debug;
};

WEACORE_NAMESPACE_END

#endif // LOGGERSTREAM_P_H
