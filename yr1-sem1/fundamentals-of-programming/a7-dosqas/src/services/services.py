from src.domain.domain import Student


class Services:
    def __init__(self, repository):
        """
        Constructor for the services
        :param repository:
        """
        self.__repo = repository

    def unique(self, id_student):
        """
        Checks if the student ID is unique
        :param id_student:
        :return:
        """
        for student in self.__repo.get_all():
            if student.student_id() == id_student:
                raise ValueError("\nError: Student ID already in use!\n")
        else:
            return True

    def add_student(self, id_student, name, group):
        """
        Adds a student to the repository
        :param id_student:
        :param name:
        :param group:
        :return:
        """
        student = Student(id_student, name, group)
        if self.unique(id_student):
            self.__repo.add(student)

    def unit_test(self):
        """
        Unit test for the first functionality for repo(add)
        :return:
        """
        length = len(self.__repo.get_all())
        self.__repo.add(Student("999", "Student999", "999"))
        assert len(self.__repo.get_all()) == length + 1
        self.__repo.remove(len(self.__repo.get_all()) - 1)

    def unit_test2(self, id_student, name, group):
        """
        Unit test for the second functionality for services(add)
        :return:
        """
        student = Student(id_student, name, group)
        assert student.student_id() == id_student
        assert student.student_name() == name
        assert student.student_group() == group
        self.__repo.add(Student("999", "Student999", "999"))
        assert not self.unique(id_student)
        self.__repo.remove(len(self.__repo.get_all()) - 1)

    def print_list(self):
        """
        Prints the list of students
        :return:
        """
        return self.__repo.get_all()

    def remove_student(self, group):
        """
        Removes a student from the repository
        :param group:
        :return:
        """
        self.__repo.remove(group)

    def undo(self):
        """
        Undoes the last operation
        :return:
        """
        self.__repo.get_all()
        self.__repo.get_stack()
        cnt = 0
        for _ in self.__repo.get_stack():
            cnt += 1
        if cnt > 1:
            self.__repo.undo()
        else:
            raise ValueError("\nError: No more undos!\n")
