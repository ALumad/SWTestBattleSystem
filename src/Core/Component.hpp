#pragma once
#include "Utils/Helper.hpp"

#include <cstdint>
#include <utility>
#include <variant>

namespace sw::core
{

	enum class ComponentType : int
	{
		Position,
		March,
		Health,
		Melee,
		Range,
		HasActed,
		Occupied,
		Dead,
		END
	};

	struct Position
	{
		// because i dont want to think about overflow thats why int64
		int64_t x;
		int64_t y;
		auto operator<=>(const Position& other) const = default;
	};

	using Target = Position;

	struct March
	{
		Target target;
		uint32_t speed{1};	// seems speed is always >= 0, or it's something about speed + direction = vilocity(?).. idk
	};

	// If an entity is unkillable: it doesn't have Health, or have const Health, or have unkillable component?
	struct Health
	{
		uint32_t health;
	};

	struct Melee
	{
		uint32_t strength;
	};

	struct Range
	{
		uint32_t agility;
		uint32_t range_from;
		uint32_t range_to;
	};

	struct HasActed
	{};

	struct Occupied
	{};

	struct Dead
	{};

	using DynamicComponent = std::variant<Position, March, Health, Melee, Range, HasActed, Occupied, Dead>;

	static_assert(
		utils::variant_has_unique_types<DynamicComponent>::value,
		"Some of components present more then one time in DynamicComponent");

	static_assert(
		std::variant_size<DynamicComponent>::value == static_cast<int>(ComponentType::END),
		"Sizes of ComponentType and DynamicComponent aren't equal");
}  // sw::core
