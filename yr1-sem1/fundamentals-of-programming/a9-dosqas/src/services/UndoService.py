from src.domain.ClientDomain import Client
from src.domain.MovieDomain import Movie
from src.domain.RentalDomain import Rental
from src.exceptions.Exceptions import UndoRedoException


class UndoService:
    def __init__(self, client_service, movie_service, rental_service):
        self._redo_operations = []
        self._undo_operations = []
        self._index = -1

        self._client_service = client_service
        self._movie_service = movie_service
        self._rental_service = rental_service

    def client_adding_operation(self, client_id, client_name):
        client = Client(client_id, client_name)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["client", "add", client])
        self._undo_operations.append(["client", "remove", client_id])
        self._index += 1

    def client_removing_operation(self, client_id, client_name, array):
        client = Client(client_id, client_name)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["client", "remove", client_id])
        self._undo_operations.append(["client", "add", client, array])
        self._index += 1

    def client_updating_operation(self, client_id, client_name, old_client_name):
        client = Client(client_id, client_name)
        old_client = Client(client_id, old_client_name)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["client", "update", client])
        self._undo_operations.append(["client", "update", old_client])
        self._index += 1

    def movie_adding_operation(self, movie_id, movie_title, movie_description, movie_genre):
        movie = Movie(movie_id, movie_title, movie_description, movie_genre)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["movie", "add", movie])
        self._undo_operations.append(["movie", "remove", movie_id])
        self._index += 1

    def movie_removing_operation(self, movie_id, movie_title, movie_description, movie_genre, array):
        movie = Movie(movie_id, movie_title, movie_description, movie_genre)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["movie", "remove", movie_id])
        self._undo_operations.append(["movie", "add", movie, array])
        self._index += 1

    def movie_updating_operation(self, movie_id, movie_title, movie_description, movie_genre,
                                 old_movie_title, old_movie_description, old_movie_genre):
        movie = Movie(movie_id, movie_title, movie_description, movie_genre)
        old_movie = Movie(movie_id, old_movie_title, old_movie_description, old_movie_genre)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["movie", "update", movie])
        self._undo_operations.append(["movie", "update", old_movie])
        self._index += 1

    def rental_adding_operation(self, rental_id, movie_id, client_id, rented_date, due_date, returned_date):
        rental = Rental(rental_id, movie_id, client_id, rented_date, due_date, returned_date)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["rental", "add", rental])
        self._undo_operations.append(["rental", "remove", rental_id])
        self._index += 1

    def rental_removing_operation(self, rental_id, movie_id, client_id,
                                  rented_date, due_date, returned_date):
        rental = Rental(rental_id, movie_id, client_id, rented_date, due_date, returned_date)

        self._redo_operations = self._redo_operations[:self._index + 1]
        self._undo_operations = self._undo_operations[:self._index + 1]

        self._redo_operations.append(["rental", "remove", rental_id])
        self._undo_operations.append(["rental", "add", rental])
        self._index += 1

    def redo(self):
        if self._index == len(self._redo_operations) - 1:
            raise UndoRedoException("No more operations to redo!\n")

        self._index += 1

        operation = self._redo_operations[self._index]
        if operation[0] == "client":
            if operation[1] == "add":
                self._client_service.add_client(operation[2].client_id(), operation[2].client_name())
            elif operation[1] == "remove":
                self._client_service.remove_client(operation[2])
            elif operation[1] == "update":
                self._client_service.update_client(operation[2].client_id(), operation[2].client_name())
        elif operation[0] == "movie":
            if operation[1] == "add":
                self._movie_service.add_movie(operation[2].movie_id(), operation[2].movie_title(),
                                              operation[2].movie_description(), operation[2].movie_genre())
            elif operation[1] == "remove":
                self._movie_service.remove_movie(operation[2])
            elif operation[1] == "update":
                self._movie_service.update_movie(operation[2].movie_id(), operation[2].movie_title(),
                                                 operation[2].movie_description(), operation[2].movie_genre())
        elif operation[0] == "rental":
            if operation[1] == "add":
                rental = [Rental(operation[2].rental_id(), operation[2].movie_id(), operation[2].client_id(),
                                 operation[2].rented_date(), operation[2].due_date(), operation[2].returned_date())]
                self._rental_service.add_rentals(rental)
            elif operation[1] == "remove":
                self._rental_service.remove_rental(operation[2])

    def undo(self):
        if self._index == -1:
            raise UndoRedoException("No more operations to undo!\n")
        operation = self._undo_operations[self._index]
        if operation[0] == "client":
            if operation[1] == "add":
                self._client_service.add_client(operation[2].client_id(), operation[2].client_name())
                self._rental_service.add_rentals(operation[3])
            elif operation[1] == "remove":
                test = operation[2]
                self._client_service.remove_client(operation[2])
            elif operation[1] == "update":
                self._client_service.update_client(operation[2].client_id(), operation[2].client_name())
        elif operation[0] == "movie":
            if operation[1] == "add":
                self._movie_service.add_movie(operation[2].movie_id(), operation[2].movie_title(),
                                              operation[2].movie_description(), operation[2].movie_genre())
                self._rental_service.add_rentals(operation[3])
            elif operation[1] == "remove":
                self._movie_service.remove_movie(operation[2])
            elif operation[1] == "update":
                self._movie_service.update_movie(operation[2].movie_id(), operation[2].movie_title(),
                                                 operation[2].movie_description(), operation[2].movie_genre())
        elif operation[0] == "rental":
            if operation[1] == "add":
                rental = [Rental(operation[2].rental_id(), operation[2].movie_id(), operation[2].client_id(),
                                 operation[2].rented_date(), operation[2].due_date(), operation[2].returned_date())]
                self._rental_service.add_rentals(rental)
            elif operation[1] == "remove":
                self._rental_service.remove_rental(operation[2])

        self._index -= 1
