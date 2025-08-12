from src.domain.RentalDomain import Rental
from datetime import datetime, date


class RentalService:
    def __init__(self, rental_repository):
        """
        Constructor for RentalService class
        :param rental_repository:
        """
        self.__rental_repository = rental_repository

    @staticmethod
    def check_valid_date(due_date):
        """
        Checks if a date is valid
        :param due_date:
        :return:
        """
        try:
            words = due_date.split("-")
            if len(words) != 3 or int(words[1]) > 12 or int(words[2]) > 31:
                raise ValueError("Invalid date format")
            datetime.strptime(due_date, "%Y-%m-%d")
            return True
        except ValueError:
            return False

    @staticmethod
    def convert_to_date(due_date):
        """
        Converts a string to a date
        :param due_date:
        :return:
        """
        return datetime.strptime(due_date, "%Y-%m-%d").date()

    def check_client_past_due(self, client_id):
        """
        Checks if a client has any past due rentals
        :param client_id:
        :return:
        """
        today = date.today()
        for rental in self.__rental_repository.get_all():
            cl_id = rental.client_id()
            if cl_id == client_id:
                due_date = rental.due_date()
                if due_date < today and rental.returned_date() == "0":
                    return True
        return False

    def rent_movie(self, client_id, movie_id, due_date):
        """
        Rents a movie
        :param client_id:
        :param movie_id:
        :param due_date:
        :return:
        """
        today = date.today()
        rental_id = self.__rental_repository.get_next_id()
        due_date = self.convert_to_date(due_date)

        rental = Rental(str(rental_id), str(client_id), str(movie_id), today, due_date, "0")
        self.__rental_repository.add(rental)

    def check_rental_id(self, rental_id):
        """
        Checks if a rental ID exists
        :param rental_id:
        :return:
        """
        for rental in self.__rental_repository.get_all():
            if rental.rental_id() == rental_id:
                return True
        return False

    def check_returned_date(self, rental_id):
        """
        Checks if a movie has been returned
        :param rental_id:
        :return:
        """
        for rental in self.__rental_repository.get_all():
            if rental.rental_id() == rental_id:
                if rental.returned_date() == "0":
                    return True
        return False

    def return_movie(self, rental_id):
        """
        Returns a movie
        :param rental_id:
        :return:
        """
        rental = self.__rental_repository.find_by_id(rental_id)
        rental.set_returned_date(date.today())
        self.__rental_repository.update(rental)

    def late_rentals(self):
        """
        Returns a list of late rentals
        :return:
        """
        late_rentals = []

        for rental in self.__rental_repository.get_all():
            if rental.returned_date() == "0":
                due_date = rental.due_date()
                if due_date < date.today():
                    days_late = (date.today() - due_date).days
                    late_rentals.append((rental, days_late))

        late_rentals.sort(key=lambda x: x[1])

        return late_rentals

    def most_rented_movies(self):
        """
        Returns a list of most rented movies
        :return:
        """
        rented_movies = []

        for rental in self.__rental_repository.get_all_movies():
            count = self.__rental_repository.count_rentals_movies(rental)
            if (rental, count) not in rented_movies:
                rented_movies.append((rental, count))

        rented_movies.sort(key=lambda x: x[1], reverse=True)

        return rented_movies

    def most_active_clients(self):
        """
        Returns a list of most active clients
        :return:
        """
        active_clients = []

        for rental in self.__rental_repository.get_all_clients():
            count = self.__rental_repository.count_rentals_clients(rental)
            if (rental, count) not in active_clients:
                active_clients.append((rental, count))

        active_clients.sort(key=lambda x: x[1], reverse=True)

        return active_clients
