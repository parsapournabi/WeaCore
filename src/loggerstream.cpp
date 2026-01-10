#include "WeaCore/loggerstream.h"
#include "loggerstream_p.h"

#include "WeaCore/datetime.h"

#include <QDateTime>

WEACORE_NAMESPACE_START

/** Template Shortcuts **/
using TLogFilter = LoggerStream::LogFilter;

/** Public Class **/

LoggerStream::LoggerStream(const QObject* obj, int level, const char* functionName, int line)
    :
    d_ptr(new LoggerStreamPrivate(this,
                                  level,
                                  obj,
                                  functionName,
                                  line))
{

}

LoggerStream::~LoggerStream()
{
    Q_D(LoggerStream);
    if (d->debugIsEnabled())
    {
        d->puzzleLogFilters();
        d->printLoggedMessage();
    }
}

LoggerStream& LoggerStream::nologlevel()
{
    Q_D(LoggerStream);
    d->m_logFilter &= ~TLogFilter::Level;
    return *this;
}

LoggerStream& LoggerStream::notimestamp()
{
    Q_D(LoggerStream);
    d->m_logFilter &= ~TLogFilter::Timestamp;
    return *this;

}

LoggerStream& LoggerStream::noclassname()
{

    Q_D(LoggerStream);
    d->m_logFilter &= ~TLogFilter::ClassName;
    return *this;
}

LoggerStream& LoggerStream::nofunctionname()
{
    Q_D(LoggerStream);
    d->m_logFilter &= ~TLogFilter::FuncName;
    return *this;
}

LoggerStream& LoggerStream::noline()
{
    Q_D(LoggerStream);
    d->m_logFilter &= ~TLogFilter::LineNo;
    return *this;
}

LoggerStream& LoggerStream::datetime(const DateTime& datetime)
{
    Q_D(LoggerStream);
    d->m_datetime = datetime;
    return *this;
}

LoggerStream& LoggerStream::datetimeFormat(int format)
{
    Q_D(LoggerStream);
    d->m_datetime.setFormat(format);
    return *this;
}

LoggerStream& LoggerStream::datetimeMonthFormat(int format)
{
    Q_D(LoggerStream);
    d->m_datetime.setMonthFormat(format);
    return *this;
}

bool LoggerStream::isLogLevelShort()
{
    return logLevelShort();
}

bool LoggerStream::logLevelShort()
{
    return LoggerStreamPrivate::logLevelShort;
}

void LoggerStream::setLogLevelShort(bool value)
{
    LoggerStreamPrivate::logLevelShort = value;
}

LoggerStream::LogFilter LoggerStream::defaultLogFilter()
{
    return (LoggerStream::LogFilter) LoggerStreamPrivate::globalLogFilter;
}

void LoggerStream::setDefaultLogFilter(LogFilter filter)
{
    LoggerStreamPrivate::globalLogFilter = filter;
}

DateTime LoggerStream::defaultDateTime()
{
    return LoggerStreamPrivate::datetime;
}

void LoggerStream::setDefaultDateTime(const DateTime& datetime)
{
    LoggerStreamPrivate::datetime = datetime;
}

QDebug& LoggerStream::debug()
{
    Q_D(LoggerStream);
    return d->m_debug;
}

typedef QDebug& (*QDebugManipulator)(QDebug&);
LoggerStream& LoggerStream::operator<<(QDebugManipulator manip)
{
    Q_D(LoggerStream);
    d->m_debug << manip;
    return *this;
}

/** Private Impelmentation Class **/

LoggerStreamPrivate::LoggerStreamPrivate(
    LoggerStream* q,
    int level,
    const QObject* obj,
    const char* functionName,
    int line)

    :
    q_ptr(q),
    m_level(level),
    m_obj(obj),
    m_function(functionName),
    m_line(line),
    m_logFilter(globalLogFilter),
    m_buffer(),
    m_datetime(datetime),
    m_debug(&m_buffer)
{
    m_debug.noquote().noquote();
}

const QString LoggerStreamPrivate::logLevelToString(int level) const
{
    switch (level)
    {
        case LoggerStream::LogLevel::Debug:
            return logLevelShort ? "D" : "DEBUG";
        case LoggerStream::LogLevel::Info:
            return logLevelShort ? "I" : "INFO";
        case LoggerStream::LogLevel::Warning:
            return logLevelShort ? "W" : "WARNING";
        case LoggerStream::LogLevel::Critical:
            return logLevelShort ? "E" : "ERROR";
        default:
            return logLevelShort ? "U" : "UNKNOWN";
    }
}

bool LoggerStreamPrivate::debugIsEnabled()
{
    return debugIsEnabled(m_obj);
}

bool LoggerStreamPrivate::debugIsEnabled(const QObject* obj)
{
    if (!obj  || obj == nullptr)
    {
        return false;
    }
    const QMetaObject* meta = obj->metaObject();
    int propIndex = -1;
    const char* propName = "debug";
    propIndex = meta->indexOfProperty(propName);

    if (propIndex < 0)
    {
        propName = "debugMode";
        propIndex = meta->indexOfProperty(propName);
    }
    if (propIndex >= 0)
    {
        QVariant value = obj->property(propName);
        if (value.isValid() && value.canConvert<bool>())
        {
            return value.toBool();
        }
    }
    return WEA_DEBUG;
}

void LoggerStreamPrivate::puzzleLogFilters()
{
    QString prefix = "";
    prefix.reserve(256);
    if (hasFlag(TLogFilter::Level, m_logFilter))
    {
        prefix += QString("[%1]").arg(logLevelToString(m_level));
    }
    if (hasFlag(TLogFilter::Timestamp, m_logFilter))
    {
        prefix += QString("[%1]").arg(m_datetime.currentDateTimeStr());
    }
    if (hasFlag(TLogFilter::ClassName, m_logFilter) || hasFlag(TLogFilter::FuncName, m_logFilter))
    {
        prefix += "[";
        if (hasFlag(TLogFilter::ClassName, m_logFilter) && m_obj)
        {
            prefix += QString("%1::").arg(m_obj->metaObject()->className());
        }
        if (hasFlag(TLogFilter::FuncName, m_logFilter))
        {
            prefix += QString("%1").arg(m_function);
        }
        prefix += "]";
    }
    if (hasFlag(TLogFilter::LineNo, m_logFilter))
    {
        prefix += QString("[Line:%1]").arg(m_line);
    }
    if (!isSameFlag(TLogFilter::NoneLog, m_logFilter))
    {
        prefix += ": ";
    }

    // QString prefix = QString("[%1][%2][%3::%4][Line:%5]: ")
    //                  .arg(logLevelToString(m_level),
    //                       QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"),
    //                       m_obj ? m_obj->metaObject()->className() : "UnknownClass", m_function)
    //                  .arg(m_line);
    if (!m_buffer.isEmpty())
    {
        m_buffer.prepend(prefix);
    }
}

void LoggerStreamPrivate::printLoggedMessage() const
{
    qDebug().noquote().nospace() << m_buffer;
}

WEACORE_NAMESPACE_END
