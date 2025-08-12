from src.repository.ClientRepo import ClientRepo
from src.domain.ClientDomain import Client
import pickle


class ClientBinaryRepo(ClientRepo):
    def __init__(self, file, rental_repo):
        super().__init__(rental_repo)
        self._file = file
        # self._client_data = []
        # for cnt in range(1, 21):
        #     self._client_data.append(Client(str(cnt), "Client" + str(cnt)))
        #     self.save_to_file(self._file)

        self._client_data = self.load_from_file(self._file)

    def add_client(self, client):
        super().add_client(client)
        self.save_to_file(self._file)

    def check_client_id(self, client_id):
        return super().check_client_id(client_id)

    def search_client_by_id(self, client_id):
        return super().search_client_by_id(client_id)

    def search_client_by_name(self, client_name):
        return super().search_client_by_name(client_name)

    def remove_client(self, client_id):
        result = super().remove_client(client_id)
        self.save_to_file(self._file)
        return result

    def update_client(self, client_id, client_name):
        super().update_client(client_id, client_name)
        self.save_to_file(self._file)

    def list_clients(self):
        return super().list_clients()

    def get_client_name(self, client_id):
        return super().get_client_name(client_id)

    def save_to_file(self, pickle_file):
        try:
            file = open(pickle_file, "wb")
            for client in self._client_data:
                pickle.dump(client, file)
            file.close()
        except IOError:
            pass

    @staticmethod
    def load_from_file(pickle_file):
        file = open(pickle_file, "rb")
        temp_data = []
        while True:
            try:
                client = pickle.load(file)
                temp_data.append(client)
            except EOFError:
                break
        file.close()
        return temp_data
