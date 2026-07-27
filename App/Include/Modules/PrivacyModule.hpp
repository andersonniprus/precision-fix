#pragma once

#include "Modules/ModuleBase.hpp"

namespace Modules
{
	enum class PrivacyFeature : std::uint8_t
	{
		Telemetry,
		AdvertisingId,
		ActivityFeed,
		TailoredExperiences,
		GameDvr,
		ContentSuggestions,
		ErrorReporting,
		DiagnosticExecution,
		LocationServices,
		FeedbackPrompts,
		NewsAndInterests,
		WindowsFeeds,
		SettingSync,
		DiagnosticTasks,
		Count,
	};

	class PrivacyModule final : public ModuleBase<PrivacyModule, PrivacyFeature>
	{
	public:
		PrivacyModule( );

	private:
		template<auto>
		friend struct FeatureTraits;

		[[nodiscard]] static Core::Result<bool> load_telemetry( );
		static Core::Status apply_telemetry( const bool& );

		[[nodiscard]] static Core::Result<bool> load_advertising_id( );
		static Core::Status apply_advertising_id( const bool& );

		[[nodiscard]] static Core::Result<bool> load_activity_feed( );
		static Core::Status apply_activity_feed( const bool& );

		[[nodiscard]] static Core::Result<bool> load_tailored_experiences( );
		static Core::Status apply_tailored_experiences( const bool& );

		[[nodiscard]] static Core::Result<bool> load_game_dvr( );
		static Core::Status apply_game_dvr( const bool& );

		[[nodiscard]] static Core::Result<bool> load_content_suggestions( );
		static Core::Status apply_content_suggestions( const bool& );

		[[nodiscard]] static Core::Result<bool> load_error_reporting( );
		static Core::Status apply_error_reporting( const bool& );

		[[nodiscard]] static Core::Result<bool> load_diagnostic_execution( );
		static Core::Status apply_diagnostic_execution( const bool& );

		[[nodiscard]] static Core::Result<bool> load_location_services( );
		static Core::Status apply_location_services( const bool& );

		[[nodiscard]] static Core::Result<bool> load_feedback_prompts( );
		static Core::Status apply_feedback_prompts( const bool& );

		[[nodiscard]] static Core::Result<bool> load_news_and_interests( );
		static Core::Status apply_news_and_interests( const bool& );

		[[nodiscard]] static Core::Result<bool> load_windows_feeds( );
		static Core::Status apply_windows_feeds( const bool& );

		[[nodiscard]] static Core::Result<bool> load_setting_sync( );
		static Core::Status apply_setting_sync( const bool& );

		[[nodiscard]] static Core::Result<bool> load_diagnostic_tasks( );
		static Core::Status apply_diagnostic_tasks( const bool& );
	};

	template<>
	struct FeatureTraits<PrivacyFeature::Telemetry>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_telemetry;
		static constexpr auto apply = &PrivacyModule::apply_telemetry;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::AdvertisingId>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_advertising_id;
		static constexpr auto apply = &PrivacyModule::apply_advertising_id;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::ActivityFeed>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_activity_feed;
		static constexpr auto apply = &PrivacyModule::apply_activity_feed;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::TailoredExperiences>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_tailored_experiences;
		static constexpr auto apply = &PrivacyModule::apply_tailored_experiences;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::GameDvr>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_game_dvr;
		static constexpr auto apply = &PrivacyModule::apply_game_dvr;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::ContentSuggestions>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_content_suggestions;
		static constexpr auto apply = &PrivacyModule::apply_content_suggestions;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::ErrorReporting>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_error_reporting;
		static constexpr auto apply = &PrivacyModule::apply_error_reporting;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::DiagnosticExecution>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_diagnostic_execution;
		static constexpr auto apply = &PrivacyModule::apply_diagnostic_execution;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::LocationServices>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_location_services;
		static constexpr auto apply = &PrivacyModule::apply_location_services;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::FeedbackPrompts>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_feedback_prompts;
		static constexpr auto apply = &PrivacyModule::apply_feedback_prompts;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::NewsAndInterests>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_news_and_interests;
		static constexpr auto apply = &PrivacyModule::apply_news_and_interests;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::WindowsFeeds>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_windows_feeds;
		static constexpr auto apply = &PrivacyModule::apply_windows_feeds;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::SettingSync>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_setting_sync;
		static constexpr auto apply = &PrivacyModule::apply_setting_sync;
	};

	template<>
	struct FeatureTraits<PrivacyFeature::DiagnosticTasks>
	{
		using value_type = bool;

		static constexpr value_type fallback( ) noexcept
		{
			return true;
		}

		static constexpr auto load  = &PrivacyModule::load_diagnostic_tasks;
		static constexpr auto apply = &PrivacyModule::apply_diagnostic_tasks;
	};
}
