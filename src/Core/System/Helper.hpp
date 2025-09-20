#pragma once
#include <memory>
#include <vector>

namespace sw::core
{

	struct Entity;

	namespace utils
	{
		int64_t GetMoveStep(int64_t current, int64_t target, uint32_t speed);
		bool HasNextMoveActivity(std::shared_ptr<Entity>& target, std::vector<std::shared_ptr<Entity>>& entities);
		std::vector<std::shared_ptr<Entity>> FindNearbyEntities(
			std::shared_ptr<Entity>& target,
			std::vector<std::shared_ptr<Entity>>& entities,
			uint32_t radius_from,
			uint32_t radius_to);

		std::shared_ptr<Entity> GetRandomEntity(std::vector<std::shared_ptr<Entity>>& entities);

	}

}
