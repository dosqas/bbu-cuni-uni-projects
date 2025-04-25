-- Create the user-defined table type
CREATE TYPE ActorTableType AS TABLE
(
    Name VARCHAR(100),
    BirthYear INT,
    Country VARCHAR(30)
);

-- Procedure to insert data into Film, Actor, and FilmActor with rollback
CREATE PROCEDURE InsertFilmAndActorsWithRollback
    @Title VARCHAR(50),
    @Director VARCHAR(50),
    @Origin VARCHAR(30),
    @ReleaseYear INT,
    @Actors ActorTableType READONLY
AS
BEGIN
    DECLARE @ProcName NVARCHAR(128) = OBJECT_NAME(@@PROCID);
    
    -- Log procedure start
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES (@ProcName, 'PROCEDURE', 'N/A', 'IN PROGRESS', 'Starting procedure execution');
    
    -- Parameter validation
    IF @Title IS NULL OR LEN(TRIM(@Title)) = 0
    BEGIN
        -- Log validation failure
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'VALIDATION', 'Film.Title', 'FAILED', 'Film title cannot be null or empty');
        
        THROW 50001, 'Film title cannot be null or empty', 1;
        RETURN;
    END
    
    -- Additional validations with logging...
    IF @Director IS NULL OR LEN(TRIM(@Director)) = 0
    BEGIN
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'VALIDATION', 'Film.Director', 'FAILED', 'Director name cannot be null or empty');
        
        THROW 50002, 'Director name cannot be null or empty', 1;
        RETURN;
    END
    
    -- Check if actors collection is empty
    IF NOT EXISTS (SELECT 1 FROM @Actors)
    BEGIN
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'VALIDATION', 'Actors', 'FAILED', 'At least one actor must be provided');
        
        THROW 50007, 'At least one actor must be provided', 1;
        RETURN;
    END
    
    -- Start a transaction
    BEGIN TRY
        -- Log transaction start
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'TRANSACTION', 'N/A', 'IN PROGRESS', 'Beginning transaction');
        
        BEGIN TRANSACTION;
        
        -- Check if the film title already exists
        IF EXISTS (SELECT 1 FROM Film WHERE Title = @Title)
        BEGIN
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'VALIDATION', 'Film.Title', 'FAILED', 'Film title already exists: ' + @Title);
            
            THROW 50006, 'A film with this title already exists', 1;
        END
        
        -- Insert into Film table
        DECLARE @IDFilm INT;
        INSERT INTO Film (Title, Director, Origin, ReleaseYear)
        VALUES (@Title, @Director, @Origin, @ReleaseYear);
        
        SET @IDFilm = SCOPE_IDENTITY();
        
        -- Log film insertion
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message, AffectedRows)
        VALUES (@ProcName, 'INSERT', 'Film', 'SUCCESS', 'Film inserted: ' + @Title, 1);
        
        -- Declare table variable to store actor IDs
        DECLARE @ActorIDs TABLE (ActorName VARCHAR(100), ActorID INT);
        DECLARE @NewActorCount INT = 0;
        
        -- Insert actors that don't already exist and get their IDs
        INSERT INTO Actor (Name, BirthYear, Country)
        OUTPUT INSERTED.Name, INSERTED.IDActor INTO @ActorIDs
        SELECT a.Name, a.BirthYear, a.Country 
        FROM @Actors a
        LEFT JOIN Actor e ON a.Name = e.Name AND a.BirthYear = e.BirthYear
        WHERE e.IDActor IS NULL;
        
        SET @NewActorCount = @@ROWCOUNT;
        
        -- Log actor insertions
        IF @NewActorCount > 0
        BEGIN
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message, AffectedRows)
            VALUES (@ProcName, 'INSERT', 'Actor', 'SUCCESS', 'New actors inserted', @NewActorCount);
        END
        
        -- Get IDs for existing actors
        DECLARE @ExistingActorCount INT = 0;
        
        INSERT INTO @ActorIDs (ActorName, ActorID)
        SELECT a.Name, e.IDActor
        FROM @Actors a
        JOIN Actor e ON a.Name = e.Name AND a.BirthYear = e.BirthYear
        WHERE NOT EXISTS (SELECT 1 FROM @ActorIDs WHERE ActorName = a.Name);
        
        SET @ExistingActorCount = @@ROWCOUNT;
        
        -- Log existing actors
        IF @ExistingActorCount > 0
        BEGIN
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message, AffectedRows)
            VALUES (@ProcName, 'REFERENCE', 'Actor', 'SUCCESS', 'Existing actors referenced', @ExistingActorCount);
        END
        
        -- Insert relationships into FilmActor table
        DECLARE @RelationshipCount INT = 0;
        
        INSERT INTO FilmActor (IDFilm, IDActor)
        SELECT @IDFilm, ActorID
        FROM @ActorIDs;
        
        SET @RelationshipCount = @@ROWCOUNT;
        
        -- Log relationship insertions
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message, AffectedRows)
        VALUES (@ProcName, 'INSERT', 'FilmActor', 'SUCCESS', 'Film-Actor relationships inserted', @RelationshipCount);
        
        -- Commit the transaction if all inserts are successful
        COMMIT TRANSACTION;
        
        -- Log transaction commit
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'TRANSACTION', 'N/A', 'SUCCESS', 'Transaction committed');
        
        -- Log procedure completion
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'PROCEDURE', 'N/A', 'SUCCESS', 'Procedure executed successfully');
    END TRY
    BEGIN CATCH
        -- Log error details
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'ERROR', ERROR_PROCEDURE(), 'FAILED', 
                'Error: ' + ERROR_MESSAGE() + ', Line: ' + CAST(ERROR_LINE() AS VARCHAR) +
                ', Number: ' + CAST(ERROR_NUMBER() AS VARCHAR));
        
        -- Rollback the transaction if any error occurs
        IF @@TRANCOUNT > 0
        BEGIN
            ROLLBACK TRANSACTION;
            
            -- Log transaction rollback
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'TRANSACTION', 'N/A', 'FAILED', 'Transaction rolled back');
        END
        
        -- Re-throw the error with additional information
        DECLARE @ErrorMessage NVARCHAR(4000) = ERROR_MESSAGE();
        DECLARE @ErrorSeverity INT = ERROR_SEVERITY();
        DECLARE @ErrorState INT = ERROR_STATE();
        
        RAISERROR(@ErrorMessage, @ErrorSeverity, @ErrorState);
        
        -- Log procedure failure
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'PROCEDURE', 'N/A', 'FAILED', 'Procedure execution failed');
    END CATCH
