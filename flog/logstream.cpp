#include "logstream.hpp"

#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace flog
{
    std::mutex logstream::m_Mutex = {};
    std::locale logstream::m_Locale = {};
    bool logstream::m_bEnableTimeStamp = true;
    bool logstream::m_bForceFlush = true;
    filter_level logstream::m_ActiveLevel = trace;
    std::ofstream logstream::m_ofs{ std::ofstream("log", std::ios::out | std::ios::app) };

    logstream::logstream(const bool &condition, const filter_level &filter)
    : m_Level(filter)
    {
        m_Mutex.lock();
        if (!condition || m_Level < m_ActiveLevel)
            m_ofs.setstate(std::ios::failbit);
        else
            Init();
    }
    
    logstream::logstream(const filter_level &level)
    : logstream(true, level) {}

    logstream::logstream(const bool &condition)
    : logstream(condition, trace) {}
    
    logstream::~logstream()
    {
        if (m_ofs.good() && m_bForceFlush)
            m_ofs << std::flush;
        else
            m_ofs.clear();
        m_Mutex.unlock();
    }
    
    void logstream::SetLoggerPath(const std::string &Path)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (m_ofs.is_open())
            m_ofs.close();
        m_ofs.open(Path, std::ios::out | std::ios::app);
    }
    
    const std::string logstream::TimeStamp()
    {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss.imbue(std::locale(m_Locale));
        ss << std::put_time(std::localtime(&now), "[%c %Z]");
        return ss.str();
    }
    
    bool logstream::SetLocale(const char *locale)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        try
        {
            m_Locale = std::locale(locale);
            return true;
        }
        catch (std::runtime_error &)
        {
            return false;
        }
    }
    
    void logstream::SetTimeStampEnable(const bool &enable)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_bEnableTimeStamp = enable;
    }

    void logstream::SetForceFlush(const bool &enable)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_bForceFlush = enable;
    }

    void logstream::SetFilterLevel(const filter_level &level)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_ActiveLevel = level;
    }
    
    void logstream::Init()
    {
        m_ofs.imbue(m_Locale);
        if (m_bEnableTimeStamp)
        {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            m_ofs << std::put_time(std::localtime(&now), "[%c %Z]");
        }
    }

    logstream& logstream::operator<<(manip1 mp)
    {
        m_ofs << mp;
        return *this;
    }

    logstream& logstream::operator<<(manip2 mp)
    {
        m_ofs << mp;
        return *this;
    }

    logstream& logstream::operator<<(manip3 mp)
    {
        m_ofs << mp;
        return *this;
    }
}
