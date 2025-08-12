USE MinimalCinemaDB;
GO

PRINT '=== SNAPSHOT ISOLATION TEST - CONNECTION 2 ===';

SET TRANSACTION ISOLATION LEVEL SNAPSHOT;
BEGIN TRANSACTION;

-- 1. Read same row
SELECT * FROM Film WHERE IDFilm = 1;

-- 2. Wait a little less than Conn1 so this commits first
WAITFOR DELAY '00:00:05';

-- 3. Update the same row
UPDATE Film SET Title = 'The Godfather (Director`s Cut)' WHERE IDFilm = 1;

COMMIT;
GO