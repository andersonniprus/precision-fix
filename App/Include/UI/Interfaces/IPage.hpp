#pragma once

namespace UI
{
	class IPage
	{
	public:
		virtual ~IPage( ) = default;

		virtual void render( ) = 0;

		virtual void on_activate( )
		{
		}
	};
}
