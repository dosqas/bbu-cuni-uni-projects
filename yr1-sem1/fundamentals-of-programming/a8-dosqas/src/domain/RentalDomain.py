class Rental:
    def __init__(self, rental_id, client_id, movie_id, rented_date, due_date, returned_date):
        """
        Creates new instance of a rental
        :param rental_id:
        :param client_id:
        :param movie_id:
        :param rented_date:
        :param due_date:
        :param returned_date:
        """
        self.__rental_id = rental_id
        self.__client_id = client_id
        self.__movie_id = movie_id
        self.__rented_date = rented_date
        self.__due_date = due_date
        self.__returned_date = returned_date

    def rental_id(self):
        """
        Returns the rental id
        :return:
        """
        return self.__rental_id

    def client_id(self):
        """
        Returns the client id
        :return:
        """
        return self.__client_id

    def movie_id(self):
        """
        Returns the movie id
        :return:
        """
        return self.__movie_id

    def due_date(self):
        """
        Returns the due date
        :return:
        """
        return self.__due_date

    def returned_date(self):
        """
        Returns the returned date
        :return:
        """
        return self.__returned_date

    def set_returned_date(self, returned_date):
        """
        Sets the returned date
        :param returned_date:
        :return:
        """
        self.__returned_date = returned_date
