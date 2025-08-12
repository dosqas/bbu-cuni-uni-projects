from src.domain.domain import Student
from src.repository.mem_repo import MemRepo


class TextRepo(MemRepo):
    def __init__(self):
        """
        Constructor for the text repository
        """
        super().__init__()
        self._data = self.load_from_file("textfile.txt")
        self._stack = []
        if len(self._data) > 0:
            self._stack.append(self._data.copy())
        else:
            self._stack.append([])

    def add(self, new_student: Student):
        """
        Adds a student to the repository
        :param new_student:
        :return:
        """
        super().add(new_student)
        self.save_to_file("textfile.txt")

    def remove(self, group):
        """
        Removes a student from the repository
        :param group:
        :return:
        """
        super().remove(group)
        self.save_to_file("textfile.txt")

    def undo(self):
        """
        Undoes the last operation
        :return:
        """
        super().undo()
        self.save_to_file("textfile.txt")

    def load_from_file(self, textfile):
        """
        Loads the list from a text file
        :param textfile:
        :return:
        """
        try:
            temp_data = []
            file = open(textfile, "r")
            for line in file:
                line = line.strip()
                line = line.split(",")
                temp_data.append((Student(line[0], line[1], line[2])))
            file.close()
            self._data = temp_data.copy()
            return temp_data
        except IOError:
            pass

    def save_to_file(self, textfile):
        """
        Saves the list to a text file
        :param textfile:
        :return:
        """
        try:
            file = open(textfile, "w")
            for student in self._data:
                file.write(str(student.student_id()) + "," + str(student.student_name())
                           + "," + str(student.student_group()) + "\n")
            file.close()
        except IOError as e:
            print("Error: " + str(e))

# 1,Student1,7
# 2,Student2,10
# 3,Student3,5
# 4,Student4,10
# 5,Student5,10
# 6,Student6,4
# 7,Student7,1
# 8,Student8,2
# 9,Student9,8
# 10,Student10,5
