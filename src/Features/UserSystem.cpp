#include "Features/UserSystem.hpp"

#include "Core/Entity.hpp"
#include "Core/System/Helper.hpp"
#include "IO/Events/Events.hpp"
#include "IO/System/EventLog.hpp"

namespace sw::feature
{
	using namespace sw::core;

	bool MarchSystem::SkipEntity(const std::shared_ptr<core::Entity>& entity) const
	{
		if (entity->ContainsComponent(ComponentType::HasActed))
		{
			return true;
		}
		if (!entity->ContainsComponent(ComponentType::March))
		{
			return true;
		}
		if (!entity->ContainsComponent(ComponentType::Position))
		{
			throw std::logic_error("Entity can move, but doesn't have position");
		}
		return false;
	}

	uint32_t MarchSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<core::Entity>>& entities) const
	{
		uint32_t acted = 0;
		for (auto& entity : entities)
		{
			if (SkipEntity(entity))
			{
				continue;
			}

			auto& march = entity->GetComponent<March>(ComponentType::March);
			auto& pos = entity->GetComponent<Position>(ComponentType::Position);

			pos.x += utils::GetMoveStep(pos.x, march.target.x, march.speed);
			pos.y += utils::GetMoveStep(pos.y, march.target.y, march.speed);

			EventLog::log(tick, io::UnitMoved{entity->Id, static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y)});

			if (pos == march.target)
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

	void ApplyDamage(uint32_t tick, uint32_t unit_id, uint32_t damage, std::shared_ptr<core::Entity>& enemy)
	{
		auto& health = enemy->GetComponent<Health>(ComponentType::Health);
		damage = std::min(damage, health.health);
		health.health -= damage;
		if (health.health == 0)
		{
			enemy->AddComponent(ComponentType::Dead, Dead{});
		}

		io::UnitAttacked event{unit_id, enemy->Id, damage, health.health};
		EventLog::log(tick, std::move(event));
	}

	bool AttackMeleeSystem::SkipEntity(const std::shared_ptr<core::Entity>& entity) const
	{
		return entity->ContainsComponent(ComponentType::HasActed) || !entity->ContainsComponent(ComponentType::Melee);
	}

	uint32_t AttackMeleeSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<core::Entity>>& entities) const
	{
		uint32_t acted = 0;
		// Find enemies for attask is O(n) => this mehthod has O(n^2) complexity
		// It possible improve if use grid/map with coords
		for (auto& entity : entities)
		{
			if (SkipEntity(entity))
			{
				continue;
			}

			auto& melee = entity->GetComponent<Melee>(ComponentType::Melee);
			auto enemies = utils::FindNearbyEntities(entity, entities, 0, 1);
			std::shared_ptr<core::Entity> enemy = utils::GetRandomEntity(enemies);
			if (!enemy)
			{
				continue;
			}

			ApplyDamage(tick, entity->Id, melee.strength, enemy);
			entity->AddComponent(ComponentType::HasActed, HasActed{});
			++acted;
		}
		return acted;
	}

	bool AttackRangeSystem::SkipEntity(const std::shared_ptr<core::Entity>& entity) const
	{
		return entity->ContainsComponent(ComponentType::HasActed) || !entity->ContainsComponent(ComponentType::Range);
	}

	uint32_t AttackRangeSystem::Apply(uint32_t tick, std::vector<std::shared_ptr<core::Entity>>& entities) const
	{
		uint32_t acted = 0;
		for (auto& entity : entities)
		{
			if (SkipEntity(entity))
			{
				continue;
			}

			auto& range = entity->GetComponent<Range>(ComponentType::Range);

			auto enemies = utils::FindNearbyEntities(entity, entities, range.range_from, range.range_to);
			std::shared_ptr<core::Entity> enemy = utils::GetRandomEntity(enemies);
			if (!enemy)
			{
				continue;
			}

			ApplyDamage(tick, entity->Id, range.agility, enemy);
			entity->AddComponent(ComponentType::HasActed, HasActed{});
			++acted;
		}
		return acted;
	}

}  // sw::feature
