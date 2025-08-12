from src.domain.MovieDomain import Movie
from src.exceptions.Exceptions import RepositoryException


class MovieRepo:
    def __init__(self):
        """
        Creates a new instance of MovieRepo
        """
        self.__movie_data = []
        for count in range(1, 21):
            self.__movie_data.append(Movie(str(count), "Movie" + str(count), "Description"
                                           + str(count), "Genre" + str(count)))

    def add_movie(self, movie):
        """
        Adds a movie to the repository
        :param movie: Movie
        :return: None
        """
        for movie_inlist in self.__movie_data:
            if movie_inlist.movie_id() == movie.movie_id():
                raise RepositoryException("Movie ID already exists!\n")
        self.__movie_data.append(movie)

    def remove_movie(self, movie_id):
        """
        Removes a movie from the repository
        :param movie_id: int
        :return: None
        """
        for movie in self.__movie_data:
            if movie.movie_id() == movie_id:
                self.__movie_data.remove(movie)

    def update_movie(self, movie_id, title, description, genre):
        """
        Updates a movie from the repository
        :param movie_id: int
        :param title: string
        :param description: string
        :param genre: string
        :return: None
        """
        for movie in self.__movie_data:
            if movie.movie_id() == movie_id:
                movie.set_movie_title(title)
                movie.set_movie_description(description)
                movie.set_movie_genre(genre)

    def list_movies(self):
        """
        Returns the list of movies
        :return: list
        """
        return self.__movie_data

    def check_movie_id(self, movie_id):
        """
        Checks if the movie ID exists
        :param movie_id: int
        :return: bool
        """
        for movie in self.__movie_data:
            if movie.movie_id() == movie_id:
                return True

    def search_movie_by_id(self, movie_id):
        """
        Searches for a movie by ID
        :param movie_id: int
        :return: Movie
        """
        movie_list = []
        for movie in self.__movie_data:
            if movie_id in movie.movie_id():
                movie_list.append(movie)

        return movie_list

    def search_movie_by_title(self, title):
        """
        Searches for a movie by title
        :param title: string
        :return: Movie
        """
        movie_list = []
        for movie in self.__movie_data:
            if title.lower() in movie.movie_title().lower():
                movie_list.append(movie)

        return movie_list

    def search_movie_by_description(self, description):
        """
        Searches for a movie by description
        :param description: string
        :return: Movie
        """
        movie_list = []
        for movie in self.__movie_data:
            if description.lower() in movie.movie_description().lower():
                movie_list.append(movie)

        return movie_list

    def search_movie_by_genre(self, genre):
        """
        Searches for a movie by genre
        :param genre: string
        :return: Movie
        """
        movie_list = []
        for movie in self.__movie_data:
            if genre.lower() in movie.movie_genre().lower():
                movie_list.append(movie)

        return movie_list

    def get_all_movies(self):
        return self.__movie_data