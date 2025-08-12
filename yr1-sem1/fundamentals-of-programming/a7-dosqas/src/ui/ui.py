class UI:
    def __init__(self, services):
        """
        Constructor for the UI
        :param services:
        """
        self.__task_service = services

    def menu(self):
        """
        Prints the menu
        :return:
        """
        print("Hello! This is the menu. The following functionalities are provided:\n"
              "1 - Add a student.\n"
              "2 - Display the list of students.\n"
              "3 - Filtering the list (by group).\n"
              "4 - Undo the LPO.\n"
              "5 - Exit.\n")

        # self.__task_service.unit_test()
        # self.__task_service.unit_test2("999", "Student999", "999")

        self.__read_input()

    def __read_input(self):
        """
        Reads the input and validates it
        :return:
        """
        print("Type a number for the operation you wish to perform.")
        try:
            op = int(input("Enter input: "))
            if op == 1:
                print("Enter a student. Each student has an id, a name and a group.\n"
                      "Syntax is: <id> <name> <group>\n")
                inp = input("Enter input: ")
                words = inp.split()
                if len(words) == 3:
                    if words[0].isdigit():
                        if words[1].isalpha():
                            if words[2].isdigit():
                                if int(words[2]) >= 0:
                                    self.__task_service.add_student(words[0], words[1], words[2])
                                else:
                                    raise ValueError("\nError: Group cannot be negative!\n")
                            else:
                                raise ValueError("\nError: Group has to be an integer!\n")
                        else:
                            raise ValueError("\nError: Name is not valid!\n")
                    else:
                        raise ValueError("\nError: ID has to be a positive integer!\n")
                else:
                    raise ValueError("\nError: Invalid syntax!\n")
            elif op == 2:
                print("\nThe list of students is: ")
                self.print_list()
            elif op == 3:
                print("\nEnter a group. The respective group will be filtered out.\n"
                      "Syntax is: <group>\n")
                inp = input("Enter input: ")
                words = inp.split()
                if len(words) == 1:
                    if words[0].isdigit():
                        if int(words[0]) >= 0:
                            self.__task_service.remove_student(words[0])
                            print("\nAll the students in the requested group "
                                  "(if there were any), have been filtered out.")
                        else:
                            raise ValueError("\nError: Group cannot be negative!\n")
                    else:
                        raise ValueError("\nError: Group has to be an integer!\n")
                else:
                    raise ValueError("\nError: Invalid syntax!\n")
            elif op == 4:
                self.__task_service.undo()
                print("\nThe last operation (if there was one), has been undone.")
            elif op == 5:
                print("\nGoodbye.")
                quit()
            else:
                raise ValueError("\nError: Invalid operation!\n")
        except ValueError as ve:
            print(ve)
        else:
            print("Operation successful.\n")

    def print_list(self):
        """
        Prints the list of students
        :return:
        """
        for student in self.__task_service.print_list():
            print(str(student))

        if len(self.__task_service.print_list()) == 0:
            print("The list is empty.\n")
