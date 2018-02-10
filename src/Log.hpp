//
// Created by BraynStorm on 2/5/18.
//

#ifndef LOG_HPP
#define LOG_HPP

#include <cstdio>
#include <cstring>

#if defined(linux) || defined(__linux) || defined(__linux__)
#define PLATFORM_LINUX

#elif defined(__APPLE__) || defined (__MACH__)
#define PLATFORM_APPLE

#elif defined(_WIN32) ||defined(_WIN64)
#defined PLATFORM_WINDOWS

#elif defined(unix) || defined(__unix) || defined(__unix__)
#define PLATFORM_UNIX
#endif

namespace ThreeDee {
	enum class LogLevel {
		DEBUG = -2,
		TRACE,
		INFO,
		WARN,
		ERROR,
		FATAL
	};

	template<typename ... Tail>
	void log(
		LogLevel level,
		int line,
		const char* filename,
		const char* format,
		const Tail ... strings
	) {
		printf("[%2d][%25s:%-5d] => ", level, filename, line);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
		printf(format, strings...); // TODO figure out if this is really a security risk,
		// because we're logging god damn it! Logs should be "static" in terms of format
#pragma clang diagnostic pop
		printf("\n");
	};
};

#if defined(PLATFORM_LINUX)
#define LOG_SEPARATOR '/'
#elif defined(PLATFORM_WINDOWS)
#define LOG_SEPARATOR '\\'
#endif

#define __FILENAME__ (strrchr(__FILE__, LOG_SEPARATOR) ? strrchr(__FILE__, LOG_SEPARATOR) + 1 : __FILE__)
#define LOG(logLevel, ...) ::ThreeDee::log(::ThreeDee::LogLevel::logLevel, __LINE__, __FILENAME__, __VA_ARGS__)

#endif //LOG_HPP
