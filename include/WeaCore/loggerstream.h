#ifndef LOGGERSTREAM_H
#define LOGGERSTREAM_H

#include "utils.h"
#include "init.h"
#include <QDebug>

WEACORE_NAMESPACE_START

class DateTime;
class LoggerStreamPrivate;
class LoggerStream
{
        Q_DISABLE_COPY(LoggerStream)
        Q_DECLARE_PRIVATE(LoggerStream)

    public:
        enum LogLevel
        {
            Debug = 0,
            Info,
            Warning,
            Critical
        };
        enum LogFilter
        {
            NoneLog = 0,
            Level = 1 << 0, // LogLevel
            Timestamp = 1 << 1,
            ClassName = 1 << 2,
            FuncName = 1 << 3,
            LineNo = 1 << 4,
            All = Level | Timestamp | ClassName | FuncName | LineNo
        };

        LoggerStream(const QObject* obj, int level, const char* functionName, int line);
        ~LoggerStream();

        /** Logics **/
        typedef QDebug& (*QDebugManipulator)(QDebug&);
        LoggerStream& operator<<(QDebugManipulator manip);

        template<typename T>
        inline LoggerStream& operator<<(T value)
        {
            debug() << value << ' ';
            return *this;
        }

        LoggerStream& nologlevel();
        LoggerStream& notimestamp();
        LoggerStream& noclassname();
        LoggerStream& nofunctionname();
        LoggerStream& noline();
        LoggerStream& datetime(const DateTime& datetime);
        LoggerStream& datetimeFormat(int format);
        LoggerStream& datetimeMonthFormat(int format);

        /** Statics **/
        static bool isLogLevelShort();
        static bool logLevelShort();
        static void setLogLevelShort(bool value);

        static LogFilter defaultLogFilter();
        static void setDefaultLogFilter(LogFilter filter);

        static DateTime defaultDateTime();
        static void setDefaultDateTime(const DateTime& datetime);

    protected:
        QDebug& debug();

    private:
        QScopedPointer<LoggerStreamPrivate> d_ptr;
};

/** Macros **/
#if (!defined(WEA_DEBUG)) && defined(QT_DEBUG)
#define WEA_DEBUG true
#elif (!defined(WEA_DEBUG))
#define WEA_DEBUG false
#endif

#define WEA_LOGGER(defaultValue) \
    W_PROP_HDEF(bool, debug, Debug, defaultValue)

#define wDebug() WeaCore::LoggerStream(this, WeaCore::LoggerStream::LogLevel::Debug, __FUNCTION__, __LINE__)
#define wInfo() WeaCore::LoggerStream(this, WeaCore::LoggerStream::LogLevel::Info, __FUNCTION__, __LINE__)
#define wWarning() WeaCore::LoggerStream(this, WeaCore::LoggerStream::LogLevel::Warning, __FUNCTION__, __LINE__)
#define wCritical() WeaCore::LoggerStream(this, WeaCore::LoggerStream::LogLevel::Critical, __FUNCTION__, __LINE__)
#define wError() WeaCore::LoggerStream(this, WeaCore::LoggerStream::LogLevel::Critical, __FUNCTION__, __LINE__)

WEACORE_NAMESPACE_END


#endif // LOGGERSTREAM_H
