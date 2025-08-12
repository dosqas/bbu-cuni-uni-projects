USE MinimalCinemaDB;
GO
SET NOCOUNT ON;
-- ==============================================
-- 1. DIRTY READ TEST (Connection 2)
-- ==============================================
PRINT '=== 1. DIRTY READ TEST - CONNECTION 2 ===';

-- First show the problem
PRINT '1.1 Problem: Reading uncommitted data (READ UNCOMMITTED)';
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
PRINT 'Results with dirty reads enabled:';
SELECT IDFilm, Title, Director, Origin, ReleaseYear FROM Film WHERE IDFilm = 1;
GO

-- Then show the solution
PRINT '1.2 Solution: Preventing dirty reads (READ COMMITTED)';
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;
PRINT 'Results with dirty reads prevented:';
SELECT IDFilm, Title, Director, Origin, ReleaseYear FROM Film WHERE IDFilm = 1;
GO

-- ==============================================
-- 2. NON-REPEATABLE READ TEST (Connection 2)
-- ==============================================
PRINT '=== 2. NON-REPEATABLE READ TEST - CONNECTION 2 ===';
PRINT '2.1 Making changes between Connection 1 reads:';
BEGIN TRANSACTION;
    UPDATE Actor SET BirthYear = 1941 WHERE Name = 'Al Pacino';
COMMIT;
PRINT '2.1 Update committed. Check Connection 1 results.';
GO

-- ==============================================
-- 3. PHANTOM READ TEST (Connection 2)
-- ==============================================
PRINT '=== 3. PHANTOM READ TEST - CONNECTION 2 ===';
PRINT '3.1 Inserting new row between Connection 1 reads:';
BEGIN TRANSACTION;
    INSERT INTO Film (Title, Director, Origin, ReleaseYear)
    VALUES ('Inception', 'Christopher Nolan', 'USA', 2010);
COMMIT;
PRINT '3.1 Insert committed. Check Connection 1 results.';
GO

-- ==============================================
-- 4. DEADLOCK TEST (Connection 2)
-- ==============================================
PRINT '=== 4. DEADLOCK TEST - CONNECTION 2 ===';
PRINT '4.1 Creating deadlock condition:';
BEGIN TRY
    BEGIN TRANSACTION;
        UPDATE Actor SET BirthYear = 1924 WHERE IDActor = 1;
        WAITFOR DELAY '00:00:03'; -- This should match the wait in Connection 1
        UPDATE Film SET ReleaseYear = 1972 WHERE IDFilm = 1;
    COMMIT;
END TRY
BEGIN CATCH
    IF ERROR_NUMBER() = 1205
        PRINT '4.1 Deadlock occurred: ' + ERROR_MESSAGE();
    IF @@TRANCOUNT > 0 ROLLBACK;
END CATCH;
GO

PRINT '=== CONNECTION 2 TESTS COMPLETE ===';
