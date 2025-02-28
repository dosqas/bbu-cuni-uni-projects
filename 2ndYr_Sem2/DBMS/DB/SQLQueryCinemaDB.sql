CREATE DATABASE CinemaDB;
USE CinemaDB;

-- Cinema Table
CREATE TABLE Cinema (
    IDCinema INT PRIMARY KEY IDENTITY, 
    Name VARCHAR(30) UNIQUE NOT NULL,
    City VARCHAR(30) NOT NULL,
    SeatCount INT NOT NULL
);

INSERT INTO Cinema (Name, City, SeatCount) VALUES
('Cineplex Downtown', 'New York', 500),
('Metro Cinema', 'Los Angeles', 450),
('Royal Theater', 'Chicago', 600),
('Starlight Cinemas', 'Houston', 550),
('Galaxy Movies', 'Phoenix', 400),
('City Lights', 'Philadelphia', 480),
('Silver Screen', 'San Antonio', 520),
('Golden Reel', 'San Diego', 470),
('Majestic Theater', 'Dallas', 530),
('Paramount Cinemas', 'San Jose', 490);

-- Film Table
CREATE TABLE Film (
    IDFilm INT PRIMARY KEY IDENTITY,
    Title VARCHAR(50) UNIQUE NOT NULL,
    Director VARCHAR(50) NOT NULL,
    Origin VARCHAR(30) NOT NULL,
    ReleaseYear INT NOT NULL
);

INSERT INTO Film (Title, Director, Origin, ReleaseYear) VALUES
('Inception', 'Christopher Nolan', 'USA', 2010),
('The Dark Knight', 'Christopher Nolan', 'USA', 2008),
('Interstellar', 'Christopher Nolan', 'USA', 2014),
('The Matrix', 'Lana Wachowski', 'USA', 1999),
('Gladiator', 'Ridley Scott', 'USA', 2000),
('Parasite', 'Bong Joon-ho', 'South Korea', 2019),
('The Godfather', 'Francis Ford Coppola', 'USA', 1972),
('Pulp Fiction', 'Quentin Tarantino', 'USA', 1994),
('Titanic', 'James Cameron', 'USA', 1997),
('Avatar', 'James Cameron', 'USA', 2009);

-- Many-to-Many: Film & Actors
CREATE TABLE Actor (
    IDActor INT PRIMARY KEY IDENTITY,
    Name VARCHAR(50) NOT NULL,
    BirthYear INT NOT NULL
);

INSERT INTO Actor (Name, BirthYear) VALUES
('Leonardo DiCaprio', 1974),
('Christian Bale', 1974),
('Matthew McConaughey', 1969),
('Keanu Reeves', 1964),
('Russell Crowe', 1964),
('Song Kang-ho', 1967),
('Marlon Brando', 1924),
('John Travolta', 1954),
('Kate Winslet', 1975),
('Sam Worthington', 1976);

CREATE TABLE FilmActor (
    IDFilm INT FOREIGN KEY REFERENCES Film(IDFilm) ON DELETE CASCADE,
    IDActor INT FOREIGN KEY REFERENCES Actor(IDActor) ON DELETE CASCADE,
    PRIMARY KEY (IDFilm, IDActor)
);

INSERT INTO FilmActor (IDFilm, IDActor) VALUES
(1, 1), (2, 2), (3, 3), (4, 4), (5, 5),
(6, 6), (7, 7), (8, 8), (9, 9), (10, 10);

-- Genre Table (1:N with Film)
CREATE TABLE Genre (
    IDGenre INT PRIMARY KEY IDENTITY,
    GenreName VARCHAR(30) UNIQUE NOT NULL
);

INSERT INTO Genre (GenreName) VALUES
('Action'), ('Drama'), ('Sci-Fi'), ('Thriller'),
('Adventure'), ('Comedy'), ('Horror'), ('Romance'),
('Crime'), ('Fantasy');

CREATE TABLE FilmGenre (
    IDFilm INT FOREIGN KEY REFERENCES Film(IDFilm) ON DELETE CASCADE,
    IDGenre INT FOREIGN KEY REFERENCES Genre(IDGenre) ON DELETE CASCADE,
    PRIMARY KEY (IDFilm, IDGenre)
);

