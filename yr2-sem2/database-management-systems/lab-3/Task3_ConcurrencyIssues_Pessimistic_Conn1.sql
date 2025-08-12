USE MinimalCinemaDB;
GO

-- ==============================================
-- CLEAN SETUP: Run only once to reset the test data
-- ==============================================
DELETE FROM FilmActor;
DELETE FROM Actor;
DELETE FROM Film;
DBCC CHECKIDENT ('Film', RESEED, 0);
DBCC CHECKIDENT ('Actor', RESEED, 0);

INSERT INTO Film (Title, Director, Origin, ReleaseYear) VALUES
('The Godfather', 'Francis Ford Coppola', 'USA', 1972),
('Pulp Fiction', 'Quentin Tarantino', 'USA', 1994);

INSERT INTO Actor (Name, BirthYear, Country) VALUES
('Marlon Brando', 1924, 'USA'),
('Al Pacino', 1940, 'USA'),
('John Travolta', 1954, 'USA');

INSERT INTO FilmActor (IDFilm, IDActor) VALUES
(1, 1), (1, 2), (2, 3);
GO

-- ==============================================
-- 1. DIRTY READ TEST - CONNECTION 1
-- ==============================================
PRINT '=== 1. DIRTY READ TEST - CONNECTION 1 ===';
BEGIN TRANSACTION;
    PRINT '1. Making uncommitted change to The Godfather...';
    UPDATE Film SET Title = 'The Godfather Part IV' WHERE IDFilm = 1;

    PRINT '1. Changes made but NOT committed. Run Connection 2 now.';
    PRINT '1. This transaction will automatically rollback after 10 seconds.';

    -- Wait for Connection 2 to perform read
    WAITFOR DELAY '00:00:10';

    -- Rollback to keep database clean
    ROLLBACK TRANSACTION;
    PRINT '1. Transaction rolled back. Database is clean.';
GO

-- ==============================================
-- 2. NON-REPEATABLE READ TEST - CONNECTION 1
-- ==============================================
PRINT '=== 2. NON-REPEATABLE READ TEST - CONNECTION 1 ===';
-- 2.1 Problem demonstration
PRINT '2.1 Non-repeatable read without isolation level change:';
BEGIN TRANSACTION;
    PRINT 'First read:';
    SELECT * FROM Actor WHERE Name = 'Al Pacino';
    WAITFOR DELAY '00:00:05'; -- Run update in Connection 2 now
    PRINT 'Second read:';
    SELECT * FROM Actor WHERE Name = 'Al Pacino';
COMMIT;
GO

-- 2.2 Solution using REPEATABLE READ
PRINT '2.2 Solved with REPEATABLE READ isolation level:';
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
BEGIN TRANSACTION;
    PRINT 'First read:';
    SELECT * FROM Actor WHERE Name = 'Al Pacino';
    WAITFOR DELAY '00:00:05'; -- Try to update in Connection 2
    PRINT 'Second read:';
    SELECT * FROM Actor WHERE Name = 'Al Pacino';
COMMIT;
-- Reset to default isolation level
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
GO

-- ==============================================
-- 3. PHANTOM READ TEST - CONNECTION 1
-- ==============================================
PRINT '=== 3. PHANTOM READ TEST - CONNECTION 1 ===';
-- 3.1 Problem demonstration
PRINT '3.1 Phantom read without SERIALIZABLE:';
BEGIN TRANSACTION;
    PRINT 'First read:';
    SELECT * FROM Film WHERE ReleaseYear > 2000;
    WAITFOR DELAY '00:00:05'; -- Insert in Connection 2 during this time
    PRINT 'Second read:';
    SELECT * FROM Film WHERE ReleaseYear > 2000;
COMMIT;
GO

-- 3.2 Solution using SERIALIZABLE
PRINT '3.2 Solved with SERIALIZABLE isolation level:';
SET TRANSACTION ISOLATION LEVEL SERIALIZABLE;
BEGIN TRANSACTION;
    PRINT 'First read:';
    SELECT * FROM Film WHERE ReleaseYear > 2000;
    WAITFOR DELAY '00:00:05'; -- Insert in Connection 2 should be blocked
    PRINT 'Second read:';
    SELECT * FROM Film WHERE ReleaseYear > 2000;
COMMIT;
-- Reset to default
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
GO

-- ==============================================
-- 4. DEADLOCK TEST - CONNECTION 1
-- ==============================================
PRINT '=== 4. DEADLOCK TEST - CONNECTION 1 ===';
-- 4.1 Problem demonstration
PRINT '4.1 Deadlock situation:';
BEGIN TRY
    BEGIN TRANSACTION;
        UPDATE Film SET Director = 'Coppola' WHERE IDFilm = 1;
        WAITFOR DELAY '00:00:03'; -- Run Connection 2 now
        UPDATE Actor SET Name = 'Brando Jr.' WHERE IDActor = 1;
    COMMIT;
END TRY
BEGIN CATCH
    IF ERROR_NUMBER() = 1205
        PRINT '4.1 Deadlock occurred: ' + ERROR_MESSAGE();
    IF @@TRANCOUNT > 0 ROLLBACK;
END CATCH;
GO

-- 4.2 Solution using consistent access order
PRINT '4.2 Solved with consistent locking order (Actor → Film):';
BEGIN TRY
    BEGIN TRANSACTION;
		UPDATE Actor SET Name = 'Brando Jr.' WHERE IDActor = 1;
		WAITFOR DELAY '00:00:03'; -- Run Connection 2 now
        UPDATE Film SET Director = 'Coppola' WHERE IDFilm = 1;
    COMMIT;
    PRINT '4.2 No deadlock occurred with consistent access order.';
END TRY
BEGIN CATCH
    PRINT 'Error: ' + ERROR_MESSAGE();
    IF @@TRANCOUNT > 0 ROLLBACK;
END CATCH;
GO

PRINT '=== CONNECTION 1 TESTS COMPLETE ===';
