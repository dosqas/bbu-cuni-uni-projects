CREATE DATABASE MinimalCinemaDB;
USE MinimalCinemaDB;

-- 1. Film Table
CREATE TABLE Film (
    IDFilm INT PRIMARY KEY IDENTITY,
    Title VARCHAR(50) UNIQUE NOT NULL,
    Director VARCHAR(50) NOT NULL,
    Origin VARCHAR(30) NOT NULL,
    ReleaseYear INT NOT NULL
);

-- 2. Actor Table
CREATE TABLE Actor (
    IDActor INT PRIMARY KEY IDENTITY,
    Name VARCHAR(100) NOT NULL,
    BirthYear INT,
    Country VARCHAR(30)
);

-- 3. FilmActor Table (Many-to-Many relationship between Film and Actor)
CREATE TABLE FilmActor (
    IDFilm INT FOREIGN KEY REFERENCES Film(IDFilm) ON DELETE CASCADE,
    IDActor INT FOREIGN KEY REFERENCES Actor(IDActor) ON DELETE CASCADE,
    PRIMARY KEY (IDFilm, IDActor)
);

-- 4. Action logging table
CREATE TABLE ActionLog (
    LogID INT PRIMARY KEY IDENTITY(1,1),
    ProcedureName NVARCHAR(128) NOT NULL,
    ActionType NVARCHAR(50) NOT NULL,  -- 'INSERT', 'UPDATE', 'DELETE', 'BEGIN TRANSACTION', etc.
    ObjectName NVARCHAR(128) NOT NULL, -- Table or object affected
    Status NVARCHAR(20) NOT NULL,      -- 'SUCCESS', 'FAILED', 'IN PROGRESS'
    Message NVARCHAR(MAX),             -- Additional information or error message
    AffectedRows INT,                  -- Number of rows affected (if applicable)
    ExecutedBy NVARCHAR(128) DEFAULT SYSTEM_USER,
    ExecutedAt DATETIME DEFAULT GETDATE()
);
