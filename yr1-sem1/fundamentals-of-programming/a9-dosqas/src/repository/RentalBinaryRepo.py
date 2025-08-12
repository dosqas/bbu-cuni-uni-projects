from src.repository.RentalRepo import RentalRepo
from src.domain.RentalDomain import Rental
import pickle
from datetime import date, timedelta
import random


class RentalBinaryRepo(RentalRepo):
    def __init__(self, file):
        super().__init__()
        self._file = file
        # self._rental_data = []
        # today = date.today()
        # for counter in range(1, 21):
        #     days_in_future = random.randint(-365, 365)
        #     future_date = today + timedelta(days=days_in_future)
        #     self._rental_data.append(Rental(str(counter), str(counter),
        #                                      str(random.randint(1, 20)), today, future_date, "0"))
        #     self.save_to_file(self._file)

        self._rental_data = self.load_from_file(self._file)

    def add(self, rental):
        super().add(rental)
        self.save_to_file(self._file)

    def remove(self, rental_id):
        super().remove(rental_id)
        self.save_to_file(self._file)

    def get_next_id(self):
        return super().get_next_id()

    def get_all(self):
        return super().get_all()

    def get_all_movies(self):
        return super().get_all_movies()

    def get_all_clients(self):
        return super().get_all_clients()

    def find_by_id(self, rental_id):
        return super().find_by_id(rental_id)

    def count_rentals_clients(self, client_id):
        return super().count_rentals_clients(client_id)

    def count_rentals_movies(self, movie_id):
        return super().count_rentals_movies(movie_id)

    def update(self, rental):
        super().update(rental)
        self.save_to_file(self._file)

    def save_to_file(self, pickle_file):
        try:
            file = open(pickle_file, "wb")
            for rental in self._rental_data:
                pickle.dump(rental, file)
            file.close()
        except IOError:
            pass

    def list_rentals(self):
        return super().list_rentals()

    def get_last_rental(self):
        return super().get_last_rental()

    def remove_rental(self, rental_id):
        return super().remove_rental(rental_id)

    def add_rentals(self, array):
        return super().add_rentals(array)

    @staticmethod
    def load_from_file(pickle_file):
        file = open(pickle_file, "rb")
        temp_data = []
        while True:
            try:
                rental = pickle.load(file)
                temp_data.append(rental)
            except EOFError:
                break
        file.close()
        return temp_data
