#pragma once
#include "Core/System/System.hpp"

namespace sw::feature
{
	struct MarchSystem : core::ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<core::Entity>>& entities) const override;
		bool SkipEntity(const std::shared_ptr<core::Entity>& entity) const override;
	};

	struct AttackMeleeSystem : core::ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<core::Entity>>& entities) const override;
		bool SkipEntity(const std::shared_ptr<core::Entity>& entity) const override;
	};

	struct AttackRangeSystem : core::ISystem
	{
		uint32_t Apply(uint32_t tick, std::vector<std::shared_ptr<core::Entity>>& entities) const override;
		bool SkipEntity(const std::shared_ptr<core::Entity>& entity) const override;
	};

}  // sw::feature
