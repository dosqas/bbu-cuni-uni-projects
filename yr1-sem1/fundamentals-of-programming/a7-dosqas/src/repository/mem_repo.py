from src.domain.domain import Student
import random


class MemRepo:
    def __init__(self):
        """
        Constructor for the repository
        """
        self._data = []
        self._stack = []
        self._temp_data_sql = []

        for cnt in range(1, 11):
            self._data.append(Student(str(cnt), "Student" + str(cnt), str(random.randint(1, 10))))

        self._stack.append(self._data.copy())

    def add(self, new_student: Student):
        """
        Adds a student to the repository
        :param new_student:
        :return:
        """
        self._data.append(new_student)
        self._stack.append(self._data.copy())

    def remove(self, group):
        """
        Removes a student from the repository
        :param group:
        :return:
        """
        length = len(self._data)
        temp_data = []
        for cnt in range(length):
            if not self._data[cnt].student_group() == group:
                temp_data.append(self._data[cnt])
            else:
                self._temp_data_sql.append(self._data[cnt])

        self._data = temp_data.copy()
        self._stack.append(self._data.copy())

    def get_all(self):
        """
        Returns the list of students
        :return:
        """
        return self._data

    def get_stack(self):
        """
        Returns the stack
        :return:
        """
        return self._stack

    def undo(self):
        """
        Undoes the last operation
        :return:
        """
        self._stack.pop()
        self._data = self._stack[-1]
