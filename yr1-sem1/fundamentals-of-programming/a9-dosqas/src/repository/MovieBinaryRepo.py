from src.repository.MovieRepo import MovieRepo
from src.domain.MovieDomain import Movie
import pickle


class MovieBinaryRepo(MovieRepo):
    def __init__(self, file, rental_repo):
        super().__init__(rental_repo)
        self._file = file
        # self._movie_data = []
        # for cnt in range(1, 21):
        #     self._movie_data.append(Movie(str(cnt), "Movie" +
        #                             str(cnt), "Description" + str(cnt), "Genre" + str(cnt)))
        #     self.save_to_file(self._file)

        self._movie_data = self.load_from_file(self._file)

    def add_movie(self, movie):
        super().add_movie(movie)
        self.save_to_file(self._file)

    def remove_movie(self, movie_id):
        result = super().remove_movie(movie_id)
        self.save_to_file(self._file)
        return result

    def update_movie(self, movie_id, title, description, genre):
        super().update_movie(movie_id, title, description, genre)
        self.save_to_file(self._file)

    def list_movies(self):
        return super().list_movies()

    def check_movie_id(self, movie_id):
        return super().check_movie_id(movie_id)

    def search_movie_by_id(self, movie_id):
        return super().search_movie_by_id(movie_id)

    def search_movie_by_title(self, title):
        return super().search_movie_by_title(title)

    def search_movie_by_description(self, description):
        return super().search_movie_by_description(description)

    def search_movie_by_genre(self, genre):
        return super().search_movie_by_genre(genre)

    def get_all_movies(self):
        return super().get_all_movies()

    def get_movie_title(self, movie_id):
        return super().get_movie_title(movie_id)

    def get_movie_description(self, movie_id):
        return super().get_movie_description(movie_id)

    def get_movie_genre(self, movie_id):
        return super().get_movie_genre(movie_id)

    def save_to_file(self, pickle_file):
        try:
            file = open(pickle_file, "wb")
            for movie in self._movie_data:
                pickle.dump(movie, file)
            file.close()
        except IOError:
            pass

    @staticmethod
    def load_from_file(pickle_file):
        file = open(pickle_file, "rb")
        temp_data = []
        while True:
            try:
                movie = pickle.load(file)
                temp_data.append(movie)
            except EOFError:
                break
        file.close()
        return temp_data
