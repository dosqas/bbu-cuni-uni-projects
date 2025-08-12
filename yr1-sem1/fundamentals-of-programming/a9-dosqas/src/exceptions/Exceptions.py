class ShowException:
    """
    Exception class to inherit from
    """
    pass


class ValidationException(Exception):
    """
    Exception class for UI
    """
    def __init__(self, message):
        self.__error_message = message

    def __str__(self):
        return "Validation exception: " + self.__error_message


class RepositoryException(Exception):
    """
    Exception class for repository
    """
    def __init__(self, message):
        self.__error_message = message

    def __str__(self):
        return "Repository exception: " + self.__error_message

class UndoRedoException(Exception):
    """
    Exception class for undo/redo
    """
    def __init__(self, message):
        self.__error_message = message

    def __str__(self):
        return "Undo/Redo exception: " + self.__error_message
