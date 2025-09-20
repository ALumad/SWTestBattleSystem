#include "Entity.hpp"

#include "Component.hpp"

#include <stdexcept>

namespace sw::core
{

	bool Entity::ContainsComponent(ComponentType type) const
	{
		return components_.contains(type);
	}

	void Entity::RemoveComponent(ComponentType type)
	{
		components_.erase(type);
	}

	EntityBuilder::EntityBuilder(uint32_t Id, const std::string& UnitType) :
			entity_(std::make_shared<Entity>(Id, UnitType))
	{}

	EntityBuilder& EntityBuilder::AddPosition(Position&& position)
	{
		entity_->AddComponent(ComponentType::Position, std::move(position));
		return *this;
	}

	EntityBuilder& EntityBuilder::AddMarch(March&& movable)
	{
		entity_->AddComponent(ComponentType::March, std::move(movable));
		return *this;
	}

	EntityBuilder& EntityBuilder::AddHealth(Health&& health)
	{
		entity_->AddComponent(ComponentType::Health, std::move(health));
		return *this;
	}

	EntityBuilder& EntityBuilder::AddMelee(Melee&& melee)
	{
		entity_->AddComponent(ComponentType::Melee, std::move(melee));
		return *this;
	}

	EntityBuilder& EntityBuilder::AddRange(Range&& range)
	{
		entity_->AddComponent(ComponentType::Range, std::move(range));
		return *this;
	}

	EntityBuilder& EntityBuilder::AddOccupied()
	{
		entity_->AddComponent(ComponentType::Occupied, Occupied{});
		return *this;
	}

	std::shared_ptr<Entity> EntityBuilder::Build()
	{
		if (!entity_)
		{
			throw std::logic_error("Entity has already built");
		}

		return std::exchange(entity_, nullptr);
	}

}  // sw::core
