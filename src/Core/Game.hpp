#pragma once
#include "Entity.hpp"
#include "System/System.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace sw::core
{

	class Game
	{
	public:
		Game(uint32_t width, uint32_t height);
		void AddEntity(std::shared_ptr<Entity>&& entity);
		void AddMarch(uint32_t id, uint32_t x, uint32_t y);
		bool DoTick();

	private:
		void RegisterSystems();
		void CheckCoord(uint32_t x, uint32_t y);
		std::vector<std::shared_ptr<Entity>> entities_;
		std::vector<std::unique_ptr<ISystem>> systems_;

		uint32_t height_;
		uint32_t width_;
		uint32_t tick_{1};
	};
}  // sw::core
