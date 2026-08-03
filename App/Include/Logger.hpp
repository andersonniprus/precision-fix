#pragma once

namespace App
{
	enum class LogLevel : std::uint8_t
	{
		Info,
		Warning,
		Error,
	};

	struct LogEntry
	{
		std::chrono::system_clock::time_point time {};
		LogLevel level {};
		std::string category;
		std::string message;
	};

	class Logger
	{
	public:
		Logger( ) = default;

		Logger( const Logger& )            = delete;
		Logger& operator=( const Logger& ) = delete;

		void add( LogLevel, std::string_view, std::string_view );
		void info( std::string_view, std::string_view );
		void warn( std::string_view, std::string_view );
		void error( std::string_view, std::string_view );

		[[nodiscard]] std::vector<LogEntry> entries( ) const;

	private:
		static constexpr std::size_t kMaxEntries = 2000;

		mutable std::mutex mutex_;
		std::deque<LogEntry> entries_;
	};
}
