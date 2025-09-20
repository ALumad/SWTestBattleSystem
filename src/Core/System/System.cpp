#include "System.hpp"

#include "Core/Component.hpp"
#include "Core/Entity.hpp"
#include "Helper.hpp"
#include "IO/Events/Events.hpp"
#include "IO/System/EventLog.hpp"

#include <algorithm>
#include <cstdint>

namespace sw::core
{

	uint32_t MarchSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const
	{
		uint32_t acted = 0;
		for (auto& entity : entities)
		{
			if (entity->ContainsComponent(ComponentType::HasActed))
			{
				continue;
			}
			if (!entity->ContainsComponent(ComponentType::March))
			{
				continue;
			}
			if (!entity->ContainsComponent(ComponentType::Position))
			{
				throw std::logic_error("Entity can move, but doesn't have position");
			}

			auto& movable = entity->GetComponent<March>(ComponentType::March);
			auto& pos = entity->GetComponent<Position>(ComponentType::Position);

			pos.x += utils::GetMoveStep(pos.x, movable.target.x, movable.speed);
			pos.y += utils::GetMoveStep(pos.y, movable.target.y, movable.speed);

			EventLog::log(tick, io::UnitMoved{entity->Id, static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y)});

			if (pos == movable.target)
			{
				entity->RemoveComponent(ComponentType::March);
				EventLog::log(
					tick, io::MarchEnded{entity->Id, static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y)});
			}

			entity->AddComponent(ComponentType::HasActed, HasActed{});
			++acted;
		}
		return acted;
	}

	std::optional<io::UnitAttacked> AttackSystem::DoAttack(
		std::shared_ptr<Entity>& entity, std::vector<std::shared_ptr<Entity>>& entities)
	{
		if (entity->ContainsComponent(ComponentType::Melee))
		{
			auto enemies = utils::FindNearbyEntities(entity, entities, 0, 1);
			std::shared_ptr<Entity> enemy = utils::GetRandomEntity(enemies);
			if (enemy)
			{
				auto& melee = entity->GetComponent<Melee>(ComponentType::Melee);
				auto& enemy_health = enemy->GetComponent<Health>(ComponentType::Health);
				auto damage = std::min(melee.strength, enemy_health.health);
				enemy_health.health -= damage;
				return io::UnitAttacked{entity->Id, enemy->Id, damage, enemy_health.health};
			}
		}

		if (!entity->ContainsComponent(ComponentType::Range))
		{
			return {};
		}

		auto& range = entity->GetComponent<Range>(ComponentType::Range);

		auto enemies = utils::FindNearbyEntities(entity, entities, range.range_from, range.range_to);
		std::shared_ptr<Entity> enemy = utils::GetRandomEntity(enemies);
		if (!enemy)
		{
			return {};
		}

		auto& enemy_health = enemy->GetComponent<Health>(ComponentType::Health);
		auto damage = std::min(range.agility, enemy_health.health);
		enemy_health.health -= damage;
		if (enemy_health.health == 0)
		{
			enemy->AddComponent(ComponentType::Dead, Dead{});
		}
		return io::UnitAttacked{entity->Id, enemy->Id, damage, enemy_health.health};
	}

	uint32_t AttackSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const
	{
		uint32_t acted = 0;
		// Find enemies for attask is O(n) => this mehthod has O(n^2) complexity
		// It possible improve if use grid/map with coords
		for (auto& entity : entities)
		{
			if (entity->ContainsComponent(ComponentType::HasActed))
			{
				continue;
			}

			bool can_attack
				= entity->ContainsComponent(ComponentType::Melee) | entity->ContainsComponent(ComponentType::Range);

			if (!can_attack)
			{
				continue;
			}

			if (!entity->ContainsComponent(ComponentType::Position))
			{
				throw std::logic_error(
					"std::shared_ptr<Entity> doesn't have Position component and can't detect enemy arround him");
			}
			auto event = DoAttack(entity, entities);

			if (!event)
			{
				continue;
			}

			EventLog::log(tick, std::move(event.value()));
			entity->AddComponent(ComponentType::HasActed, HasActed{});
			++acted;
		}
		return acted;
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

		for (auto& entity : entities)
		{
			if (entity->ContainsComponent(ComponentType::HasActed))
			{
				entity->RemoveComponent(ComponentType::HasActed);
			}
		}
		return entities.size();
	}

	uint32_t ScoreSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const
	{
		if (entities.size() <= 1)
		{
			return 0;
		}
		uint32_t total_score = 0;

		// O(n^2) for the same reason as above
		for (auto& entity : entities)
		{
			if (utils::HasNextMoveActivity(entity, entities))
			{
				++total_score;
			}
		}
		return total_score;
	}

}  // sw::core
