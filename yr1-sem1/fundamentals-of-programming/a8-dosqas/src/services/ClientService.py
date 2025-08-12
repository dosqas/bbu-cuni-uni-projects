from src.domain.ClientDomain import Client


class ClientService:
    def __init__(self, client_repository):
        """
        Constructor for ClientService class
        :param client_repository:
        """
        self.__client_repository = client_repository

    def add_client(self, client_id, client_name):
        """
        Adds a client to the repository
        :param client_id: int
        :param client_name: string
        :return: None
        """
        client = Client(client_id, client_name)
        self.__client_repository.add_client(client)

    def check_client_id_unique(self, client_id):
        """
        Checks if the client id exists
        :param client_id: int
        :return: bool
        """
        for client in self.__client_repository.list_clients():
            if client.client_id() == client_id:
                return False
        return True

    def remove_client(self, client_id):
        """
        Removes a client from the repository
        :param client_id: int
        :return: None
        """
        self.__client_repository.remove_client(client_id)

    def can_remove_client(self, client_id):
        """
        Checks if a client can be removed
        :param client_id: int
        :return: bool
        """
        for client in self.__client_repository.list_clients():
            if client.client_id() == client_id:
                return True
        return False

    def update_client(self, client_id, client_name):
        """
        Updates a client from the repository
        :param client_id: int
        :param client_name: string
        :return: None
        """
        self.__client_repository.update_client(client_id, client_name)

    def can_update_client(self, client_id):
        """
        Checks if a client can be updated
        :param client_id: int
        :return: bool
        """
        for client in self.__client_repository.list_clients():
            if client.client_id() == client_id:
                return True

    def list_clients(self):
        """
        Returns the list of clients
        :return: list
        """
        return self.__client_repository.list_clients()

    def check_client_id(self, client_id):
        """
        Checks if a client ID exists
        :param client_id: int
        :return: bool
        """
        return self.__client_repository.check_client_id(client_id)

    def search_client_by_id(self, client_id):
        """
        Searches for a client
        :param client_id: int
        :return: client
        """
        return self.__client_repository.search_client_by_id(client_id)

    def search_client_by_name(self, client_name):
        """
        Searches for a client
        :param client_name: string
        :return: client
        """
        return self.__client_repository.search_client_by_name(client_name)
