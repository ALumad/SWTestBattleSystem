cat src/main.cpp
#include <Core/Component.hpp>
#include <Core/Entity.hpp>
#include <Core/Game.hpp>
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

	io::CommandParser parser;
	std::unique_ptr<core::Game> game;
	parser.add<io::CreateMap>([&](auto command) { game = std::make_unique<core::Game>(command.width, command.height); })
		.add<io::SpawnSwordsman>([&](auto command) { game->AddEntity(sw::feature::CreateSwordsman(command)); })
		.add<io::SpawnHunter>([&](auto command) { game->AddEntity(sw::feature::CreateHunter(command)); })
		.add<io::March>([&](auto command) { game->AddMarch(command.unitId, command.targetX, command.targetY); });

	std::cout << "\n\nEvents:\n";
	parser.parse(file);
	bool has_next_move = true;
	while (has_next_move)
	{
		has_next_move = game->DoTick();
	}

	return 0;
}
