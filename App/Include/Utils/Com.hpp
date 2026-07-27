#pragma once

namespace Utils::Com
{
	class Guard
	{
	public:
		Guard( ) noexcept
			: result_( CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED ) )
		{
		}

		~Guard( )
		{
			if ( SUCCEEDED( result_ ) )
				CoUninitialize( );
		}

		Guard( const Guard& )            = delete;
		Guard& operator=( const Guard& ) = delete;

		[[nodiscard]] bool ok( ) const noexcept
		{
			return SUCCEEDED( result_ ) || result_ == RPC_E_CHANGED_MODE;
		}

	private:
		HRESULT result_;
	};
}
