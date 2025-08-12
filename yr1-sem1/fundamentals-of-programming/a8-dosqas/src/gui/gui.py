import tkinter as tk
from tkinter import messagebox, simpledialog

from src.exceptions.Exceptions import ValidationException
from src.exceptions.Exceptions import RepositoryException


class GUI:
    def __init__(self, master, client_service, movie_service, rental_service):
        self.master = master
        self.__client_service = client_service
        self.__movie_service = movie_service
        self.___rental_service = rental_service
        master.title("Movie Rental System")

        # Create and place widgets
        self.menu_label = tk.Label(master, text="MENU")
        self.menu_label.pack()

        # Option 1: Manage Clients/Movies
        self.manage_clients_movies_button = tk.Button(master, text="1. Manage Clients/Movies",
                                                      command=self.manage_clients_movies)
        self.manage_clients_movies_button.pack()

        # Option 2: Rent/Return Movie
        self.rent_return_movie_button = tk.Button(master, text="2. Rent/Return Movie", command=self.rent_return_movie)
        self.rent_return_movie_button.pack()

        # Option 3: Search for Clients/Movies
        self.search_clients_movies_button = tk.Button(master, text="3. Search for Clients/Movies",
                                                      command=self.search_clients_movies)
        self.search_clients_movies_button.pack()

        # Option 4: Statistics
        self.statistics_button = tk.Button(master, text="4. Statistics", command=self.show_statistics)
        self.statistics_button.pack()

    def manage_clients_movies(self):
        # Create a submenu for managing clients or movies
        submenu = tk.Toplevel(self.master)
        submenu.title("Manage Clients/Movies")

        # Option 1.1: Manage Clients
        manage_clients_button = tk.Button(submenu, text="1.1 Manage Clients", command=self.manage_clients)
        manage_clients_button.pack()

        # Option 1.2: Manage Movies
        manage_movies_button = tk.Button(submenu, text="1.2 Manage Movies", command=self.manage_movies)
        manage_movies_button.pack()

    def manage_clients(self):
        # Create a submenu for managing clients
        submenu = tk.Toplevel(self.master)
        submenu.title("Manage Clients")

        # Option 1.1.1: Add Client
        add_client_button = tk.Button(submenu, text="1.1.1 Add Client", command=self.add_client)
        add_client_button.pack()

        # Option 1.1.2: Remove Client
        remove_client_button = tk.Button(submenu, text="1.1.2 Remove Client", command=self.remove_client)
        remove_client_button.pack()

        # Option 1.1.3: Update Client
        update_client_button = tk.Button(submenu, text="1.1.3 Update Client", command=self.update_client)
        update_client_button.pack()

        # Option 1.1.4: List Clients
        list_clients_button = tk.Button(submenu, text="1.1.4 List Clients", command=self.list_clients)
        list_clients_button.pack()

    def manage_movies(self):
        # Create a submenu for managing movies
        submenu = tk.Toplevel(self.master)
        submenu.title("Manage Movies")

        # Option 1.2.1: Add Movie
        add_movie_button = tk.Button(submenu, text="1.2.1 Add Movie", command=self.add_movie)
        add_movie_button.pack()

        # Option 1.2.2: Remove Movie
        remove_movie_button = tk.Button(submenu, text="1.2.2 Remove Movie", command=self.remove_movie)
        remove_movie_button.pack()

        # Option 1.2.3: Update Movie
        update_movie_button = tk.Button(submenu, text="1.2.3 Update Movie", command=self.update_movie)
        update_movie_button.pack()

        # Option 1.2.4: List Movies
        list_movies_button = tk.Button(submenu, text="1.2.4 List Movies", command=self.list_movies)
        list_movies_button.pack()

    def add_client(self):
        user_input = self.get_user_input("Enter client details as <id> <name>:")
        if user_input:
            try:
                user_input = user_input.split()
                if len(user_input) == 2:
                    if user_input[0].isdigit():
                        if user_input[1].isalpha():
                            if self.__client_service.check_client_id_unique(user_input[0]):
                                self.__client_service.add_client(user_input[0], user_input[1])
                            else:
                                raise RepositoryException("\nError: Client ID already exists!\n")
                        else:
                            raise ValidationException("\nError: Name is not valid!\n")
                    else:
                        raise ValidationException("\nError: ID has to be a positive integer!\n")
            except ValueError:
                messagebox.showerror("ValidationException",
                                     ve)
            except RepositoryException as re:
                print(re)

    def remove_client(self):
        client_id = self.get_user_input("Enter client ID to remove:")
        if client_id:
            # Perform logic for removing the client with the given ID
            messagebox.showinfo("Remove Client", f"Removed Client with ID: {client_id}")

    def update_client(self):
        client_id = self.get_user_input("Enter client ID to update:")
        if client_id:
            new_name = self.get_user_input("Enter new name for the client:")
            # Perform logic for updating the client with the given ID and new name
            messagebox.showinfo("Update Client", f"Updated Client with ID {client_id}. New Name: {new_name}")

    @staticmethod
    def list_clients():
        messagebox.showinfo("List Clients", "Functionality for listing clients.")

    def add_movie(self):
        user_input = self.get_user_input("Enter movie details (id title description genre):")
        if user_input:
            try:
                movie_id, title, description, genre = user_input.split()
                messagebox.showinfo("Add Movie",
                                    f"Added Movie - ID: {movie_id}, Title: {title}, Description: {description}, Genre: {genre}")
            except ValueError:
                messagebox.showerror("Error",
                                     "Invalid input format. Please enter id, title, description, and genre separated by spaces.")

    def remove_movie(self):
        movie_id = self.get_user_input("Enter movie ID to remove:")
        if movie_id:
            # Perform logic for removing the movie with the given ID
            messagebox.showinfo("Remove Movie", f"Removed Movie with ID: {movie_id}")

    def update_movie(self):
        movie_id = self.get_user_input("Enter movie ID to update:")
        if movie_id:
            new_title = self.get_user_input("Enter new title for the movie:")
            new_description = self.get_user_input("Enter new description for the movie:")
            new_genre = self.get_user_input("Enter new genre for the movie:")
            # Perform logic for updating the movie with the given ID and new details
            messagebox.showinfo("Update Movie",
                                f"Updated Movie with ID {movie_id}. New Title: {new_title}, New Description: {new_description}, New Genre: {new_genre}")

    @staticmethod
    def list_movies():
        messagebox.showinfo("List Movies", "Functionality for listing movies.")

    def rent_return_movie(self):
        # Create a submenu for renting/returning movies
        submenu = tk.Toplevel(self.master)
        submenu.title("Rent/Return Movie")

        # Option 2.1: Rent Movie
        rent_movie_button = tk.Button(submenu, text="2.1 Rent Movie", command=self.rent_movie)
        rent_movie_button.pack()

        # Option 2.2: Return Movie
        return_movie_button = tk.Button(submenu, text="2.2 Return Movie", command=self.return_movie)
        return_movie_button.pack()

    def rent_movie(self):
        messagebox.showinfo("Rent Movie", "Functionality for renting a movie.")

    def return_movie(self):
        messagebox.showinfo("Return Movie", "Functionality for returning a movie.")

    def search_clients_movies(self):
        # Create a submenu for searching clients/movies
        submenu = tk.Toplevel(self.master)
        submenu.title("Search for Clients/Movies")

        # Option 3.1: Search for Clients
        search_clients_button = tk.Button(submenu, text="3.1 Search for Clients", command=self.search_clients)
        search_clients_button.pack()

        # Option 3.2: Search for Movies
        search_movies_button = tk.Button(submenu, text="3.2 Search for Movies", command=self.search_movies)
        search_movies_button.pack()

    def search_clients(self):
        # Create a submenu for searching clients
        submenu = tk.Toplevel(self.master)
        submenu.title("Search for Clients")

        # Option 3.1.1: Search by ID
        search_by_id_button = tk.Button(submenu, text="3.1.1 Search by ID", command=self.search_clients_by_id)
        search_by_id_button.pack()

        # Option 3.1.2: Search by Name
        search_by_name_button = tk.Button(submenu, text="3.1.2 Search by Name", command=self.search_clients_by_name)
        search_by_name_button.pack()

    def search_movies(self):
        # Create a submenu for searching movies
        submenu = tk.Toplevel(self.master)
        submenu.title("Search for Movies")

        # Option 3.2.1: Search by ID
        search_by_id_button = tk.Button(submenu, text="3.2.1 Search by ID", command=self.search_movies_by_id)
        search_by_id_button.pack()

        # Option 3.2.2: Search by Title
        search_by_title_button = tk.Button(submenu, text="3.2.2 Search by Title", command=self.search_movies_by_title)
        search_by_title_button.pack()

        # Option 3.2.3: Search by Description
        search_by_description_button = tk.Button(submenu, text="3.2.3 Search by Description",
                                                 command=self.search_movies_by_description)
        search_by_description_button.pack()

        # Option 3.2.4: Search by Genre
        search_by_genre_button = tk.Button(submenu, text="3.2.4 Search by Genre", command=self.search_movies_by_genre)
        search_by_genre_button.pack()

    def search_clients_by_id(self):
        messagebox.showinfo("Search Clients", "Functionality for searching clients by ID.")

    def search_clients_by_name(self):
        messagebox.showinfo("Search Clients", "Functionality for searching clients by name.")

    def search_movies_by_id(self):
        messagebox.showinfo("Search Movies", "Functionality for searching movies by ID.")

    def search_movies_by_title(self):
        messagebox.showinfo("Search Movies", "Functionality for searching movies by title.")

    def search_movies_by_description(self):
        messagebox.showinfo("Search Movies", "Functionality for searching movies by description.")

    def search_movies_by_genre(self):
        messagebox.showinfo("Search Movies", "Functionality for searching movies by genre.")

    def show_statistics(self):
        # Create a submenu for displaying statistics
        submenu = tk.Toplevel(self.master)
        submenu.title("Statistics")

        # Option 4.1: Most Rented Movies
        most_rented_movies_button = tk.Button(submenu, text="4.1 Most Rented Movies", command=self.most_rented_movies)
        most_rented_movies_button.pack()

        # Option 4.2: Most Active Clients
        most_active_clients_button = tk.Button(submenu, text="4.2 Most Active Clients",
                                               command=self.most_active_clients)
        most_active_clients_button.pack()

        # Option 4.3: Late Rentals
        late_rentals_button = tk.Button(submenu, text="4.3 Late Rentals", command=self.late_rentals)
        late_rentals_button.pack()

    def most_rented_movies(self):
        messagebox.showinfo("Most Rented Movies", "Functionality for displaying most rented movies.")

    def most_active_clients(self):
        messagebox.showinfo("Most Active Clients", "Functionality for displaying most active clients.")

    def late_rentals(self):
        messagebox.showinfo("Late Rentals", "Functionality for displaying late rentals.")

    def get_user_input(self, prompt):
        return simpledialog.askstring("Input", prompt)


if __name__ == "__main__":
    root = tk.Tk()
    app = GUI(root)
    root.mainloop()
