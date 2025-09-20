#include "Game.hpp"

#include "IO/Events/Events.hpp"
#include "IO/System/EventLog.hpp"
#include "System/System.hpp"

#include <cstdint>
#include <sstream>

namespace sw::core
{
	Game::Game(uint32_t width, uint32_t height) :
			width_(width),
			height_(height)
	{
		RegisterSystems();
		EventLog::log(tick_, io::MapCreated{width_, height});
	}

	void Game::CheckCoord(uint32_t x, uint32_t y)
	{
		if (x < width_ && y < height_)
		{
			return;
		}
		std::stringstream err_msg;
		err_msg << "The coord (" << x << ", " << y << ") is out of the map!";
		throw std::logic_error(err_msg.str());
	}

	void Game::AddEntity(std::shared_ptr<Entity>&& entity)
	{
		auto& pos = entity->GetComponent<Position>(ComponentType::Position);
		uint32_t x = pos.x;
		uint32_t y = pos.y;

		CheckCoord(x, y);
		entities_.emplace_back(std::move(entity));

		auto& e = entities_.back();
		EventLog::log(tick_, io::UnitSpawned{e->Id, e->UnitType, x, y});
	}

	void Game::AddMarch(uint32_t id, uint32_t x, uint32_t y)
	{
		CheckCoord(x, y);
		for (auto& entity : entities_)
		{
			if (entity->Id != id)
			{
				continue;
			}
			entity->AddComponent(ComponentType::March, March{.target = {x, y}});
			auto& pos = entity->GetComponent<Position>(ComponentType::Position);
			uint32_t start_x = pos.x;
			uint32_t start_y = pos.y;
			EventLog::log(tick_, io::MarchStarted{id, start_x, start_y, x, y});
			break;
		}
	}

	bool Game::DoTick()
	{
		uint32_t score = 0;
		for (const auto& system : systems_)
		{
			score = system->Apply(tick_, entities_);
		}
		++tick_;
		return score > 0;
	}

	void Game::RegisterSystems()
	{
		// Emplace order matters
		// Maybe add priority, but in that case it looks unnecessary
		systems_.emplace_back(std::make_unique<AttackSystem>());
		systems_.emplace_back(std::make_unique<MarchSystem>());
		systems_.emplace_back(std::make_unique<FinalizeMoveSystem>());
		systems_.emplace_back(std::make_unique<ScoreSystem>());
	}

}  // sw::core