END;




-- Clear logs for fresh testing
TRUNCATE TABLE ActionLog;

-- Record start time for this test run
DECLARE @TestStartTime DATETIME = GETDATE();
PRINT 'Starting test run at: ' + CONVERT(VARCHAR, @TestStartTime, 120);

-- Clear any existing test data first
DELETE FROM FilmActor;
DELETE FROM Actor;
DELETE FROM Film;

-- Reset identity values if needed
DBCC CHECKIDENT ('Film', RESEED, 0);
DBCC CHECKIDENT ('Actor', RESEED, 0);

-- Create a temporary table for testing
DECLARE @TestActors AS ActorTableType;



-- Log test case start
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'TEST', 'Test Case 1', 'IN PROGRESS', 'Starting happy path test');

-- TEST CASE 1: Happy Path - Everything should succeed
INSERT INTO @TestActors (Name, BirthYear, Country)
VALUES ('Tom Hanks', 1956, 'USA'), 
       ('Meryl Streep', 1949, 'USA');

-- Execute procedure
BEGIN TRY
    PRINT 'Running Test Case 1: Happy Path';
    EXEC InsertFilmAndActorsWithRollback 
        @Title = 'The Post',
        @Director = 'Steven Spielberg',
        @Origin = 'USA',
        @ReleaseYear = 2017,
        @Actors = @TestActors;
    
    PRINT 'Test Case 1 succeeded';
    
    -- Log test case success
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 1', 'SUCCESS', 'Happy path test completed successfully');
    
    -- Verify data was inserted
    SELECT 'Film Table After Success:' AS Message;
    SELECT * FROM Film;
    
    SELECT 'Actor Table After Success:' AS Message;
    SELECT * FROM Actor;
    
    SELECT 'FilmActor Table After Success:' AS Message;
    SELECT * FROM FilmActor;
