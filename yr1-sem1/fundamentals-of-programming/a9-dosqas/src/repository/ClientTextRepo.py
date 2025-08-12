from src.repository.ClientRepo import ClientRepo
from src.domain.ClientDomain import Client


class ClientTextRepo(ClientRepo):
    def __init__(self, file, rental_repo):
        super().__init__(rental_repo)
        self._file = file
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

    def save_to_file(self, textfile):
        try:
            file = open(textfile, "w")
            for client in self._client_data:
                file.write(str(client.client_id()) + "," + client.client_name() + "\n")
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
                temp_data.append((Client(line[0], line[1])))
            file.close()
            self._client_data = temp_data.copy()
            return temp_data
        except IOError:
            pass

# 1,Client1
# 2,Client2
# 3,Client3
# 4,Client4
# 5,Client5
# 6,Client6
# 7,Client7
# 8,Client8
# 9,Client9
# 10,Client10
# 11,Client11
# 12,Client12
# 13,Client13
# 14,Client14
# 15,Client15
# 16,Client16
# 17,Client17
# 18,Client18
# 19,Client19
# 20,Client20
