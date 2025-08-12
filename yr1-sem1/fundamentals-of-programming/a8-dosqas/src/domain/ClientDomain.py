class Client:
    def __init__(self, client_id, client_name):
        """
        Constructor for Client class
        :param client_id:
        :param client_name:
        """
        self.__client_id = client_id
        self.__client_name = client_name

    def client_id(self):
        """
        Returns the client ID
        :return:
        """
        return self.__client_id

    def client_name(self):
        """
        Returns the client name
        :return:
        """
        return self.__client_name

    def set_client_name(self, client_name):
        """
        Sets the client name
        :param client_name:
        :return:
        """
        self.__client_name = client_name

    def __str__(self):
        """
        String representation of Client class
        :return:
        """
        return "#" + str(self.__client_id) + " " + str(self.__client_name)
