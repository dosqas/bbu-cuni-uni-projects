# import random
import pickle
from src.domain.domain import Student
from src.repository.mem_repo import MemRepo


class BinaryRepo(MemRepo):
    def __init__(self):
        """
        Constructor for the binary repository
        """
        super().__init__()
        self._data = self.load_from_file("binary.pkl")
        # self._data = []
        # for cnt in range(1, 11):
        #     self._data.append(Student(str(cnt), "Student" + str(cnt), str(random.randint(1, 10))))
        #     self.save_to_file("binary.pkl")
        self._stack = []
        if self._data is not None:
            self._stack.append(self._data.copy())
        else:
            self._data = []
            self._stack.append(self._data.copy())

    def add(self, new_student: Student):
        """
        Adds a student to the repository
        :param new_student:
        :return:
        """
        super().add(new_student)
        self.save_to_file("binary.pkl")

    def remove(self, group):
        """
        Removes a student from the repository
        :param group:
        :return:
        """
        super().remove(group)
        self.save_to_file("binary.pkl")

    def undo(self):
        """
        Undoes the last operation
        :return:
        """
        super().undo()
        self.save_to_file("binary.pkl")

    @staticmethod
    def load_from_file(pkl_file):
        """
        Loads the list from a binary file
        :param pkl_file:
        :return:
        """
        file = open(pkl_file, "rb")
        temp_data = []
        while True:
            try:
                student = pickle.load(file)
                temp_data.append(student)
            except EOFError:
                break
        file.close()
        return temp_data

    def save_to_file(self, pkl_file):
        """
        Saves the list to a binary file
        :param pkl_file:
        :return:
        """
        file = open(pkl_file, "wb")
        for student in self._data:
            pickle.dump(student, file)
        file.close()
