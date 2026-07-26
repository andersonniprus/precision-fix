#pragma once

namespace Core
{
	enum class Error : std::uint8_t
	{
		AccessDenied,
		NotFound,
		Unsupported,
		InvalidValue,
		NotImplemented,
		Unknown,
	};

	template<typename T>
	using Result = std::expected<T, Error>;
	using Status = std::expected<void, Error>;

	[[nodiscard]] constexpr std::string_view to_string( const Error error ) noexcept
	{
		switch ( error )
		{
			case Error::AccessDenied:
				return "Access denied (requires administrator)";

			case Error::NotFound:
				return "Value not present";

			case Error::Unsupported:
				return "Not supported on this system";

			case Error::InvalidValue:
				return "Invalid value";

			case Error::NotImplemented:
				return "Not implemented";

			case Error::Unknown:
				return "Unknown error";
		}

		return "Unknown error";
	}
}
