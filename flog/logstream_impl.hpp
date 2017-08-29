#pragma once

#include <map>
#include <set>
#include <list>
#include <array>
#include <deque>
#include <string>
#include <vector>
#include <ostream>
#include <type_traits>
#include <unordered_map>

namespace helper
{
    template <class T>
    struct is_iterable
    {
        using false_type = long;
        using true_type = char;
        template <class _T> static false_type check(...);
        template <class _T> static true_type check(typename _T::const_iterator*);

        static const bool value = sizeof(check<T>(0)) == sizeof(true_type);
    };

    static int s_depth = 0;

    template <class T>
    flog::logstream& ostream_impl(flog::logstream &os, const T &container)
    {
        os << std::string(4 * s_depth++, ' ') << '{' << "\n";
        int a = 0;
        if (is_iterable<typename T::value_type>::value)
            a = 1;
        else
            a = 2;
        for (auto it = container.begin(); it != container.end(); it++)
        {
#ifdef __GNUG__
            os << (it == container.begin() && !is_iterable<typename T::value_type>::value ? std::string(4 * s_depth, ' ') : "")
               << *it
               << (std::next(it) == container.end() || is_iterable<typename T::value_type>::value ? "" : ", ");
            if (std::next(it) == container.end() && !is_iterable<typename T::value_type>::value)
                os << "\n";
#else
            os << (it == container.begin() && !is_iterable<T::value_type>::value ? std::string(4 * s_depth, ' ') : "")
               << *it
               << (std::next(it) == container.end() || is_iterable<T::value_type>::value ? "" : ", ");
            if (std::next(it) == container.end() && !is_iterable<T::value_type>::value)
                os << "\n";
#endif
        }
        os << std::string(4 * --s_depth, ' ') << '}' << "\n";
        return os;
    }

    template <class T>
    flog::logstream& ostream_map_impl(flog::logstream &os, const T &map)
    {
        os << std::string(4 * s_depth++, ' ') << '{' << "\n";
        for (auto it = map.begin(); it != map.end(); it++)
        {
            os << std::string(4 * s_depth, ' ')
               << (*it).first << ": ";
            if (is_iterable<decltype((*it).second)>::value)
                os << "\n";
            os << (*it).second
               << (std::next(it) == map.end() || is_iterable<decltype((*it).second)>::value ? "" : ", ");
            if (!is_iterable<decltype((*it).second)>::value)
                os << "\n";
        }
        os << std::string(4 * --s_depth, ' ') << '}' << "\n";
        return os;
    }
}

namespace flog
{
    template <class T>
    logstream& logstream::operator<<(const T &entry)
    {
        m_ofs << entry;
        return *this;
    }

    template <class T, template <class> class A>
    logstream& operator<<(logstream &os, const std::vector<T, A<T>> &container)
    {
        return helper::ostream_impl(os, container);
    }

    template <class T, template <class> class A>
    logstream& operator<<(logstream &os, const std::deque<T, A<T>> &container)
    {
        return helper::ostream_impl(os, container);
    }

    template <class T, template <class> class A>
    logstream& operator<<(logstream &os, const std::list<T, A<T>> &container)
    {
        return helper::ostream_impl(os, container);
    }

    template <class T, template <class> class C, template <class> class A>
    logstream& operator<<(logstream &os, const std::set<T, C<T>, A<T>> &container)
    {
        return helper::ostream_impl(os, container);
    }

    template <class T, template <class> class C, template <class> class A>
    logstream& operator<<(logstream &os, const std::multiset<T, C<T>, A<T>> &container)
    {
        return helper::ostream_impl(os, container);
    }

    template <class T, std::size_t N>
    logstream& operator<<(logstream &os, const std::array<T, N> &container)
    {
        return helper::ostream_impl(os, container);
    }

    template <class TKey, class TVal, class C, class A>
    logstream& operator<<(logstream &os, const std::map<TKey, TVal, C, A> &map)
    {
        return helper::ostream_map_impl(os, map);
    }

    template <class TKey, class TVal, class C, class A>
    logstream& operator<<(logstream &os, const std::multimap<TKey, TVal, C, A> &map)
    {
        return helper::ostream_map_impl(os, map);
    }

    template <class TKey, class TVal, class C, class A>
    logstream& operator<<(logstream &os, const std::unordered_map<TKey, TVal, C, A> &map)
    {
        return helper::ostream_map_impl(os, map);
    }
}
