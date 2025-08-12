from src.repository.RentalRepo import RentalRepo
from src.domain.RentalDomain import Rental


class RentalTextRepo(RentalRepo):
    def __init__(self, file):
        super().__init__()
        self._file = file
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

    def list_rentals(self):
        return super().list_rentals()

    def get_last_rental(self):
        return super().get_last_rental()

    def remove_rental(self, rental_id):
        return super().remove_rental(rental_id)

    def add_rentals(self, array):
        return super().add_rentals(array)

    def save_to_file(self, textfile):
        try:
            file = open(textfile, "w")
            for rental in self._rental_data:
                file.write(str(rental.rental_id()) + "," + str(rental.movie_id()) + "," +
                           str(rental.client_id()) + "," + str(rental.rented_date()) + "," +
                           str(rental.due_date()) + "," + str(rental.returned_date()) + "\n")
            file.close()
        except IOError:
            pass

    def load_from_file(self, textfile):
        try:
            temp_data = []
            file = open(textfile, "r")
            lines = file.readlines()
            for line in lines:
                line = line.strip()
                line = line.split(",")
                rental = Rental(line[0], line[1], line[2], line[3], line[4], line[5])
                temp_data.append(rental)
            file.close()
            self._rental_data = temp_data.copy()
            return temp_data
        except IOError:
            pass

# 1,1,20,2023-12-15,2024-07-30,0
# 2,2,12,2023-12-15,2023-02-25,0
# 3,3,20,2023-12-15,2024-06-04,0
# 4,4,10,2023-12-15,2023-06-03,0
# 5,5,4,2023-12-15,2023-09-02,0
# 6,6,13,2023-12-15,2023-07-30,0
# 7,7,16,2023-12-15,2023-08-16,0
# 8,8,20,2023-12-15,2024-05-17,0
# 9,9,15,2023-12-15,2024-04-24,0
# 10,10,18,2023-12-15,2024-06-03,0
# 11,11,19,2023-12-15,2023-08-09,0
# 12,12,1,2023-12-15,2023-03-18,0
# 13,13,8,2023-12-15,2024-04-25,0
# 14,14,7,2023-12-15,2023-10-26,0
# 15,15,9,2023-12-15,2023-12-02,0
# 16,16,5,2023-12-15,2024-04-19,0
# 17,17,14,2023-12-15,2024-11-06,0
# 18,18,15,2023-12-15,2023-07-24,0
# 19,19,10,2023-12-15,2023-01-01,0
# 20,20,4,2023-12-15,2024-11-05,0
