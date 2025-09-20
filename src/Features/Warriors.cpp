#include "Features/Warriors.hpp"

namespace sw::feature
{
std::shared_ptr<core::Entity> CreateSwordsman(const sw::io::SpawnSwordsman& command)
{
    core::EntityBuilder builder(command.unitId, "Swordsman");
    auto swordsman = builder.AddHealth(core::Health{.health = command.hp})
						 .AddPosition(core::Position{.x = command.x, .y = command.y})
						 .AddMelee(core::Melee{.strength = command.strength})
						 .Build();
    return swordsman;

}
std::shared_ptr<core::Entity> CreateHunter(const sw::io::SpawnHunter& command)
{
    core::EntityBuilder builder(command.unitId, "Hunter");
    auto hunter = builder.AddHealth(core::Health{.health = command.hp})
						 .AddPosition(core::Position{.x = command.x, .y = command.y})
						 .AddMelee(core::Melee{.strength = command.strength})
						 .AddRange(core::Range{.range_from = 2, .range_to = command.range, .agility = command.agility})
						 .Build();
    return hunter;
}
}

