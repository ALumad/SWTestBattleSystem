#include "Core/System/System.hpp"

#include <Core/Component.hpp>
#include <Core/Entity.hpp>
#include <Core/Game.hpp>
#include <Features/UserSystem.hpp>
#include <Features/Warriors.hpp>
#include <IO/Commands/Commands.hpp>
#include <IO/System/CommandParser.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	auto game = std::make_unique<core::Game>();
	game->RegisterSystem(core::SystemCategory::Effect, std::make_unique<feature::AttackMeleeSystem>())
		.RegisterSystem(core::SystemCategory::Effect, std::make_unique<feature::AttackRangeSystem>())
		.RegisterSystem(core::SystemCategory::Movement, std::make_unique<feature::MarchSystem>());

	io::CommandParser parser;
	parser.add<io::CreateMap>([&](auto command) { game->InitMap(command.width, command.height); })
		.add<io::SpawnSwordsman>([&](auto command) { game->AddEntity(sw::feature::CreateSwordsman(command)); })
		.add<io::SpawnHunter>([&](auto command) { game->AddEntity(sw::feature::CreateHunter(command)); })
		.add<io::March>([&](auto command) { game->AddMarch(command.unitId, command.targetX, command.targetY); });
	parser.parse(file);

	bool has_next_move = true;
	while (has_next_move)
	{
		has_next_move = game->DoTick();
	}

	return 0;
}
