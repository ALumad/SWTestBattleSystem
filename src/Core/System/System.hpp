#pragma once
#include "IO/Events/Events.hpp"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace sw::core
{
	struct Entity;

	struct ISystem
	{
		virtual uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const = 0;
		virtual ~ISystem() = default;
	};

	struct MarchSystem : ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const override;
	};

	struct AttackSystem : ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const override;
		static std::optional<io::UnitAttacked> DoAttack(
			std::shared_ptr<Entity>& entity, std::vector<std::shared_ptr<Entity>>& entities);
	};

	struct FinalizeMoveSystem : ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const override;
	};

	struct ScoreSystem : ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const override;
	};
}  // sw::core
