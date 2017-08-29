#pragma once

#include "logstream.hpp"

/* log stream api */
/* syntax from https://stackoverflow.com/questions/16683146/can-macros-be-overloaded-by-number-of-arguments */
#define NUM_ARGS_(_1, _2, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 2, 1)

#define CONCATE_(X, Y) X##Y  // Fixed the double '_' from previous code
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define LOG(...) VA_MACRO(LOG, __VA_ARGS__)
#define LOG1(_1) flog::logstream(_1)
#define LOG2(_1, _2) flog::logstream(_1, _2)

/* global config api */
#define LOG_CONFIG_PATH(path) flog::logstream::SetLoggerPath(path)
#define LOG_CONFIG_LOCALE(locale) flog::logstream::SetLocale(locale)
#define LOG_CONFIG_TIMESTAMP(enable) flog::logstream::SetTimeStampEnable(enable)
#define LOG_CONFIG_FORCE_FLUSH(enable) flog::logstream::SetForceFlush(enable)
#define LOG_CONFIG_FILTER(filter) flog::logstream::SetFilterLevel(filter)
