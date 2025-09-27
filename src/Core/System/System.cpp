#include "System.hpp"

#include "Core/Component.hpp"
#include "Core/Entity.hpp"
#include "Helper.hpp"
#include "IO/Events/Events.hpp"
#include "IO/System/EventLog.hpp"

#include <algorithm>
#include <cstdint>
#include <ranges>

namespace sw::core
{

	bool FinalizeMoveSystem::SkipEntity(const std::shared_ptr<Entity>& entity) const
	{
		return !entity->ContainsComponent(ComponentType::HasActed);
	}

	uint32_t FinalizeMoveSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const
	{
		// remove died entities
		auto end_it = std::remove_if(
			entities.begin(),
			entities.end(),
			[tick](const std::shared_ptr<Entity>& e)
			{
				bool dead = e->ContainsComponent(ComponentType::Dead);
				if (dead)
				{
					EventLog::log(tick, io::UnitDied{e->Id});
				}
				return dead;
			});

		entities.erase(end_it, entities.end());

		std::for_each(
			entities.begin(),
			entities.end(),
			[&](std::shared_ptr<Entity>& e)
			{
				if (SkipEntity(e))
				{
					return;
				}
				e->RemoveComponent(ComponentType::HasActed);
			});
		return entities.size();
	}

	bool ScoreSystem::SkipEntity([[maybe_unused]] const std::shared_ptr<Entity>& entity) const
	{
		return false;
	}

	uint32_t ScoreSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const
	{
		if (entities.size() <= 1)
		{
			return 0;
		}
		return std::count_if(
			entities.begin(),
			entities.end(),
			[&](std::shared_ptr<Entity>& e) { return utils::HasNextMoveActivity(e, entities); });
	}

}  // sw::core