END TRY
BEGIN CATCH
    PRINT 'Test Case 1 failed: ' + ERROR_MESSAGE();
    
    -- Log test case failure
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 1', 'FAILED', 'Error: ' + ERROR_MESSAGE());
END CATCH

-- Clear test data for next test
DELETE FROM @TestActors;



-- Log test case start
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'TEST', 'Test Case 2', 'IN PROGRESS', 'Starting rollback test (duplicate title)');

-- TEST CASE 2: Failure Case - Should trigger rollback
-- First, insert a film with unique title constraint that will conflict
INSERT INTO Film (Title, Director, Origin, ReleaseYear)
VALUES ('Duplicate Film', 'Some Director', 'USA', 2020);

-- Log manual film insertion
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'INSERT', 'Film', 'SUCCESS', 'Created film with title "Duplicate Film" for rollback test');

-- Now try to insert the same film title with new actors
INSERT INTO @TestActors (Name, BirthYear, Country)
VALUES ('Leonardo DiCaprio', 1974, 'USA'), 
       ('Kate Winslet', 1975, 'UK');

-- Count rows before failure test
DECLARE @FilmCountBefore INT, @ActorCountBefore INT, @FilmActorCountBefore INT;
SELECT @FilmCountBefore = COUNT(*) FROM Film;
SELECT @ActorCountBefore = COUNT(*) FROM Actor;
SELECT @FilmActorCountBefore = COUNT(*) FROM FilmActor;

PRINT 'Before failure test: Films=' + CAST(@FilmCountBefore AS VARCHAR) + 
      ', Actors=' + CAST(@ActorCountBefore AS VARCHAR) + 
      ', FilmActor=' + CAST(@FilmActorCountBefore AS VARCHAR);

-- Log counts before test
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'COUNT', 'Tables', 'INFO', 
        'Before failure test: Films=' + CAST(@FilmCountBefore AS VARCHAR) + 
        ', Actors=' + CAST(@ActorCountBefore AS VARCHAR) + 
        ', FilmActor=' + CAST(@FilmActorCountBefore AS VARCHAR));

