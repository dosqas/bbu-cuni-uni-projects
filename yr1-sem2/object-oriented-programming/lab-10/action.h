#pragma once
#include "service.h"
#include "repository.h"
using namespace std;

class Action
{
public:
	virtual void executeUndo() = 0;
	virtual void executeRedo() = 0;
};

class ActionAdd : public Action
{
private:
	TextRepository& repo;
	Dog addedDog;

public:
	ActionAdd(TextRepository& repo, const Dog& addedDog) : repo{ repo }, addedDog{ addedDog } {}
	void executeUndo() override
	{
		repo.removeRepoDog(addedDog.get_photograph());
	}
	void executeRedo() override
	{
		repo.addRepoDog(addedDog);
	}
};

class ActionRemove : public Action
{
private:
	TextRepository& repo;
	Dog deletedDog;
public:
	ActionRemove(TextRepository& repo, const Dog& deletedDog) : repo{ repo }, deletedDog{ deletedDog } {}
	void executeUndo() override
	{
		repo.addRepoDog(deletedDog);
	}
	void executeRedo() override
	{
		repo.removeRepoDog(deletedDog.get_photograph());
	}
};

class ActionUpdate : public Action
{
private:
	TextRepository& repo;
	Dog oldDog;
	Dog newDog;

public:
	ActionUpdate(TextRepository& repo, const Dog& oldDog, const Dog& newDog) : repo{ repo }, oldDog{ oldDog }, newDog{ newDog } {}
	void executeUndo() override
	{
		repo.updateRepoDog(oldDog, newDog.get_name(), newDog.get_breed(), newDog.get_age());
	}
	void executeRedo() override
	{
		repo.updateRepoDog(newDog, oldDog.get_name(), oldDog.get_breed(), oldDog.get_age());
	}
};