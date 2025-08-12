from src.domain.ClientDomain import Client
from src.exceptions.Exceptions import RepositoryException


class ClientRepo:
    def __init__(self, rental_repo):
        """
        Creates a new instance of ClientRepo
        """
        self._client_data = []
        self._rental_repo = rental_repo
        for count in range(1, 21):
            self._client_data.append(Client(str(count), "Client" + str(count)))

    def add_client(self, client):
        """
        Adds a client to the repository
        :param client: Client
        :return: None
        """
        self._client_data.append(client)

    def remove_client(self, client_id):
        """
        Removes a client from the repository
        :param client_id: int
        :return: None
        """
        confirm = False
        array = []
        for client in self._client_data:
            if client.client_id() == client_id:
                self._client_data.remove(client)
                for rental in self._rental_repo.list_rentals():
                    if rental.client_id() == client_id:
                        self._rental_repo.remove(rental.rental_id())
                        array.append(rental)
                return array
        if confirm is False:
            raise RepositoryException("Client ID does not exist!\n")

    def update_client(self, client_id, client_name):
        """
        Updates a client from the repository
        :param client_id: int
        :param client_name: string
        :return: None
        """
        confirm = False
        for client in self._client_data:
            if client.client_id() == client_id:
                client.set_client_name(client_name)
                confirm = True
        if confirm is False:
            raise RepositoryException("Client ID does not exist!\n")

    def list_clients(self):
        """
        Returns the data of clients
        :return: data
        """
        return self._client_data

    def check_client_id(self, client_id):
        """
        Checks if the client id exists
        :param client_id: int
        :return: bool
        """
        for client in self._client_data:
            if client.client_id() == client_id:
                return True

    def search_client_by_id(self, client_id):
        """
        Searches for a client by partial string matching
        :param client_id: int
        :return: client
        """
        client_list = []
        for client in self._client_data:
            if client_id in client.client_id():
                client_list.append(client)

        return client_list

    def search_client_by_name(self, client_name):
        """
        Searches for a client by partial string matching
        :param client_name: string
        :return: client
        """
        client_list = []
        for client in self._client_data:
            if client_name.lower() in client.client_name().lower():
                client_list.append(client)

        return client_list

    def get_client_name(self, client_id):
        for client in self._client_data:
            if client.client_id() == client_id:
                return client.client_name()
