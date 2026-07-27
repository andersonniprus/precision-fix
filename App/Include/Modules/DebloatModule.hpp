#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class DebloatFeature : std::uint8_t
	{
		BingWeather,
		GetHelp,
		GetStarted,
		HeifImageExtension,
		Messaging,
		Microsoft3DViewer,
		SolitaireCollection,
		StickyNotes,
		MixedRealityPortal,
		OneConnect,
		People,
		Print3D,
		SkypeApp,
		WebMediaExtensions,
		WebpImageExtension,
		WindowsAlarms,
		WindowsCamera,
		FeedbackHub,
		WindowsMaps,
		SoundRecorder,
		YourPhone,
		ZuneMusic,
		MailAndCalendar,
		BingApps,
		DrawboardPdf,
		Sway,
		Cortana,
		Copilot,
		Count,
	};

	class DebloatModule final : public ModuleBase<DebloatModule, DebloatFeature>
	{
	public:
		DebloatModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		template<DebloatFeature F>
		[[nodiscard]] static Core::Result<bool> load( );

		template<DebloatFeature F>
		static Core::Status apply( const bool& );
	};

	template<>
	struct FeatureTraits<DebloatFeature::BingWeather>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::BingWeather>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::BingWeather>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::GetHelp>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::GetHelp>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::GetHelp>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::GetStarted>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::GetStarted>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::GetStarted>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::HeifImageExtension>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::HeifImageExtension>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::HeifImageExtension>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::Messaging>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::Messaging>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::Messaging>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::Microsoft3DViewer>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::Microsoft3DViewer>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::Microsoft3DViewer>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::SolitaireCollection>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::SolitaireCollection>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::SolitaireCollection>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::StickyNotes>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::StickyNotes>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::StickyNotes>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::MixedRealityPortal>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::MixedRealityPortal>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::MixedRealityPortal>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::OneConnect>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::OneConnect>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::OneConnect>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::People>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::People>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::People>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::Print3D>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::Print3D>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::Print3D>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::SkypeApp>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::SkypeApp>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::SkypeApp>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::WebMediaExtensions>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::WebMediaExtensions>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::WebMediaExtensions>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::WebpImageExtension>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::WebpImageExtension>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::WebpImageExtension>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::WindowsAlarms>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::WindowsAlarms>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::WindowsAlarms>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::WindowsCamera>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::WindowsCamera>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::WindowsCamera>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::FeedbackHub>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::FeedbackHub>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::FeedbackHub>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::WindowsMaps>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::WindowsMaps>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::WindowsMaps>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::SoundRecorder>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::SoundRecorder>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::SoundRecorder>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::YourPhone>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::YourPhone>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::YourPhone>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::ZuneMusic>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::ZuneMusic>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::ZuneMusic>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::MailAndCalendar>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::MailAndCalendar>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::MailAndCalendar>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::BingApps>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::BingApps>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::BingApps>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::DrawboardPdf>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::DrawboardPdf>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::DrawboardPdf>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::Sway>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::Sway>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::Sway>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::Cortana>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::Cortana>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::Cortana>;
	};

	template<>
	struct FeatureTraits<DebloatFeature::Copilot>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &DebloatModule::load<DebloatFeature::Copilot>;
		static constexpr auto apply = &DebloatModule::apply<DebloatFeature::Copilot>;
	};
}
