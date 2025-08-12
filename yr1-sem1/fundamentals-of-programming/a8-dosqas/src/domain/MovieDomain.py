class Movie:
    def __init__(self, movie_id, title, description, genre):
        """
        Constructor for Movie class
        :param movie_id:
        :param title:
        :param description:
        :param genre:
        """
        self.__movie_id = movie_id
        self.__title = title
        self.__description = description
        self.__genre = genre

    def movie_id(self):
        """
        Returns the movie ID
        :return:
        """
        return self.__movie_id

    def movie_title(self):
        """
        Returns the movie title
        :return:
        """
        return self.__title

    def movie_description(self):
        """
        Returns the movie description
        :return:
        """
        return self.__description

    def movie_genre(self):
        """
        Returns the movie genre
        :return:
        """
        return self.__genre

    def set_movie_title(self, title):
        """
        Sets the movie title
        :param title:
        :return:
        """
        self.__title = title

    def set_movie_description(self, description):
        """
        Sets the movie description
        :param description:
        :return:
        """
        self.__description = description

    def set_movie_genre(self, genre):
        """
        Sets the movie genre
        :param genre:
        :return:
        """
        self.__genre = genre

    def __str__(self):
        """
        String representation of Movie
        :return:
        """
        return ("#" + str(self.__movie_id) + " " + str(self.__title) + " /*/ "
                + str(self.__description) + " - " + str(self.__genre))
