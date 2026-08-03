#include "Stdafx.hpp"
#include "Logger.hpp"

namespace App
{
	void Logger::add( const LogLevel level, const std::string_view category, const std::string_view message )
	{
		LogEntry entry {
			.time = std::chrono::system_clock::now( ),
			.level = level,
			.category = std::string { category },
			.message = std::string { message },
		};

		std::scoped_lock lock( mutex_ );

		entries_.push_back( std::move( entry ) );

		while ( entries_.size( ) > kMaxEntries )
		{
			entries_.pop_front( );
		}
	}

	void Logger::info( const std::string_view category, const std::string_view message )
	{
		add( LogLevel::Info, category, message );
	}

	void Logger::warn( const std::string_view category, const std::string_view message )
	{
		add( LogLevel::Warning, category, message );
	}

	void Logger::error( const std::string_view category, const std::string_view message )
	{
		add( LogLevel::Error, category, message );
	}

	std::vector<LogEntry> Logger::entries( ) const
	{
		std::scoped_lock lock( mutex_ );
		return { entries_.begin( ), entries_.end( ) };
	}
}
