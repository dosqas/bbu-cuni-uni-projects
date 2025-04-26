USE MinimalCinemaDB;

-- Clear existing data and setup (run this once before any tests)
DELETE FROM FilmActor;
DELETE FROM Actor;
DELETE FROM Film;
DBCC CHECKIDENT ('Film', RESEED, 0);
DBCC CHECKIDENT ('Actor', RESEED, 0);

-- Insert sample data for testing
INSERT INTO Film (Title, Director, Origin, ReleaseYear) VALUES
('The Godfather', 'Francis Ford Coppola', 'USA', 1972),
('Pulp Fiction', 'Quentin Tarantino', 'USA', 1994);

INSERT INTO Actor (Name, BirthYear, Country) VALUES
('Marlon Brando', 1924, 'USA'),
('Al Pacino', 1940, 'USA'),
('John Travolta', 1954, 'USA');

INSERT INTO FilmActor (IDFilm, IDActor) VALUES
(1, 1), -- Marlon Brando in The Godfather
(1, 2), -- Al Pacino in The Godfather
(2, 3); -- John Travolta in Pulp Fiction

-- ==============================================
-- 1. DIRTY READ DEMONSTRATION (Connection 1)
-- ==============================================
PRINT '=== DIRTY READ TEST - CONNECTION 1 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 1: Updating film title without committing...';
    UPDATE Film SET Title = 'The Godfather Part IV' WHERE IDFilm = 1;
    -- DON'T COMMIT YET - leave this transaction open
    PRINT 'Connection 1: Transaction is open. Switch to Connection 2 now.';
    WAITFOR DELAY '00:00:10'; -- Gives 10 seconds to run Connection 2
    -- After running Connection 2, come back here and either:
    -- COMMIT; -- To keep changes
    ROLLBACK; -- To undo changes
PRINT 'Connection 1: Transaction completed.';

-- ==============================================
-- 2. NON-REPEATABLE READ DEMONSTRATION (Connection 1)
-- ==============================================
PRINT '=== NON-REPEATABLE READ TEST - CONNECTION 1 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 1: First read of Al Pacino:';
    SELECT * FROM Actor WHERE Name = 'Al Pacino';
    
    PRINT 'Connection 1: Waiting 10 seconds for Connection 2 to update...';
    WAITFOR DELAY '00:00:10';
    
    PRINT 'Connection 1: Second read of Al Pacino (should be different):';
    SELECT * FROM Actor WHERE Name = 'Al Pacino';
COMMIT;

-- ==============================================
-- 3. PHANTOM READ DEMONSTRATION (Connection 1)
-- ==============================================
PRINT '=== PHANTOM READ TEST - CONNECTION 1 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 1: First read of 2000-2010 films:';
    SELECT * FROM Film WHERE ReleaseYear BETWEEN 2000 AND 2010;
    
    PRINT 'Connection 1: Waiting 10 seconds for Connection 2 to insert...';
    WAITFOR DELAY '00:00:10';
    
    PRINT 'Connection 1: Second read (should show new phantom row):';
    SELECT * FROM Film WHERE ReleaseYear BETWEEN 2000 AND 2010;
COMMIT;

-- ==============================================
-- 4. DEADLOCK DEMONSTRATION (Connection 1)
-- ==============================================
PRINT '=== DEADLOCK TEST - CONNECTION 1 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 1: Updating The Godfather...';
    UPDATE Film SET Director = 'Coppola' WHERE Title = 'The Godfather';
    
    PRINT 'Connection 1: Waiting 3 seconds for Connection 2...';
    WAITFOR DELAY '00:00:03';
    
    PRINT 'Connection 1: Updating Marlon Brando...';
    UPDATE Actor SET Name = 'Brando Jr.' WHERE Name = 'Marlon Brando';
COMMIT;
PRINT 'Connection 1: Deadlock test completed (if no errors)';

-- ==============================================
-- CLEANUP (run after all tests)
-- ==============================================
PRINT '=== CLEANUP ===';
-- Check what data was changed
SELECT * FROM Film;
SELECT * FROM Actor;
SELECT * FROM FilmActor;

-- Reset to original data
UPDATE Film SET 
    Title = 'The Godfather',
    Director = 'Francis Ford Coppola',
    ReleaseYear = 1972
WHERE IDFilm = 1;

UPDATE Actor SET
    Name = 'Marlon Brando',
    BirthYear = 1924
WHERE IDActor = 1;

UPDATE Actor SET
    Name = 'Al Pacino',
    BirthYear = 1940
WHERE IDActor = 2;

DELETE FROM Film WHERE Title = 'Inception';

PRINT 'Connection 1: Cleanup complete. All tests finished.';