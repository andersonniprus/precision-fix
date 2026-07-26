#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class AudioFeature : std::uint8_t
	{
		Enhancements,
		Count,
	};

	class AudioModule final : public ModuleBase<AudioModule, AudioFeature>
	{
	public:
		AudioModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<bool> load_enhancements( );
		static Core::Status apply_enhancements( const bool& );
	};

	template<>
	struct FeatureTraits<AudioFeature::Enhancements>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &AudioModule::load_enhancements;
		static constexpr auto apply = &AudioModule::apply_enhancements;
	};
}
