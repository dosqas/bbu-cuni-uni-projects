from src.domain.RentalDomain import Rental
from datetime import date, timedelta
import random


class RentalRepo:
    def __init__(self):
        """
        Creates a new instance of RentalRepo
        """
        self._rental_data = []
        today = date.today()

        for counter in range(1, 21):
            days_in_future = random.randint(-365, 365)
            future_date = today + timedelta(days=days_in_future)
            self._rental_data.append(Rental(str(counter), str(counter),
                                             str(random.randint(1, 20)), today, future_date, "0"))

    def add(self, rental):
        """
        Adds a new rental to the repository
        :param rental:
        :return:
        """
        self._rental_data.append(rental)

    def remove(self, rental_id):
        """
        Removes a rental from the repository
        :param rental_id:
        :return:
        """
        for rental in self._rental_data:
            if rental.rental_id() == rental_id:
                self._rental_data.remove(rental)

    def get_next_id(self):
        """
        Returns the next id for a rental
        :return:
        """
        return len(self._rental_data) + 1

    def get_all(self):
        """
        Returns all the rentals
        :return:
        """
        return self._rental_data

    def get_all_movies(self):
        """
        Returns all the movies
        :return:
        """
        movies = []
        for rental in self._rental_data:
            movies.append(rental.movie_id())
        return movies

    def get_all_clients(self):
        """
        Returns all the clients
        :return:
        """
        clients = []
        for rental in self._rental_data:
            clients.append(rental.client_id())
        return clients

    def find_by_id(self, rental_id):
        """
        Finds a rental by id
        :param rental_id:
        :return:
        """
        for rental in self._rental_data:
            if rental.rental_id() == rental_id:
                return rental

    def count_rentals_movies(self, movie_id):
        """
        Counts the number of rentals for a movie
        :param movie_id:
        :return:
        """
        counter = 0
        for rental in self._rental_data:
            if rental.movie_id() == movie_id:
                counter += 1
        return counter

    def count_rentals_clients(self, client_id):
        """
        Counts the number of rentals for a client
        :param client_id:
        :return:
        """
        counter = 0
        for rental in self._rental_data:
            if rental.client_id() == client_id:
                counter += 1
        return counter

    def update(self, rental):
        """
        Updates a rental
        :param rental:
        :return:
        """
        for index in range(len(self._rental_data)):
            if self._rental_data[index].rental_id() == rental.rental_id():
                self._rental_data[index] = rental
                break

    def list_rentals(self):
        """
        Returns the list of rentals
        :return:
        """
        return self._rental_data

    def get_last_rental(self):
        """
        Returns the last rental
        :return:
        """
        return self._rental_data[-1]

    def remove_rental(self, rental_id):
        """
        Removes a rental
        :param rental_id:
        :return:
        """
        for rental in self._rental_data:
            if rental.rental_id() == rental_id:
                self._rental_data.remove(rental)

    def add_rentals(self, array):
        for rental in array:
            self._rental_data.append(rental)
