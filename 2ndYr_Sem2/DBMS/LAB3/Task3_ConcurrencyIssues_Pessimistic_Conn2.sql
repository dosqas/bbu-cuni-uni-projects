USE MinimalCinemaDB;

-- ==============================================
-- 1. DIRTY READ DEMONSTRATION (Connection 2)
-- ==============================================
PRINT '=== DIRTY READ TEST - CONNECTION 2 ===';
-- First show dirty read (uncommitted data)
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
PRINT 'Connection 2: Reading with READ UNCOMMITTED (dirty read):';
SELECT * FROM Film WHERE IDFilm = 1;

-- Then show proper read (only committed data)
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
PRINT 'Connection 2: Reading with READ COMMITTED (no dirty read):';
SELECT * FROM Film WHERE IDFilm = 1;

-- ==============================================
-- 2. NON-REPEATABLE READ DEMONSTRATION (Connection 2)
-- ==============================================
PRINT '=== NON-REPEATABLE READ TEST - CONNECTION 2 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 2: Updating Al Pacino''s birth year';
    UPDATE Actor SET BirthYear = 1941 WHERE Name = 'Al Pacino';
COMMIT;
PRINT 'Connection 2: Update committed. Check Connection 1 results.';

-- ==============================================
-- 3. PHANTOM READ DEMONSTRATION (Connection 2)
-- ==============================================
PRINT '=== PHANTOM READ TEST - CONNECTION 2 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 2: Inserting new film from 2010';
    INSERT INTO Film (Title, Director, Origin, ReleaseYear)
    VALUES ('Inception', 'Christopher Nolan', 'USA', 2010);
COMMIT;
PRINT 'Connection 2: Insert committed. Check Connection 1 results.';

-- ==============================================
-- 4. DEADLOCK DEMONSTRATION (Connection 2)
-- ==============================================
PRINT '=== DEADLOCK TEST - CONNECTION 2 ===';
BEGIN TRANSACTION;
    PRINT 'Connection 2: Updating Marlon Brando...';
    UPDATE Actor SET BirthYear = 1924 WHERE Name = 'Marlon Brando';
    
    PRINT 'Connection 2: Waiting 3 seconds for Connection 1...';
    WAITFOR DELAY '00:00:03';
    
    PRINT 'Connection 2: Updating The Godfather...';
    UPDATE Film SET ReleaseYear = 1972 WHERE Title = 'The Godfather';
COMMIT;
PRINT 'Connection 2: Deadlock test completed (if no errors)';

PRINT 'Connection 2: All tests executed.';