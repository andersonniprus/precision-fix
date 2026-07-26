#pragma once

class ServiceHub
{
public:
	template<typename T>
	void register_service( const std::shared_ptr<T>& service )
	{
		services_[ std::type_index( typeid( T ) ) ] = service;
	}

	template<typename... Ts>
	void register_services( const std::shared_ptr<Ts>&... services )
	{
		( register_service( services ), ... );
	}

	void clear( )
	{
		services_.clear( );
	}

	template<typename T>
	[[nodiscard]] std::shared_ptr<T> get( ) const
	{
		const auto it = services_.find( std::type_index( typeid( T ) ) );

		assert( it != services_.end ( ) && "Service not found!" );

		return std::any_cast<std::shared_ptr<T>>( it->second );
	}

private:
	std::unordered_map<std::type_index, std::any> services_;
};
