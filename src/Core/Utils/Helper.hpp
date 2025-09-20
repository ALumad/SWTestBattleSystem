#pragma once
#include <variant>

namespace sw::core::utils
{
	template <typename T, typename... Ts>
	inline constexpr bool contains_v = (std::is_same_v<T, Ts> || ...);

	template <typename...>
	inline constexpr bool are_unique_v = true;

	template <typename T, typename... Rest>
	inline constexpr bool are_unique_v<T, Rest...> = !contains_v<T, Rest...> && are_unique_v<Rest...>;

	template <typename Variant>
	struct variant_has_unique_types;

	template <typename... Ts>
	struct variant_has_unique_types<std::variant<Ts...>> : std::bool_constant<are_unique_v<Ts...>>
	{};
}  // sw::utils
