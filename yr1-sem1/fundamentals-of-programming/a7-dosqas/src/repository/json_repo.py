from src.domain.domain import Student
from src.repository.mem_repo import MemRepo
import json
# import random


class JsonRepo(MemRepo):
    def __init__(self):
        """
        Constructor for the json repository
        """
        super().__init__()
        self._data = self.load_from_file("jsonfile.json")
        # self._data = []
        # for cnt in range(1, 11):
        #     self._data.append(Student(str(cnt), "Student" + str(cnt), str(random.randint(1, 10))))
        #     self.save_to_file("jsonfile.json")
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
        self.save_to_file("jsonfile.json")

    def remove(self, group):
        """
        Removes a student from the repository
        :param group:
        :return:
        """
        super().remove(group)
        self.save_to_file("jsonfile.json")

    def undo(self):
        """
        Undoes the last operation
        :return:
        """
        super().undo()
        self.save_to_file("jsonfile.json")

    @staticmethod
    def load_from_file(jsonfile):
        """
        Loads the list from a json file
        :param jsonfile:
        :return:
        """
        file = open(jsonfile, "r")
        temp_data = []
        students = json.load(file)
        for student in students:
            temp_data.append(Student(student["_Student__student_id"], student["_Student__student_name"],
                                     student["_Student__student_group"]))
        file.close()
        return temp_data

    def save_to_file(self, jsonfile):
        """
        Saves the list to a json file
        :param jsonfile:
        :return:
        """
        file = open(jsonfile, "w")
        file.write("[\n")
        for student in self._data:
            file.write(student.to_json())
            if student != self._data[-1]:
                file.write(",\n")
        file.write("\n]")
        file.close()
