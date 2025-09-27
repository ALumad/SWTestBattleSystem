#pragma once
#include "IO/Events/Events.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace sw::core
{
	struct Entity;

	enum class SystemCategory : int
	{
		Effect,
		Movement,
		Internal,
	};

	struct ISystem
	{
		virtual uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const = 0;
		virtual bool SkipEntity(const std::shared_ptr<Entity>& entity) const = 0;
		virtual ~ISystem() = default;
	};

	struct FinalizeMoveSystem : ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const override;
		bool SkipEntity(const std::shared_ptr<Entity>& entity) const override;
	};

	struct ScoreSystem : ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<Entity>>& entities) const override;
		bool SkipEntity(const std::shared_ptr<Entity>& entity) const override;
	};
}  // sw::core
