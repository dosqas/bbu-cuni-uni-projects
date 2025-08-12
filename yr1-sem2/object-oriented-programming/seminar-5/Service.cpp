#include "Service.h"
#include <algorithm>
#include "FilePlaylist.h"
#include "RepositoryExceptions.h"

using namespace std;

void Service::addSongToRepository(const std::string& artist, const std::string& title, double minutes, double seconds, const std::string& source)
{
	Song s{ artist, title, Duration{minutes, seconds}, source };
	this->validator.validate(s);
	this->repo.addSong(s);

	this->undoActions.push_back(make_unique<ActionAdd>(this->repo, s));
	this->redoActions.clear();
}

void Service::removeSongFromRepository(const std::string & artist, const std::string & title)
{
	Song s = this->repo.findByArtistAndTitle(artist, title);
	this->repo.removeSong(s);

	this->undoActions.push_back(make_unique<ActionRemove>(this->repo, s));
	this->redoActions.clear();
}

void Service::addSongToPlaylist(const Song& song)
{
	if (this->playList == nullptr)
		return;
	this->playList->add(song);
}

void Service::addAllSongsByArtistToPlaylist(const std::string& artist)
{
	vector<Song> songs = this->repo.getSongs();
	int nSongs = static_cast<int>(count_if(songs.begin(), songs.end(),
		[artist](const Song& s)
		{
			return s.getArtist() == artist;
		}));

	vector<Song> songsByArtist(nSongs);
	copy_if(songs.begin(), songs.end(), songsByArtist.begin(),
		[artist](const Song& s)
		{
			return s.getArtist() == artist;
		});

	for (auto s : songsByArtist)
		this->playList->add(s);
}

void Service::startPlaylist()
{
	if (this->playList == nullptr)
		return;
	this->playList->play();
}

void Service::nextSongPlaylist()
{
	if (this->playList == nullptr)
		return;
	this->playList->next();
}

void Service::savePlaylist(const std::string& filename)
{
	if (this->playList == nullptr)
		return;

	this->playList->setFilename(filename);
	this->playList->writeToFile();
}

void Service::openPlaylist() const
{
	if (this->playList == nullptr)
		return;

	this->playList->displayPlaylist();
}

void Service::undo()
{
	Action* a = this->undoActions.back().get();
	this->redoActions.push_back(move(this->undoActions.back()));
	this->undoActions.pop_back();
	a->executeUndo();
}

void Service::redo()
{
	Action* a = this->redoActions.back().get();
	a->executeRedo();
}

bool Service::canUndoRedo(int op) const
{
	if (op == 1)
		return !this->undoActions.empty();
	if (op == 2)
		return !this->redoActions.empty();
	return false;
}

void Service::updateSong(const std::string& artist, const std::string& title, double minutes, double seconds, const std::string& source)
{
	Song s = this->repo.findByArtistAndTitle(artist, title);
	Song oldSong = s;
	s.setDuration(Duration{ minutes, seconds });
	s.setSource(source);
	this->repo.removeSong(oldSong);
	this->repo.addSong(s);
	
	this->undoActions.push_back(make_unique<ActionUpdate>(this->repo, oldSong, s));
	this->redoActions.clear();
}
