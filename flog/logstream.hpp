#pragma once

#include <mutex>
#include <string>
#include <locale>
#include <vector>
#include <fstream>
#include <type_traits>

namespace flog
{
    using ios_type = std::basic_ios<std::ostream::char_type, std::ostream::traits_type>;
    using manip1 = std::add_pointer<std::ostream&(std::ostream&)>::type;
    using manip2 = std::add_pointer<ios_type&(ios_type&)>::type;
    using manip3 = std::add_pointer<std::ios_base&(std::ios_base&)>::type;

    enum filter_level
    {
        trace = 0,
        warning,
        fatal,
        hide_all
    };

    class logstream
    {
    public:
        //! universal constructor
        logstream(const bool &condition, const filter_level &filter);
        //! filtered file logging
        logstream(const filter_level &level = trace);
        //! conditional file logging
        logstream(const bool &condition);
        virtual ~logstream();
    public:
        template <class T>
        logstream& operator<<(const T &entry);
        logstream& operator<<(manip1 mp);
        logstream& operator<<(manip2 mp);
        logstream& operator<<(manip3 mp);
    public:
        static void SetLoggerPath(const std::string &Path);
        static const std::string TimeStamp();
        static bool SetLocale(const char *locale);
        static void SetTimeStampEnable(const bool &enable);
        static void SetForceFlush(const bool &enable);
        static void SetFilterLevel(const filter_level &level);
    private:
        void Init();
    private:
        static std::mutex m_Mutex;
        static std::locale m_Locale;
        static bool m_bEnableTimeStamp;
        static bool m_bForceFlush;
        static filter_level m_ActiveLevel;
        static std::ofstream m_ofs;
    private:
        filter_level m_Level;
    };
}

#include "logstream_impl.hpp"