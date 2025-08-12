from src.exceptions.Exceptions import ValidationException
from src.exceptions.Exceptions import RepositoryException
from src.exceptions.Exceptions import UndoRedoException


class UI:
    def __init__(self, client_service, movie_service, rental_service, undo_service):
        """
        Initializes the UI class.
        :param client_service:
        :param movie_service:
        :param rental_service:
        """
        self.__client_service = client_service
        self.__movie_service = movie_service
        self.__rental_service = rental_service
        self.__undo_service = undo_service

    def input(self):
        """
        Reads the input from the user.
        :return:
        """
        while True:
            print("Hello! Welcome to the movie rental store!\n"
                  "Please choose one of the following options:\n"
                  "[1] Manage clients or movies (add/remove/update/list)\n"
                  "[2] Rent or return a movie\n"
                  "[3] Search (clients/movies)\n"
                  "[4] Statistics\n"
                  "[5] Undo/Redo\n"
                  "[0] Exit\n")
            try:
                command = input("Enter command: ").strip()
                if command == '0':
                    print("Thank you for using our services!")
                    quit()
                elif command == '1':
                    self.manage()
                elif command == '2':
                    self.rent_or_return_movie()
                elif command == '3':
                    self.search()
                elif command == '4':
                    self.statistics()
                elif command == '5':
                    self.undo_menu()
                else:
                    raise ValidationException("Invalid command!\n")
            except ValidationException as ve:
                print(ve)

    # 1
    def manage(self):
        """
        Menu for the first option.
        :return:
        """
        while True:
            print("What would you like to manage?\n"
                  "[1] Clients\n"
                  "[2] Movies\n"
                  "[3] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.manage_clients()
                elif command == '2':
                    self.manage_movies()
                elif command == '3':
                    break
                else:
                    raise ValidationException("Invalid command!\n")
            except ValidationException as ve:
                print(ve)

    # 1.1
    def manage_clients(self):
        """
        Menu for managing clients.
        :return:
        """
        while True:
            print("What would you like to do?\n"
                  "[1] Add a client\n"
                  "[2] Remove a client\n"
                  "[3] Update a client\n"
                  "[4] List clients\n"
                  "[5] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.add_client()
                elif command == '2':
                    self.remove_client()
                elif command == '3':
                    self.update_client()
                elif command == '4':
                    self.list_clients()
                elif command == '5':
                    break
                else:
                    raise ValidationException("Invalid command!\n")
            except ValidationException as ve:
                print(ve)

    # 1.1.1
    def add_client(self):
        """
        UI part of adding a client.
        :return:
        """
        try:
            print("Please enter the following information using syntax:\n"
                  "<ID> <Name>\n")
            syntax = input("Enter information: ")
            syntax = syntax.split(" ")
            if len(syntax) == 2:
                if syntax[0].isdigit():
                    if syntax[1].isalpha():
                        if self.__client_service.check_client_id_unique(syntax[0]):
                            self.__client_service.add_client(syntax[0], syntax[1])
                            self.__undo_service.client_adding_operation(syntax[0], syntax[1])
                        else:
                            raise RepositoryException("\nError: Client ID already exists!\n")
                    else:
                        raise ValidationException("\nError: Name is not valid!\n")
                else:
                    raise ValidationException("\nError: ID has to be a positive integer!\n")
            else:
                raise ValidationException("\nError: Invalid syntax!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Client added successfully!\n")

    # 1.1.2
    def remove_client(self):
        """
        UI part of removing a client.
        :return:
        """
        try:
            print("Please enter the ID of the client you want to remove.\n")
            client_id = input("Enter ID: ").strip()
            if client_id.isdigit():
                if self.__client_service.can_remove_client(client_id):
                    test = self.__client_service.get_client_name(client_id)
                    array = self.__client_service.remove_client(client_id)
                    if array is not None:
                        self.__undo_service.client_removing_operation(client_id,
                                                                      test,
                                                                      array)
                else:
                    raise RepositoryException("\nError: No client has the requested ID!\n")
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Client removed successfully!\n")

    # 1.1.3
    def update_client(self):
        """
        UI part of updating a client.
        :return:
        """
        try:
            print("Please enter the ID of the client you wish to update.\n")
            client_id = input("Enter ID: ").strip()
            if client_id.isdigit():
                if self.__client_service.can_update_client(client_id):
                    print("Please update the name of the client that has the requested ID.\n")
                    client_name = input("Enter name: ").strip()
                    if client_name.isalpha():
                        self.__undo_service.client_updating_operation(client_id, client_name,
                                                                      self.__client_service.get_client_name(client_id))
                        self.__client_service.update_client(client_id, client_name)
                    else:
                        raise ValidationException("\nError: Name is not valid!\n")
                else:
                    raise RepositoryException("\nError: No client has the requested ID!\n")
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Client updated successfully!\n")

    # 1.1.4
    def list_clients(self):
        """
        UI part of listing clients.
        :return:
        """
        print("Here is the list of clients:\n")
        for client in self.__client_service.list_clients():
            print(str(client))
        if len(self.__client_service.list_clients()) == 0:
            print("No clients to show!\n")
        print("\n")

    # 1.2
    def manage_movies(self):
        """
        Menu for managing movies.
        :return:
        """
        while True:
            print("What would you like to do?\n"
                  "[1] Add a movie\n"
                  "[2] Remove a movie\n"
                  "[3] Update a movie\n"
                  "[4] List movies\n"
                  "[5] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.add_movie()
                elif command == '2':
                    self.remove_movie()
                elif command == '3':
                    self.update_movie()
                elif command == '4':
                    self.list_movies()
                elif command == '5':
                    break
                else:
                    print("Error: Invalid command!")
            except ValidationException:
                print("Error: Invalid command!")

    # 1.2.1
    def add_movie(self):
        """
        UI part of adding a movie.
        :return:
        """
        try:
            print("Please enter the following information using syntax:\n"
                  "<ID> <Title> <Description> <Genre>\n")
            syntax = input("Enter information: ")
            syntax = syntax.split(" ")
            if len(syntax) == 4:
                if syntax[0].isdigit():
                    if syntax[3].isalpha():
                        if self.__movie_service.check_movie_id_unique(syntax[0]):
                            self.__movie_service.add_movie(syntax[0], syntax[1], syntax[2], syntax[3])
                            self.__undo_service.movie_adding_operation(syntax[0], syntax[1], syntax[2], syntax[3])
                        else:
                            raise RepositoryException("\nError: Movie ID already exists!\n")
                    else:
                        raise ValidationException("\nError: Genre is not valid!\n")
                else:
                    raise ValidationException("\nError: ID has to be a positive integer!\n")
            else:
                raise ValidationException("\nError: Invalid syntax!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Movie added successfully!\n")

    # 1.2.2
    def remove_movie(self):
        """
        UI part of removing a movie.
        :return:
        """
        try:
            print("Please enter the ID of the movie you want to remove:\n")
            movie_id = input("Enter ID: ").strip()
            if movie_id.isdigit():
                if not self.__movie_service.can_remove_movie(movie_id):
                    raise RepositoryException("\nError: No movie has the requested id!\n")
                else:
                    temp1 = self.__movie_service.get_movie_title(movie_id)
                    temp2 = self.__movie_service.get_movie_description(movie_id)
                    temp3 = self.__movie_service.get_movie_genre(movie_id)
                    array = self.__movie_service.remove_movie(movie_id)
                    if array is not None:
                        self.__undo_service.movie_removing_operation(movie_id, temp1, temp2, temp3, array)
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Movie removed successfully!\n")

    # 1.2.3
    def update_movie(self):
        """
        UI part of updating a movie.
        :return:
        """
        try:
            print("Please enter the ID of the movie you wish to update.\n")
            movie_id = input("Enter ID: ").strip()
            if movie_id.isdigit():
                if self.__movie_service.can_update_movie(movie_id):
                    print("Please update the title, description and genre of the movie that has the requested ID.\n")
                    syntax = input("Enter information: ")
                    syntax = syntax.split(" ")
                    if len(syntax) == 3:
                        if syntax[2].isalpha():
                            self.__undo_service.movie_updating_operation(movie_id, syntax[0], syntax[1], syntax[2],
                                                                         self.__movie_service.get_movie_title(movie_id),
                                                                         self.__movie_service.get_movie_description(
                                                                             movie_id),
                                                                         self.__movie_service.get_movie_genre(movie_id))
                            self.__movie_service.update_movie(movie_id, syntax[0], syntax[1], syntax[2])
                        else:
                            raise ValidationException("\nError: Genre is not valid!\n")
                    else:
                        raise ValidationException("\nError: Invalid syntax!\n")
                else:
                    raise RepositoryException("\nError: No movie has the requested ID!\n")
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Movie updated successfully!\n")

    # 1.2.4
    def list_movies(self):
        """
        UI part of listing movies.
        :return:
        """
        print("Here is the list of movies:\n")
        for movie in self.__movie_service.list_movies():
            print(str(movie))
        if len(self.__movie_service.list_movies()) == 0:
            print("No movies to show!\n")
        print("\n")

    # 2
    def rent_or_return_movie(self):
        """
        Menu for renting or returning a movie.
        :return:
        """
        while True:
            print("What would you like to do?\n"
                  "[1] Rent a movie\n"
                  "[2] Return a movie\n"
                  "[3] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.rent_movie()
                elif command == '2':
                    self.return_movie()
                elif command == '3':
                    break
                else:
                    print("Error: Invalid command!")
            except ValidationException:
                print("Error: Invalid command!")

    # 2.1
    def rent_movie(self):
        """
        UI part of renting a movie.
        :return:
        """
        try:
            print("Please enter the following information using syntax:\n"
                  "<Client ID> <Movie ID> <Due date (year-month-day)>\n")
            syntax = input("Enter information: ")
            syntax = syntax.split(" ")
            if len(syntax) == 3:
                if syntax[0].isdigit():
                    if syntax[1].isdigit():
                        if self.__client_service.check_client_id(syntax[0]):
                            if self.__movie_service.check_movie_id(syntax[1]):
                                if self.__rental_service.check_valid_date(syntax[2]):
                                    if not self.__rental_service.check_client_past_due(syntax[0]):
                                        self.__rental_service.rent_movie(syntax[0], syntax[1], syntax[2])
                                        last_rental = self.__rental_service.get_last_rental()
                                        self.__undo_service.rental_adding_operation(last_rental.rental_id(),
                                                                                    last_rental.movie_id(),
                                                                                    last_rental.client_id(),
                                                                                    last_rental.rented_date(),
                                                                                    last_rental.due_date(),
                                                                                    last_rental.returned_date())
                                    else:
                                        raise RepositoryException("\nClient has a past due rental!\n")
                                else:
                                    raise ValidationException("\nError: Invalid date!\n")
                            else:
                                raise RepositoryException("\nError: Movie ID does not exist!\n")
                        else:
                            raise RepositoryException("\nError: Client ID does not exist!\n")
                    else:
                        raise ValidationException("\nError: Movie ID has to be a positive integer!\n")
                else:
                    raise ValidationException("\nError: Client ID has to be a positive integer!\n")
            else:
                raise ValidationException("\nError: Invalid syntax!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Movie rented successfully!\n")

    # 2.2
    def return_movie(self):
        """
        UI part of returning a movie.
        :return:
        """
        try:
            print("Please enter the rental ID of the movie you want to return:\n")
            syntax = input("Enter ID: ").strip()
            if syntax.isdigit():
                if self.__rental_service.check_rental_id(syntax):
                    if self.__rental_service.check_returned_date(syntax):
                        needed_rental = self.__rental_service.get_rental(syntax)
                        self.__undo_service.rental_removing_operation(needed_rental.rental_id(),
                                                                      needed_rental.movie_id(),
                                                                      needed_rental.client_id(),
                                                                      needed_rental.rented_date(),
                                                                      needed_rental.due_date(),
                                                                      needed_rental.returned_date())
                        self.__rental_service.return_movie(syntax)
                    else:
                        raise ValidationException("\nError: Movie has already been returned!\n")
                else:
                    raise RepositoryException("\nError: Rental ID does not exist!\n")
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)
        except RepositoryException as re:
            print(re)
        else:
            print("Movie returned successfully!\n")

    # 3
    def search(self):
        """
        Menu for searching.
        :return:
        """
        while True:
            print("What would you like to search?\n"
                  "[1] Clients\n"
                  "[2] Movies\n"
                  "[3] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.search_clients()
                elif command == '2':
                    self.search_movies()
                elif command == '3':
                    break
                else:
                    print("Error: Invalid command!")
            except ValidationException:
                print("Error: Invalid command!")

    # 3.1
    def search_clients(self):
        """
        Menu for searching clients.
        :return:
        """
        while True:
            print("What would you like to search by?\n"
                  "[1] ID\n"
                  "[2] Name\n"
                  "[3] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.search_clients_by_id()
                elif command == '2':
                    self.search_clients_by_name()
                elif command == '3':
                    break
                else:
                    print("Error: Invalid command!")
            except ValidationException:
                print("Error: Invalid command!")

    # 3.1.1
    def search_clients_by_id(self):
        """
        UI part of searching clients by ID.
        :return:
        """
        try:
            print("Please enter the ID of the client you want to search for:\n")
            client_id = input("Enter ID: ").strip()
            if client_id.isdigit():
                if len(self.__client_service.search_client_by_id(client_id)) == 0:
                    print("No clients to show!\n")
                else:
                    print("Here is the list of clients:\n")
                    for client in self.__client_service.search_client_by_id(client_id):
                        print(str(client))
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)

    # 3.1.2
    def search_clients_by_name(self):
        """
        UI part of searching clients by name.
        :return:
        """
        print("Please enter the name of the client you want to search for:\n")
        client_name = input("Enter name: ")
        if len(self.__client_service.search_client_by_name(client_name)) == 0:
            print("No clients to show!\n")
        else:
            print("Here is the list of clients:\n")
            for client in self.__client_service.search_client_by_name(client_name):
                print(str(client))

    # 3.2
    def search_movies(self):
        """
        Menu for searching movies.
        :return:
        """
        while True:
            print("What would you like to search by?\n"
                  "[1] ID\n"
                  "[2] Title\n"
                  "[3] Description\n"
                  "[4] Genre\n"
                  "[5] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.search_movies_by_id()
                elif command == '2':
                    self.search_movies_by_title()
                elif command == '3':
                    self.search_movies_by_description()
                elif command == '4':
                    self.search_movies_by_genre()
                elif command == '5':
                    break
                else:
                    print("Error: Invalid command!")
            except ValidationException:
                print("Error: Invalid command!")

    # 3.2.1
    def search_movies_by_id(self):
        """
        UI part of searching movies by ID.
        :return:
        """
        try:
            print("Please enter the ID of the movie you want to search for:\n")
            movie_id = input("Enter ID: ").strip()
            if movie_id.isdigit():
                if len(self.__movie_service.search_movie_by_id(movie_id)) == 0:
                    print("No movies to show!\n")
                else:
                    print("Here is the list of movies:\n")
                    for movie in self.__movie_service.search_movie_by_id(movie_id):
                        print(str(movie))
            else:
                raise ValidationException("\nError: ID has to be a positive integer!\n")
        except ValidationException as ve:
            print(ve)

    # 3.2.2
    def search_movies_by_title(self):
        """
        UI part of searching movies by title.
        :return:
        """
        print("Please enter the title of the movie you want to search for:\n")
        movie_title = input("Enter title: ")
        if len(self.__movie_service.search_movie_by_title(movie_title)) == 0:
            print("No movies to show!\n")
        else:
            print("Here is the list of movies:\n")
            for movie in self.__movie_service.search_movie_by_title(movie_title):
                print(str(movie))

    # 3.2.3
    def search_movies_by_description(self):
        """
        UI part of searching movies by description.
        :return:
        """
        print("Please enter the description of the movie you want to search for:\n")
        movie_description = input("Enter description: ")
        if len(self.__movie_service.search_movie_by_description(movie_description)) == 0:
            print("No movies to show!\n")
        else:
            print("Here is the list of movies:\n")
            for movie in self.__movie_service.search_movie_by_description(movie_description):
                print(str(movie))

    # 3.2.4
    def search_movies_by_genre(self):
        """
        UI part of searching movies by genre.
        :return:
        """
        print("Please enter the genre of the movie you want to search for:\n")
        movie_genre = input("Enter genre: ")
        if len(self.__movie_service.search_movie_by_genre(movie_genre)) == 0:
            print("No movies to show!\n")
        else:
            print("Here is the list of movies:\n")
            for movie in self.__movie_service.search_movie_by_genre(movie_genre):
                print(str(movie))

    # 4
    def statistics(self):
        """
        Menu for statistics.
        :return:
        """
        while True:
            print("What would you like to see?\n"
                  "[1] Most rented movies\n"
                  "[2] Most active clients\n"
                  "[3] Late rentals\n"
                  "[4] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.most_rented_movies()
                elif command == '2':
                    self.most_active_clients()
                elif command == '3':
                    self.late_rentals()
                elif command == '4':
                    break
                else:
                    print("Error: Invalid command!")
            except ValidationException:
                print("Error: Invalid command!")

    # 4.1
    def most_rented_movies(self):
        """
        UI part of most rented movies.
        :return:
        """
        print("Here is the list of most rented movies:\n")
        for movie, count in self.__rental_service.most_rented_movies():
            print(movie.title() + " - " + str(count) + " rentals")
        print("\n")

    # 4.2
    def most_active_clients(self):
        """
        UI part of most active clients.
        :return:
        """
        print("Here is the list of most active clients:\n")
        for client, count in self.__rental_service.most_active_clients():
            print(client + " - " + str(count) + " rentals")
        print("\n")

    # 4.3
    def late_rentals(self):
        """
        UI part of late rentals.
        :return:
        """
        print("Here is the list of late rentals:\n")
        for rental, days_late in self.__rental_service.late_rentals():
            print(rental.client_id() + " " + rental.rental_id() + " - " + str(days_late) + " days late")
        if len(self.__rental_service.late_rentals()) == 0:
            print("No late rentals to show!\n")
        print("\n")

    # 5
    def undo_menu(self):
        while True:
            print("What operation would you like to perform?\n"
                  "[1] Undo\n"
                  "[2] Redo\n"
                  "[3] Back\n")
            try:
                command = input("Enter command: ").strip()
                if command == '1':
                    self.undo()
                elif command == '2':
                    self.redo()
                elif command == '3':
                    break
                else:
                    raise ValidationException("Error: Invalid command!")
            except ValidationException as ve:
                print(ve)

    def undo(self):
        try:
            self.__undo_service.undo()
        except UndoRedoException as ure:
            print(ure)
        else:
            print("Operation undone successfully!\n")

    def redo(self):
        try:
            self.__undo_service.redo()
        except UndoRedoException as ure:
            print(ure)
        else:
            print("Operation redone successfully!\n")
