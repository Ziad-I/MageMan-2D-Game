#pragma once
#include <fstream>
#include <chrono>
#include <mutex>
#include <string>
#include <algorithm>
#include <thread>
#include <process.h>

#define PROFILING 1;
#ifdef PROFILING
	#define PROFILE_SCOPE(name) \
		ProfileTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() \
		PROFILE_SCOPE(__FUNCTION__)
#else
	#define PROFILE_SCOPE(name)
#endif // PROFILING


struct ProfileResult
{
	std::string name = "Default";
	long long start = 0;
	long long end = 0;
	size_t threadID = 0;
	size_t pID = 0;
};

class Profiler //! singleton class
{
private:

	std::string m_outputFile = "result.json";
	size_t m_profileCount = 0;
	std::ofstream m_outputStream;
	std::mutex m_lock;

	Profiler()
	{
		m_outputStream = std::ofstream(m_outputFile);
		writeHeader();
	}

	void writeHeader()
	{
		m_outputStream << "{\"otherData\": {}, \"traceEvents\":[";
	}

	void writeFooter()
	{
		m_outputStream << "]}";
	}

public:

	static Profiler& instance()
	{
		static Profiler instance;
		return instance;
	}

	void writeProfile(const ProfileResult& result)
	{
		// write results in json
		std::lock_guard<std::mutex> lock(m_lock);

		if (m_profileCount++ > 0)
			m_outputStream << ",";

		std::string name = result.name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_outputStream << "\n{";
		m_outputStream << "\"cat\":\"function\",";
		m_outputStream << "\"dur\":" << (result.end - result.start) << ",";
		m_outputStream << "\"name\":\"" << name << "\",";
		m_outputStream << "\"ph\":\"X\",";
		m_outputStream << "\"pid\":" << result.pID << ",";
		m_outputStream << "\"tid\":" << result.threadID << ",";
		m_outputStream << "\"ts\":" << result.start;
		m_outputStream << "}";
	}

	~Profiler()
	{
		writeFooter();
	}
};

class ProfileTimer
{
private:

	using ClockType = std::chrono::time_point<std::chrono::high_resolution_clock>;

	ClockType m_startTimepoint;
	ProfileResult m_result;
	bool m_stopped = false;

public:

	ProfileTimer(const std::string& name)
		: m_result({name, 0, 0, 0, 0})
	{
		start();
	}

	~ProfileTimer()
	{
		stop();
	}

	void start()
	{
		static long long lastStartTime = 0;

		m_startTimepoint = std::chrono::high_resolution_clock::now();
		m_result.start = std::chrono::time_point_cast<std::chrono::microseconds>
			(m_startTimepoint).time_since_epoch().count();

		//! add 1 microsecond to start time if its the same as previous start time
		//! to solve display issue for identical start times in chrome://tracing
		m_result.start += (m_result.start == lastStartTime ? 1 : 0);

		lastStartTime = m_result.start;

		m_stopped = false;
	}

	void stop()
	{
		if (m_stopped) //! to prevernt double logging
			return;

		auto endTimepoint = std::chrono::high_resolution_clock::now();
		m_result.end = std::chrono::time_point_cast<std::chrono::microseconds>
			(endTimepoint).time_since_epoch().count();
		m_result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		m_result.pID = _getpid();

		Profiler::instance().writeProfile(m_result);
		m_stopped = true;
	}
};