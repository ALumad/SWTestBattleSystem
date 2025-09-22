#pragma once

#include "Component.hpp"

#include <cstdint>
#include <memory.h>
#include <string>
#include <unordered_map>

namespace sw::core
{

	class Entity
	{
	public:
		Entity(uint32_t Id, const std::string& UnitType) :
				Id(Id),
				UnitType(UnitType)
		{}

		bool ContainsComponent(ComponentType type) const;
		void RemoveComponent(ComponentType type);

		template <class TComponent>
		Entity& AddComponent(ComponentType type, TComponent&& component)
		{
			components_[type] = std::move(component);
			return *this;
		}

		template <class TComponent>
		TComponent& GetComponent(ComponentType type)
		{
			return std::get<TComponent>(components_[type]);
		}

		const uint32_t Id{};
		const std::string UnitType{};

	private:
		std::unordered_map<ComponentType, DynamicComponent> components_;
	};

	// Bind CompnentType with Component
	class EntityBuilder
	{
	public:
		EntityBuilder(uint32_t Id, const std::string& UnitType);
		std::shared_ptr<Entity> Build();
		EntityBuilder& AddPosition(Position&& position);
		EntityBuilder& AddMarch(March&& movable);
		EntityBuilder& AddHealth(Health&& health);
		EntityBuilder& AddMelee(Melee&& melee);
		EntityBuilder& AddRange(Range&& range);
		EntityBuilder& AddOccupied();

	private:
		std::shared_ptr<Entity> entity_;
	};

}  // sw::core
