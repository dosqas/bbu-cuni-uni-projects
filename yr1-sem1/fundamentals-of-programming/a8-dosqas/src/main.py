from ui.ui import UI
# from gui.gui import GUI

from services.ClientService import ClientService
from services.MovieService import MovieService
from services.RentalService import RentalService

from repository.ClientRepo import ClientRepo
from repository.MovieRepo import MovieRepo
from repository.RentalRepo import RentalRepo

from datetime import date as day
import unittest
# import tkinter as tk

from src.test.test import TestClientService
from src.test.test import TestMovieService
from src.test.test import TestRentalService


def start():
    print("Today is", day.today(), "\n")
    while True:
        UI.input()


if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(TestClientService)
    suite.addTests(unittest.TestLoader().loadTestsFromTestCase(TestMovieService))
    suite.addTests(unittest.TestLoader().loadTestsFromTestCase(TestRentalService))
    unittest.TextTestRunner().run(suite)
    Client_Repo = ClientRepo()
    Movie_Repo = MovieRepo()
    Rental_Repo = RentalRepo()

    Client_Service = ClientService(Client_Repo)
    Movie_Service = MovieService(Movie_Repo)
    Rental_Service = RentalService(Rental_Repo)

    UI = UI(Client_Service, Movie_Service, Rental_Service)

    start()
    # root = tk.Tk()
    # app = GUI(root, client_service=Client_Service, movie_service=Movie_Service, rental_service=Rental_Service)
    # root.mainloop()