INSERT INTO FilmGenre (IDFilm, IDGenre) VALUES
(1, 3), (1, 4), (2, 1), (2, 4), (3, 3),
(4, 1), (4, 3), (5, 2), (5, 5), (6, 2);

-- Hall Table (each Cinema has multiple halls)
CREATE TABLE Hall (
    IDHall INT PRIMARY KEY IDENTITY,
    IDCinema INT FOREIGN KEY REFERENCES Cinema(IDCinema) ON DELETE CASCADE,
    HallNumber INT NOT NULL,
    Capacity INT NOT NULL
);

INSERT INTO Hall (IDCinema, HallNumber, Capacity) VALUES
(1, 1, 100), (1, 2, 150), (2, 1, 120), (2, 2, 130),
(3, 1, 200), (3, 2, 180), (4, 1, 90), (4, 2, 110),
(5, 1, 140), (5, 2, 160);

-- Program Table (Screening schedule)
CREATE TABLE Program (
    IDProgram INT PRIMARY KEY IDENTITY,
    IDCinema INT FOREIGN KEY REFERENCES Cinema(IDCinema) ON DELETE NO ACTION,
    IDFilm INT FOREIGN KEY REFERENCES Film(IDFilm) ON DELETE NO ACTION,
    IDHall INT FOREIGN KEY REFERENCES Hall(IDHall) ON DELETE SET NULL, 
    Day VARCHAR(10) NOT NULL,
    StartTime TIME NOT NULL
);

INSERT INTO Program (IDCinema, IDFilm, IDHall, Day, StartTime) VALUES
(1, 1, 1, 'Monday', '18:00'), (1, 2, 2, 'Tuesday', '19:00'),
(2, 3, 3, 'Wednesday', '20:00'), (2, 4, 4, 'Thursday', '21:00'),
(3, 5, 5, 'Friday', '17:00'), (3, 6, 6, 'Saturday', '18:30'),
(4, 7, 7, 'Sunday', '19:00'), (4, 8, 8, 'Monday', '20:00'),
(5, 9, 9, 'Tuesday', '21:00'), (5, 10, 10, 'Wednesday', '22:00');

INSERT INTO Program (IDCinema, IDFilm, IDHall, Day, StartTime) VALUES
-- Additional screenings for IDFilm 1 (Inception)
(1, 1, 1, 'Wednesday', '15:00'), -- Same cinema, different day and time
(2, 1, 3, 'Friday', '19:30'),    -- Different cinema, same film

-- Additional screenings for IDFilm 2 (The Dark Knight)
(1, 2, 2, 'Thursday', '20:00'),  -- Same cinema, different day and time
(3, 2, 5, 'Sunday', '18:00'),    -- Different cinema, same film

-- Additional screenings for IDFilm 3 (Interstellar)
(2, 3, 4, 'Saturday', '14:00'),  -- Same cinema, different day and time
(4, 3, 7, 'Tuesday', '21:30'),   -- Different cinema, same film

-- Additional screenings for IDFilm 4 (The Matrix)
(2, 4, 4, 'Monday', '17:00'),    -- Same cinema, different day and time
(5, 4, 9, 'Wednesday', '20:00'), -- Different cinema, same film

-- Additional screenings for IDFilm 5 (Gladiator)
(3, 5, 5, 'Thursday', '19:00'),  -- Same cinema, different day and time
(1, 5, 1, 'Sunday', '16:00');    -- Different cinema, same film

-- Clients Table
CREATE TABLE Client (
    IDClient INT PRIMARY KEY IDENTITY,
    Name VARCHAR(30) UNIQUE NOT NULL,
    Email VARCHAR(50) UNIQUE NOT NULL
);

INSERT INTO Client (Name, Email) VALUES
('John Doe', 'john.doe@example.com'),
('Jane Smith', 'jane.smith@example.com'),
('Alice Johnson', 'alice.johnson@example.com'),
('Bob Brown', 'bob.brown@example.com'),
('Charlie Davis', 'charlie.davis@example.com'),
('Eve White', 'eve.white@example.com'),
('Frank Wilson', 'frank.wilson@example.com'),
('Grace Lee', 'grace.lee@example.com'),
('Henry Harris', 'henry.harris@example.com'),
('Ivy Clark', 'ivy.clark@example.com');

