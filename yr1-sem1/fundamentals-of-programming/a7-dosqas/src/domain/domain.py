import json


class Student:
    def __init__(self, student_id, student_name, student_group):
        """
        Creates a student
        :param student_id:
        :param student_name:
        :param student_group:
        """
        self.__student_id = student_id
        self.__student_name = student_name
        self.__student_group = student_group

    def student_id(self):
        """
        Returns the student ID
        :return:
        """
        return str(self.__student_id)

    def student_name(self):
        """
        Returns the student name
        :return:
        """
        return str(self.__student_name)

    def student_group(self):
        """
        Returns the student group
        :return:
        """
        return str(self.__student_group)

    def __str__(self):
        """
        Returns a string representation of the student
        :return:
        """
        return "#" + str(self.__student_id) + " " + str(self.__student_name) + ", group: " + str(self.__student_group)

    def to_json(self):
        return json.dumps(self, default=lambda o: o.__dict__,
                          sort_keys=True, indent=4)
