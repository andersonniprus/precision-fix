#pragma once

namespace Utils::Process
{
	[[nodiscard]] inline Core::Result<std::string> run( const wchar_t* command_line )
	{
		SECURITY_ATTRIBUTES security {
			.nLength = sizeof( security ),
			.lpSecurityDescriptor = nullptr,
			.bInheritHandle = TRUE,
		};

		HANDLE read_pipe {};
		HANDLE write_pipe {};

		if ( !CreatePipe( &read_pipe, &write_pipe, &security, 0 ) )
			return std::unexpected( Core::Error::Unknown );

		if ( !SetHandleInformation( read_pipe, HANDLE_FLAG_INHERIT, 0 ) )
		{
			CloseHandle( read_pipe );
			CloseHandle( write_pipe );
			return std::unexpected( Core::Error::Unknown );
		}

		STARTUPINFOW startup {
			.cb = sizeof( startup ),
			.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW,
			.wShowWindow = SW_HIDE,
			.hStdInput = nullptr,
			.hStdOutput = write_pipe,
			.hStdError = write_pipe,
		};

		PROCESS_INFORMATION process {};

		std::wstring mutable_command { command_line };

		const BOOL created = CreateProcessW(
			nullptr, mutable_command.data( ), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &startup, &process
		);

		CloseHandle( write_pipe );

		if ( !created )
		{
			CloseHandle( read_pipe );
			return std::unexpected( Core::Error::Unknown );
		}

		std::string output;
		char buffer[ 4096 ];
		DWORD bytes_read {};

		while ( ReadFile( read_pipe, buffer, sizeof( buffer ), &bytes_read, nullptr ) && bytes_read > 0 )
		{
			output.append( buffer, bytes_read );
		}

		CloseHandle( read_pipe );

		WaitForSingleObject( process.hProcess, 15000 );

		DWORD exit_code = 1;
		GetExitCodeProcess( process.hProcess, &exit_code );

		CloseHandle( process.hThread );
		CloseHandle( process.hProcess );

		if ( exit_code != 0 )
			return std::unexpected( Core::Error::Unknown );

		return output;
	}

	[[nodiscard]] inline Core::Status run_silent( const wchar_t* command_line )
	{
		STARTUPINFOW startup {
			.cb = sizeof( startup ),
			.dwFlags = STARTF_USESHOWWINDOW,
			.wShowWindow = SW_HIDE,
		};

		PROCESS_INFORMATION process {};

		std::wstring mutable_command { command_line };

		if ( !CreateProcessW(
			nullptr, mutable_command.data( ), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &startup, &process
		) )
			return std::unexpected( Core::Error::Unknown );

		WaitForSingleObject( process.hProcess, 15000 );

		DWORD exit_code = 1;
		GetExitCodeProcess( process.hProcess, &exit_code );

		CloseHandle( process.hThread );
		CloseHandle( process.hProcess );

		if ( exit_code != 0 )
			return std::unexpected( Core::Error::Unknown );

		return {};
	}
}
