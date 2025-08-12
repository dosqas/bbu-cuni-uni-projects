from src.domain.domain import Student
from src.repository.mem_repo import MemRepo
import sqlite3
import random


class SqlRepo(MemRepo):
    def __init__(self):
        """
        Constructor for the sql repository
        """
        super().__init__()
        self.table_with_undo = TableWithUndo()
        self._data = self.load_from_file("database.db")
        # connection = sqlite3.connect('database.db')
        # cursor = connection.cursor()
        #
        # cursor.execute('''
        # CREATE TABLE users (
        #     student_id TEXT,
        #     student_name TEXT,
        #     student_group TEXT
        # )
        # ''')
        #
        # connection.commit()
        # connection.close()
        # self._data = []
        # for cnt in range(1, 11):
        #     self._data.append(Student(str(cnt), "Student" + str(cnt), str(random.randint(1, 10))))
        #     self.save_to_file("database.db")
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
        self.table_with_undo.add_row(new_student)
        self.save_to_file("database.db")

    def remove(self, group):
        """
        Removes a student from the repository
        :param group:
        :return:
        """
        super().remove(group)
        connection = sqlite3.connect('database.db')
        cursor = connection.cursor()

        try:
            # Database operations (including DELETE and COMMIT)
            for student in self._temp_data_sql:
                std_id = student.student_id()
                std_name = student.student_name()
                std_group = student.student_group()
                cursor.execute("DELETE FROM users WHERE student_id = ? AND student_name = ? AND student_group = ?",
                               (std_id, std_name, std_group))

            self.table_with_undo.remove_row(group)

            connection.commit()
        except sqlite3.Error as e:
            print("SQLite error:", e)
        finally:
            # Close the connection
            connection.close()

    def undo(self):
        """
        Undoes the last operation
        :return:
        """
        super().undo()
        self.table_with_undo.undo()

    @staticmethod
    def load_from_file(sql_file):
        """
        Loads the list from a sql file
        :param sql_file:
        :return:
        """
        connection = sqlite3.connect(sql_file)
        cursor = connection.cursor()

        cursor.execute("SELECT * FROM users")
        temp_data = []

        for row in cursor.fetchall():
            temp_data.append(Student(row[0], row[1], row[2]))

        connection.close()
        return temp_data

    def save_to_file(self, sql_file):
        """
        Saves the list to a sql file
        :param sql_file:
        :return:
        """
        connection = sqlite3.connect(sql_file)
        cursor = connection.cursor()
        student_id = self._data[-1].student_id()
        student_name = self._data[-1].student_name()
        student_group = self._data[-1].student_group()

        cursor.execute("INSERT INTO users (student_id, student_name, student_group) VALUES (?, ?, ?)",
                       (student_id, student_name, student_group))

        connection.commit()
        connection.close()


class TableWithUndo:
    def __init__(self):
        self.conn = sqlite3.connect("database.db")
        self.cursor = self.conn.cursor()
        self.undo_stack = []

    def add_row(self, table_name, row_data):
        query = f"INSERT INTO {table_name} VALUES (?, ?)"
        self.cursor.execute(query, (row_data['id'], row_data['name']))
        self.conn.commit()
        self.undo_stack.append(('add', table_name, row_data['id']))

    def remove_row(self, table_name, student_id):
        query = f"DELETE FROM {table_name} WHERE id = ?"
        self.cursor.execute(query, (student_id,))
        self.conn.commit()
        self.undo_stack.append(('remove', table_name, student_id))

    def undo(self):
        if self.undo_stack:
            action, table_name, item_id = self.undo_stack.pop()
            if action == 'add':
                query = f"DELETE FROM {table_name} WHERE id = ?"
                self.cursor.execute(query, (item_id,))
                self.conn.commit()
            elif action == 'remove':
                query = f"INSERT INTO {table_name} VALUES (?, ?)"
                self.cursor.execute(query, (item_id, 'Dummy Name'))
                self.conn.commit()