-- Execute procedure with duplicate title - should fail
BEGIN TRY
    PRINT 'Running Test Case 2: Failure Case (Duplicate Title)';
    EXEC InsertFilmAndActorsWithRollback 
        @Title = 'Duplicate Film', -- This will conflict with existing title
        @Director = 'James Cameron',
        @Origin = 'USA',
        @ReleaseYear = 2022,
        @Actors = @TestActors;
    
    PRINT 'Test Case 2 unexpectedly succeeded';
    
    -- Log unexpected success
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 2', 'WARNING', 'Test case unexpectedly succeeded but should have failed');
END TRY
BEGIN CATCH
    PRINT 'Test Case 2 failed as expected: ' + ERROR_MESSAGE();
    
    -- Log expected failure
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 2', 'SUCCESS', 'Test failed as expected with error: ' + ERROR_MESSAGE());
    
    -- Verify rollback - counts should match before counts
    DECLARE @FilmCountAfter INT, @ActorCountAfter INT, @FilmActorCountAfter INT;
    SELECT @FilmCountAfter = COUNT(*) FROM Film;
    SELECT @ActorCountAfter = COUNT(*) FROM Actor;
    SELECT @FilmActorCountAfter = COUNT(*) FROM FilmActor;
    
    PRINT 'After failure test: Films=' + CAST(@FilmCountAfter AS VARCHAR) + 
          ', Actors=' + CAST(@ActorCountAfter AS VARCHAR) + 
          ', FilmActor=' + CAST(@FilmActorCountAfter AS VARCHAR);
    
    -- Log counts after test
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'COUNT', 'Tables', 'INFO', 
            'After failure test: Films=' + CAST(@FilmCountAfter AS VARCHAR) + 
            ', Actors=' + CAST(@ActorCountAfter AS VARCHAR) + 
            ', FilmActor=' + CAST(@FilmActorCountAfter AS VARCHAR));
    
    -- Check if rollback worked
    IF (@FilmCountAfter = @FilmCountBefore AND 
        @ActorCountAfter = @ActorCountBefore AND 
        @FilmActorCountAfter = @FilmActorCountBefore)
    BEGIN
        PRINT 'ROLLBACK SUCCESSFUL - No data was inserted';
        
        -- Log rollback success
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES ('TestScript', 'ROLLBACK', 'All Tables', 'SUCCESS', 'Transaction rolled back successfully');
    END
    ELSE
    BEGIN
        PRINT 'ROLLBACK FAILED - Some data remained';
        
        -- Log rollback failure
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES ('TestScript', 'ROLLBACK', 'All Tables', 'FAILED', 
                'Rollback incomplete - data counts changed: Films=' + 
                CAST(@FilmCountAfter - @FilmCountBefore AS VARCHAR) + 
                ', Actors=' + CAST(@ActorCountAfter - @ActorCountBefore AS VARCHAR) + 
                ', FilmActor=' + CAST(@FilmActorCountAfter - @FilmActorCountBefore AS VARCHAR));
    END
END CATCH



-- Log test case start
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'TEST', 'Test Case 3', 'IN PROGRESS', 'Starting validation test (empty title)');

-- TEST CASE 3: Parameter validation failure
DELETE FROM @TestActors;

-- Try with empty title
BEGIN TRY
    PRINT 'Running Test Case 3: Empty Title Validation';
    EXEC InsertFilmAndActorsWithRollback 
        @Title = '',  -- Empty title should fail validation
        @Director = 'Some Director',
        @Origin = 'USA',
        @ReleaseYear = 2023,
        @Actors = @TestActors;
    
    PRINT 'Test Case 3 unexpectedly succeeded';
    
    -- Log unexpected success
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 3', 'WARNING', 'Validation test unexpectedly succeeded');
END TRY
BEGIN CATCH
    PRINT 'Test Case 3 failed as expected: ' + ERROR_MESSAGE();
    
    -- Log expected validation failure
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 3', 'SUCCESS', 'Validation test failed as expected: ' + ERROR_MESSAGE());
END CATCH



-- Final verification
SELECT 'Final Film Table:' AS Message;
SELECT * FROM Film;

SELECT 'Final Actor Table:' AS Message;
SELECT * FROM Actor;

SELECT 'Final FilmActor Table:' AS Message;
SELECT * FROM FilmActor;

-- Record end time for this test run
DECLARE @TestEndTime DATETIME = GETDATE();
PRINT 'Test run completed at: ' + CONVERT(VARCHAR, @TestEndTime, 120);
PRINT 'Total test duration: ' + 
      CAST(DATEDIFF(MILLISECOND, @TestStartTime, @TestEndTime) / 1000.0 AS VARCHAR) + ' seconds';

-- Log test completion
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'TEST', 'Test Suite', 'COMPLETED', 
        'All tests completed in ' + CAST(DATEDIFF(MILLISECOND, @TestStartTime, @TestEndTime) / 1000.0 AS VARCHAR) + ' seconds');

-- Display action logs for this test run
SELECT 'Action Log for Test Run:' AS Message;
SELECT 
    LogID,
    ProcedureName,
    ActionType,
    ObjectName,
    Status,
    Message,
    AffectedRows,
    ExecutedBy,
    ExecutedAt
FROM ActionLog
WHERE ExecutedAt >= @TestStartTime
ORDER BY LogID;
