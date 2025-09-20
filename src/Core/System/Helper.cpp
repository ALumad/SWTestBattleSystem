#include "Helper.hpp"

#include "Core/Entity.hpp"

#include <random>

namespace sw::core::utils
{

	int64_t GetMoveStep(int64_t current, int64_t target, uint32_t speed)
	{
		int64_t diff = (target - current);
		if (diff == 0)
		{
			return 0;
		}

		auto step = std::min(std::abs(diff), static_cast<int64_t>(speed));
		return (diff < 0 ? -step : step);
	}

	std::vector<std::shared_ptr<Entity>> FindNearbyEntities(
		std::shared_ptr<Entity>& target,
		std::vector<std::shared_ptr<Entity>>& entities,
		uint32_t radius_from,
		uint32_t radius_to)
	{
		auto& pos = target->GetComponent<Position>(ComponentType::Position);
		std::vector<std::shared_ptr<Entity>> result;
		uint32_t radius_from_sqr = radius_from * radius_from;
		uint32_t radius_to_sqr = radius_to * radius_to;

		for (const auto& e : entities)
		{
			if (e->Id == target->Id)
			{
				continue;
			}
			if (!e->ContainsComponent(ComponentType::Position))
			{
				continue;
			}

			// TDOD: There should be code about occupied and possible attack flying unit

			auto& enemy_pos = e->GetComponent<Position>(ComponentType::Position);

			auto dx = enemy_pos.x - pos.x;
			auto dy = enemy_pos.y - pos.y;

			auto dist = dx * dx + dy * dy;

			if (radius_from_sqr <= dist && dist <= radius_to_sqr)
			{
				result.push_back(e);
			}
		}

		return result;
	}

	bool HasNextMoveActivity(std::shared_ptr<Entity>& target, std::vector<std::shared_ptr<Entity>>& entities)
	{
		if (target->ContainsComponent(ComponentType::March))
		{
			return true;
		}

		bool can_atack
			= target->ContainsComponent(ComponentType::Melee) | target->ContainsComponent(ComponentType::Range);

		if (!can_atack)
		{
			return false;
		}

		if (target->ContainsComponent(ComponentType::Melee))
		{
			if (!FindNearbyEntities(target, entities, 0, 1).empty())
			{
				return true;
			}
		}
		if (!target->ContainsComponent(ComponentType::Range))
		{
			return false;
		}

		Range& range = target->GetComponent<Range>(ComponentType::Range);
		return !FindNearbyEntities(target, entities, range.range_from, range.range_to).empty();
	}

	std::shared_ptr<Entity> GetRandomEntity(std::vector<std::shared_ptr<Entity>>& entities)
	{
		if (entities.empty())
		{
			return {};
		}
		static std::mt19937 gen(std::random_device{}());
		std::uniform_int_distribution<> dist(0, entities.size() - 1);
		return entities[dist(gen)];
	}
}  // sw::core::utils
