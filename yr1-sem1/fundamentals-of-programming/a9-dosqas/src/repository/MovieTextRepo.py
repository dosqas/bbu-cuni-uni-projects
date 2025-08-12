from src.repository.MovieRepo import MovieRepo
from src.domain.MovieDomain import Movie


class MovieTextRepo(MovieRepo):
    def __init__(self, file, rental_repo):
        super().__init__(rental_repo)
        self._file = file
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

    def save_to_file(self, textfile):
        try:
            file = open(textfile, "w")
            for movie in self._movie_data:
                file.write(str(movie.movie_id()) + "," + movie.movie_title() + "," +
                           movie.movie_description() + "," + movie.movie_genre() + "\n")
            file.close()
        except IOError:
            pass

    def load_from_file(self, textfile):
        try:
            temp_data = []
            file = open(textfile, "r")
            for line in file:
                line = line.strip()
                line = line.split(",")
                temp_data.append((Movie(line[0], line[1], line[2], line[3])))
            file.close()
            self._movie_data = temp_data.copy()
            return temp_data
        except IOError:
            pass

# 1,Movie1,Description1,Genre1
# 2,Movie2,Description2,Genre2
# 3,Movie3,Description3,Genre3
# 4,Movie4,Description4,Genre4
# 5,Movie5,Description5,Genre5
# 6,Movie6,Description6,Genre6
# 7,Movie7,Description7,Genre7
# 8,Movie8,Description8,Genre8
# 9,Movie9,Description9,Genre9
# 10,Movie10,Description10,Genre10
# 11,Movie11,Description11,Genre11
# 12,Movie12,Description12,Genre12
# 13,Movie13,Description13,Genre13
# 14,Movie14,Description14,Genre14
# 15,Movie15,Description15,Genre15
# 16,Movie16,Description16,Genre16
# 17,Movie17,Description17,Genre17
# 18,Movie18,Description18,Genre18
# 19,Movie19,Description19,Genre19
# 20,Movie20,Description20,Genre20
