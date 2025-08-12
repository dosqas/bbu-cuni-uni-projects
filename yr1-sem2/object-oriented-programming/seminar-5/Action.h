#pragma once
#include "Repository.h"
#include "FilePlaylist.h"
#include "Service.h"
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
	Repository& repo;
	Song addedSong;

public:
	ActionAdd(Repository& repo, const Song& addedSong) : repo{ repo }, addedSong{ addedSong } {}
	void executeUndo() override
	{
		repo.removeSong(addedSong);
	}
	void executeRedo() override
	{
		repo.addSong(addedSong);
	}
};

class ActionRemove : public Action
{
private:
	Repository& repo;
	Song deletedSong;
public:
	ActionRemove(Repository& repo, const Song& deletedSong) : repo{ repo }, deletedSong{ deletedSong } {}
	void executeUndo() override
	{
		repo.addSong(deletedSong);
	}
	void executeRedo() override
	{
		repo.removeSong(deletedSong);
	}
};

class ActionUpdate : public Action
{
private:
	Repository& repo;
	Song oldSong;
	Song newSong;

public:
	ActionUpdate(Repository& repo, const Song& oldSong, const Song& newSong) : repo{ repo }, oldSong{ oldSong }, newSong{ newSong } {}
	void executeUndo() override
	{
		repo.removeSong(newSong);
		repo.addSong(oldSong);
	}
	void executeRedo() override
	{
		repo.removeSong(oldSong);
		repo.addSong(newSong);
	}
};