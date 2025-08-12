ALTER DATABASE MinimalCinemaDB SET ALLOW_SNAPSHOT_ISOLATION ON;
ALTER DATABASE MinimalCinemaDB SET READ_COMMITTED_SNAPSHOT OFF; -- optional

USE MinimalCinemaDB;
GO

PRINT '=== SNAPSHOT ISOLATION TEST - CONNECTION 1 ===';

SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRANSACTION;

-- 1. Read a row
SELECT * FROM Film WHERE IDFilm = 1;

-- 2. Wait so Connection 2 can read and update
WAITFOR DELAY '00:00:10';

-- 3. Try to update (will fail if row was changed since read)
UPDATE Film SET Title = 'The Godfather: Remastered' WHERE IDFilm = 1;

COMMIT;
GO