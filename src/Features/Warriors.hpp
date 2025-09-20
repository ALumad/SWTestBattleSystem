#pragma once
#include "Core/Entity.hpp"
#include "IO/Commands/Commands.hpp"

namespace sw::feature
{
	std::shared_ptr<core::Entity> CreateSwordsman(const sw::io::SpawnSwordsman& command);
	std::shared_ptr<core::Entity> CreateHunter(const sw::io::SpawnHunter& command);
}  // sw::feature::warriors
