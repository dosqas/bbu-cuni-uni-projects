import unittest
from datetime import date
from src.services.ClientService import ClientService
from src.services.MovieService import MovieService
from src.repository.ClientRepo import ClientRepo
from src.repository.MovieRepo import MovieRepo
from src.repository.RentalRepo import RentalRepo
from src.services.RentalService import RentalService
from src.domain.RentalDomain import Rental
from src.domain.ClientDomain import Client
from src.domain.MovieDomain import Movie


class TestClientService(unittest.TestCase):
    def setUp(self):
        self.__client_repository = ClientRepo()
        self.__client_service = ClientService(self.__client_repository)

    def test_client_add(self):
        self.__client_service.add_client(1, "Client1")
        self.assertEqual(len(self.__client_repository.list_clients()), 21)

    def test_client_update(self):
        self.__client_service.update_client("3", "Client1")
        self.assertEqual(self.__client_service.can_update_client("3"), True)
        self.assertEqual(self.__client_repository.list_clients()[2].client_name(), "Client1")

    def test_client_uniques(self):
        self.assertEqual(self.__client_service.check_client_id_unique("1"), False)
        self.assertEqual(self.__client_service.check_client_id_unique("21"), True)
        self.assertEqual(self.__client_service.check_client_id("1"), True)

    def test_client_remove(self):
        self.assertEqual(self.__client_service.can_remove_client("2"), True)
        self.assertEqual(self.__client_service.can_remove_client("21"), False)
        self.__client_service.remove_client("2")
        self.assertEqual(len(self.__client_repository.list_clients()), 19)
        self.assertEqual(self.__client_service.can_remove_client("2"), False)

    def tests_for_domain(self):
        self.assertEqual(self.__client_repository.list_clients()[0].client_id(), "1")
        self.assertEqual(self.__client_repository.list_clients()[0].client_name(), "Client1")
        self.assertEqual(str(self.__client_repository.list_clients()[0]), "#1 Client1")
        self.assertEqual(self.__client_repository.list_clients()[0].client_id(), "1")
        self.assertEqual(self.__client_repository.list_clients()[0].client_name(), "Client1")
        self.assertEqual(str(self.__client_repository.list_clients()[0]), "#1 Client1")
        self.__client_service.search_client_by_name("Client1")
        self.__client_service.search_client_by_id("1")


class TestMovieService(unittest.TestCase):
    def setUp(self):
        self.__movie_repository = MovieRepo()
        self.__movie_service = MovieService(self.__movie_repository)

    def test_movie_add(self):
        self.__movie_service.add_movie(21, "Movie21", "Description21", "Genre21")
        self.assertEqual(len(self.__movie_repository.get_all_movies()), 21)

    def test_movie_update(self):
        self.__movie_service.update_movie("3", "Movie1", "Description1", "Genre1")
        self.assertEqual(self.__movie_service.can_update_movie("3"), True)
        self.assertEqual(self.__movie_repository.get_all_movies()[2].movie_title(), "Movie1")
        self.assertEqual(self.__movie_repository.get_all_movies()[2].movie_description(), "Description1")
        self.assertEqual(self.__movie_repository.get_all_movies()[2].movie_genre(), "Genre1")

    def test_movie_uniques(self):
        self.assertEqual(self.__movie_service.check_movie_id_unique("1"), False)
        self.assertEqual(self.__movie_service.check_movie_id("1"), True)

    def test_movie_remove(self):
        self.assertEqual(self.__movie_service.can_remove_movie("2"), True)
        self.assertEqual(self.__movie_service.can_remove_movie("21"), False)
        self.__movie_service.remove_movie("2")
        self.assertEqual(len(self.__movie_repository.get_all_movies()), 19)
        self.assertEqual(self.__movie_service.can_remove_movie("2"), False)
        self.__movie_service.search_movie_by_id("1")
        self.__movie_service.search_movie_by_title("Movie1")
        self.__movie_service.search_movie_by_description("Description1")
        self.__movie_service.search_movie_by_genre("Genre1")
        self.__movie_service.list_movies()
        self.__movie_service.check_movie_id_unique("1")


class TestRentalService(unittest.TestCase):
    def setUp(self):
        self.__movie_repository = MovieRepo()
        self.__client_repository = ClientRepo()
        self.__rental_repository = RentalRepo()
        self.__rental_service = RentalService(self.__rental_repository)

    def test_rental_add(self):
        self.__rental_repository.add(Rental("21", "21", "21", "21", "21", "21"))
        self.assertEqual(len(self.__rental_repository.get_all()), 21)
        self.assertEqual(self.__rental_service.check_valid_date("2022-1-1"), True)
        self.assertEqual(self.__rental_service.check_valid_date("2020-33-1"), False)
        self.assertEqual(self.__rental_repository.get_all()[0].movie_id(), self.__rental_repository.get_all()[0].movie_id())
        self.assertEqual(self.__rental_repository.get_all()[0].client_id(), "1")
        self.assertEqual(self.__rental_repository.get_all()[0].rental_id(), "1")
        self.assertEqual(self.__rental_repository.get_all()[0].returned_date(), "0")
        self.assertEqual(self.__rental_repository.get_all()[0].set_returned_date("3"), None)

    def test_rental_service(self):
        self.__rental_repository.get_all_clients()
        self.__rental_repository.get_all_movies()
        self.__rental_repository.find_by_id("1")
        self.__rental_repository.count_rentals_movies("1")
        self.__rental_repository.count_rentals_clients("1")
        self.__rental_repository.update(Rental("1", "1", "1", "1", date.today(), "1"))

        self.__rental_service.convert_to_date("2020-1-1")
        self.__rental_service.check_client_past_due("300")
        self.__rental_service.check_client_past_due("1")
        self.__rental_service.rent_movie("1", "1", "2020-1-1")
        self.__rental_service.check_rental_id("1")
        self.__rental_service.check_returned_date("1")
        self.__rental_service.return_movie("1")
        self.__rental_service.late_rentals()
        self.__rental_service.most_rented_movies()
        self.__rental_service.most_active_clients()



if __name__ == '__main__':
    unittest.main()