-- Tickets Table (Client buying tickets for a program)
CREATE TABLE Ticket (
    IDTicket INT PRIMARY KEY IDENTITY,
    IDProgram INT FOREIGN KEY REFERENCES Program(IDProgram) ON DELETE CASCADE,
    IDClient INT FOREIGN KEY REFERENCES Client(IDClient) ON DELETE CASCADE,
    PurchaseDate DATE NOT NULL
);

INSERT INTO Ticket (IDProgram, IDClient, PurchaseDate) VALUES
(1, 1, '2023-10-01'), (2, 2, '2023-10-02'),
(3, 3, '2023-10-03'), (4, 4, '2023-10-04'),
(5, 5, '2023-10-05'), (6, 6, '2023-10-06'),
(7, 7, '2023-10-07'), (8, 8, '2023-10-08'),
(9, 9, '2023-10-09'), (10, 10, '2023-10-10');

-- Reviews Table (Clients can review films)
CREATE TABLE Review (
    IDReview INT PRIMARY KEY IDENTITY,
    IDClient INT FOREIGN KEY REFERENCES Client(IDClient) ON DELETE CASCADE,
    IDFilm INT FOREIGN KEY REFERENCES Film(IDFilm) ON DELETE CASCADE,
    Rating INT CHECK (Rating BETWEEN 1 AND 10),
    Comment TEXT
);

INSERT INTO Review (IDClient, IDFilm, Rating, Comment) VALUES
(1, 1, 9, 'Amazing movie!'), (2, 2, 10, 'Masterpiece!'),
(3, 3, 8, 'Mind-blowing visuals'), (4, 4, 9, 'Iconic film'),
(5, 5, 7, 'Great acting'), (6, 6, 10, 'Brilliant storytelling'),
(7, 7, 9, 'Classic'), (8, 8, 8, 'Quirky and fun'),
(9, 9, 7, 'Emotional'), (10, 10, 8, 'Spectacular visuals');

-- Employee Table (Cinema employees)
CREATE TABLE Employee (
    IDEmployee INT PRIMARY KEY IDENTITY,
    IDCinema INT FOREIGN KEY REFERENCES Cinema(IDCinema) ON DELETE CASCADE,
    Name VARCHAR(50) NOT NULL,
    Role VARCHAR(30) NOT NULL,
    Salary DECIMAL(10, 2) NOT NULL
);

INSERT INTO Employee (IDCinema, Name, Role, Salary) VALUES
(1, 'Michael Scott', 'Manager', 5000.00),
(1, 'Dwight Schrute', 'Assistant Manager', 4000.00),
(2, 'Pam Beesly', 'Receptionist', 3000.00),
(2, 'Jim Halpert', 'Projectionist', 3500.00),
(3, 'Angela Martin', 'Accountant', 4500.00),
(3, 'Kevin Malone', 'Concession Staff', 2500.00),
(4, 'Stanley Hudson', 'Security', 3200.00),
(4, 'Phyllis Vance', 'Cleaner', 2800.00),
(5, 'Oscar Martinez', 'IT Support', 3800.00),
(5, 'Ryan Howard', 'Intern', 2000.00);

-- Membership Table (Clients with VIP access)
CREATE TABLE Membership (
    IDMembership INT PRIMARY KEY IDENTITY,
    IDClient INT FOREIGN KEY REFERENCES Client(IDClient) ON DELETE CASCADE,
    ExpirationDate DATE NOT NULL
);

INSERT INTO Membership (IDClient, ExpirationDate) VALUES
(1, '2024-10-01'), (2, '2024-10-02'),
(3, '2024-10-03'), (4, '2024-10-04'),
(5, '2024-10-05'), (6, '2024-10-06'),
(7, '2024-10-07'), (8, '2024-10-08'),
(9, '2024-10-09'), (10, '2024-10-10');