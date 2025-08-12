from ui.ui import UI
import configparser

from services.ClientService import ClientService
from services.MovieService import MovieService
from services.RentalService import RentalService
from services.UndoService import UndoService

from repository.ClientRepo import ClientRepo
from repository.MovieRepo import MovieRepo
from repository.RentalRepo import RentalRepo
from repository.ClientTextRepo import ClientTextRepo
from repository.MovieTextRepo import MovieTextRepo
from repository.RentalTextRepo import RentalTextRepo
from repository.ClientBinaryRepo import ClientBinaryRepo
from repository.MovieBinaryRepo import MovieBinaryRepo
from repository.RentalBinaryRepo import RentalBinaryRepo

from datetime import date as day
import unittest

from src.test.test import TestClientService
from src.test.test import TestMovieService
from src.test.test import TestRentalService


def start():
    print("Today is", day.today(), "\n")
    while True:
        UI.input()


def for_tests():
    suite = unittest.TestLoader().loadTestsFromTestCase(TestClientService)
    suite.addTests(unittest.TestLoader().loadTestsFromTestCase(TestMovieService))
    suite.addTests(unittest.TestLoader().loadTestsFromTestCase(TestRentalService))
    unittest.TextTestRunner().run(suite)


def config_settings():
    config = configparser.ConfigParser()
    config.read('settings.properties')
    client_repos = None
    movie_repos = None
    rental_repos = None

    repository_name = config.get('Config', 'repository')

    if repository_name == "binary":
        client_repo_file_name = config.get('Config', 'client_repo_file')
        movie_repo_file_name = config.get('Config', 'movie_repo_file')
        rental_repo_file_name = config.get('Config', 'rental_repo_file')

        rental_repos = RentalBinaryRepo(rental_repo_file_name)
        client_repos = ClientBinaryRepo(client_repo_file_name, rental_repos)
        movie_repos = MovieBinaryRepo(movie_repo_file_name, rental_repos)

        repository_name = "Binary"
    elif repository_name == "text":
        client_repo_file_name = config.get('Config', 'client_repo_file')
        movie_repo_file_name = config.get('Config', 'movie_repo_file')
        rental_repo_file_name = config.get('Config', 'rental_repo_file')

        rental_repos = RentalTextRepo(rental_repo_file_name)
        client_repos = ClientTextRepo(client_repo_file_name, rental_repos)
        movie_repos = MovieTextRepo(movie_repo_file_name, rental_repos)

        repository_name = "Text"
    elif repository_name == "memory":
        rental_repos = RentalRepo()
        client_repos = ClientRepo(rental_repos)
        movie_repos = MovieRepo(rental_repos)

        repository_name = "Memory"

    print(f"Using repository: {repository_name}\n")

    return client_repos, movie_repos, rental_repos


if __name__ == '__main__':
    for_tests()
    Client_Repo, Movie_Repo, Rental_Repo = config_settings()

    Client_Service = ClientService(Client_Repo)
    Movie_Service = MovieService(Movie_Repo)
    Rental_Service = RentalService(Rental_Repo)

    UndoService = UndoService(Client_Service, Movie_Service, Rental_Service)

    UI = UI(Client_Service, Movie_Service, Rental_Service, UndoService)

    start()
