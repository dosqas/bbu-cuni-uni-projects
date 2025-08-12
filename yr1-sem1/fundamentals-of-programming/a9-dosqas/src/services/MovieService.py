from src.domain.MovieDomain import Movie


class MovieService:
    def __init__(self, movie_repository):
        """
        Constructor for MovieService class
        :param movie_repository:
        """
        self.__movie_repository = movie_repository

    def add_movie(self, movie_id, title, description, genre):
        """
        Adds a movie to the repository
        :param movie_id:
        :param title:
        :param description:
        :param genre:
        :return:
        """
        movie = Movie(movie_id, title, description, genre)
        self.__movie_repository.add_movie(movie)

    def check_movie_id_unique(self, movie_id):
        """
        Checks if a movie ID is unique
        :param movie_id:
        :return:
        """
        for movie in self.__movie_repository.get_all_movies():
            if movie.movie_id() == movie_id:
                return False
        return True

    def remove_movie(self, movie_id):
        """
        Removes a movie from the repository
        :param movie_id:
        :return:
        """
        return self.__movie_repository.remove_movie(movie_id)

    def can_remove_movie(self, movie_id):
        """
        Checks if a movie can be removed from the repository
        :param movie_id:
        :return:
        """
        for movie in self.__movie_repository.get_all_movies():
            if movie.movie_id() == movie_id:
                return True
        return False

    def update_movie(self, movie_id, title, description, genre):
        """
        Updates a movie from the repository
        :param movie_id:
        :param title:
        :param description:
        :param genre:
        :return:
        """
        self.__movie_repository.update_movie(movie_id, title, description, genre)

    def can_update_movie(self, movie_id):
        """
        Checks if a movie can be updated from the repository
        :param movie_id:
        :return:
        """
        for movie in self.__movie_repository.get_all_movies():
            if movie.movie_id() == movie_id:
                return True
        return False

    def list_movies(self):
        """
        Returns the list of movies
        :return:
        """
        return self.__movie_repository.list_movies()

    def check_movie_id(self, movie_id):
        """
        Checks if a movie ID exists
        :param movie_id:
        :return:
        """
        return self.__movie_repository.check_movie_id(movie_id)

    def search_movie_by_id(self, movie_id):
        """
        Searches for a movie by ID
        :param movie_id:
        :return:
        """
        return self.__movie_repository.search_movie_by_id(movie_id)

    def search_movie_by_title(self, title):
        """
        Searches for a movie by title
        :param title:
        :return:
        """
        return self.__movie_repository.search_movie_by_title(title)

    def search_movie_by_description(self, description):
        """
        Searches for a movie by description
        :param description:
        :return:
        """
        return self.__movie_repository.search_movie_by_description(description)

    def search_movie_by_genre(self, genre):
        """
        Searches for a movie by genre
        :param genre:
        :return:
        """
        return self.__movie_repository.search_movie_by_genre(genre)

    def get_movie_title(self, movie_id):
        """
        Gets the movie title
        :param movie_id:
        :return:
        """
        return self.__movie_repository.get_movie_title(movie_id)

    def get_movie_description(self, movie_id):
        """
        Gets the movie description
        :param movie_id:
        :return:
        """
        return self.__movie_repository.get_movie_description(movie_id)

    def get_movie_genre(self, movie_id):
        """
        Gets the movie genre
        :param movie_id:
        :return:
        """
        return self.__movie_repository.get_movie_genre(movie_id)
