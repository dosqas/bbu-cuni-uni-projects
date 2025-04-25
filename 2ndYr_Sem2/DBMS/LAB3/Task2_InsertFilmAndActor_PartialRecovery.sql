-- Create the user-defined table type
CREATE TYPE ActorTableType AS TABLE
(
    Name VARCHAR(100),
    BirthYear INT,
    Country VARCHAR(30)
);

-- Procedure to insert data into Film, Actor, and FilmActor with rollback
CREATE PROCEDURE InsertFilmAndActorsWithPartialRecovery
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
    
    -- Declare variables for tracking success/failure
    DECLARE @ActorsInserted BIT = 0;
    DECLARE @FilmInserted BIT = 0;
    DECLARE @RelationshipsInserted BIT = 0;
    DECLARE @IDFilm INT = NULL;
    DECLARE @ErrorOccurred BIT = 0;
    DECLARE @ErrorMessage NVARCHAR(4000);
    
    -- Declare table variable to store actor IDs
    DECLARE @ActorIDs TABLE (ActorName VARCHAR(100), ActorID INT);
    DECLARE @NewActorCount INT = 0;
    DECLARE @ExistingActorCount INT = 0;
    
    -- STEP 1: Process actors first (these will be kept even if the film fails)
    BEGIN TRY
        -- Insert new actors that don't already exist and get their IDs
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
            SET @ActorsInserted = 1;
        END
        
        -- Get IDs for existing actors
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
    END TRY
    BEGIN CATCH
        SET @ErrorOccurred = 1;
        SET @ErrorMessage = ERROR_MESSAGE();
        
        -- Log error details
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'ERROR', 'Actor', 'FAILED', 
                'Error inserting actors: ' + @ErrorMessage + ', Line: ' + CAST(ERROR_LINE() AS VARCHAR) +
                ', Number: ' + CAST(ERROR_NUMBER() AS VARCHAR));
    END CATCH
    
    -- STEP 2: Now try to insert the film (separate try/catch to keep actors even if film fails)
    IF @ErrorOccurred = 0 OR @ActorsInserted = 1
    BEGIN
        BEGIN TRY
            -- Check if the film title already exists
            IF EXISTS (SELECT 1 FROM Film WHERE Title = @Title)
            BEGIN
                INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
                VALUES (@ProcName, 'VALIDATION', 'Film.Title', 'FAILED', 'Film title already exists: ' + @Title);
                
                SET @ErrorOccurred = 1;
                SET @ErrorMessage = 'A film with this title already exists';
            END
            ELSE
            BEGIN
                -- Insert into Film table
                INSERT INTO Film (Title, Director, Origin, ReleaseYear)
                VALUES (@Title, @Director, @Origin, @ReleaseYear);
                
                SET @IDFilm = SCOPE_IDENTITY();
                SET @FilmInserted = 1;
                
                -- Log film insertion
                INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message, AffectedRows)
                VALUES (@ProcName, 'INSERT', 'Film', 'SUCCESS', 'Film inserted: ' + @Title, 1);
            END
        END TRY
        BEGIN CATCH
            SET @ErrorOccurred = 1;
            SET @ErrorMessage = ERROR_MESSAGE();
            
            -- Log error details
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'ERROR', 'Film', 'FAILED', 
                    'Error inserting film: ' + @ErrorMessage + ', Line: ' + CAST(ERROR_LINE() AS VARCHAR) +
                    ', Number: ' + CAST(ERROR_NUMBER() AS VARCHAR));
        END CATCH
    END
    
    -- STEP 3: Only try to create relationships if both film and actors are available
    IF @FilmInserted = 1 AND (@NewActorCount > 0 OR @ExistingActorCount > 0)
    BEGIN
        BEGIN TRY
            -- Insert relationships into FilmActor table
            DECLARE @RelationshipCount INT = 0;
            
            INSERT INTO FilmActor (IDFilm, IDActor)
            SELECT @IDFilm, ActorID
            FROM @ActorIDs;
            
            SET @RelationshipCount = @@ROWCOUNT;
            SET @RelationshipsInserted = 1;
            
            -- Log relationship insertions
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message, AffectedRows)
            VALUES (@ProcName, 'INSERT', 'FilmActor', 'SUCCESS', 'Film-Actor relationships inserted', @RelationshipCount);
        END TRY
        BEGIN CATCH
            SET @ErrorOccurred = 1;
            SET @ErrorMessage = ERROR_MESSAGE();
            
            -- Log error details
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'ERROR', 'FilmActor', 'FAILED', 
                    'Error inserting relationships: ' + @ErrorMessage + ', Line: ' + CAST(ERROR_LINE() AS VARCHAR) +
                    ', Number: ' + CAST(ERROR_NUMBER() AS VARCHAR));
        END CATCH
    END
    
    -- STEP 4: Report final status
    IF @ErrorOccurred = 1
    BEGIN
        -- Report partial success if applicable
        IF @ActorsInserted = 1 AND @FilmInserted = 0
        BEGIN
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'PROCEDURE', 'N/A', 'PARTIAL', 
                    'Procedure completed with partial success: Actors were inserted but Film failed');
            
            -- Return information about partial success
            RAISERROR('Operation partially successful: Actors were inserted but Film insertion failed: %s', 
                      10, -- Severity 10 for informational message
                      1,  -- State
                      @ErrorMessage);
        END
        ELSE IF @ActorsInserted = 1 AND @FilmInserted = 1 AND @RelationshipsInserted = 0
        BEGIN
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'PROCEDURE', 'N/A', 'PARTIAL', 
                    'Procedure completed with partial success: Actors and Film were inserted but relationships failed');
            
            -- Return information about partial success
            RAISERROR('Operation partially successful: Actors and Film were inserted but relationships failed: %s', 
                      10, -- Severity 10 for informational message
                      1,  -- State
                      @ErrorMessage);
        END
        ELSE
        BEGIN
            INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
            VALUES (@ProcName, 'PROCEDURE', 'N/A', 'FAILED', 'Procedure execution failed');
            
            -- Re-throw the error with additional information if nothing was inserted
            RAISERROR(@ErrorMessage, 16, 1);
        END
    END
    ELSE
    BEGIN
        -- Full success
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES (@ProcName, 'PROCEDURE', 'N/A', 'SUCCESS', 'Procedure executed successfully');
    END
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
    EXEC InsertFilmAndActorsWithPartialRecovery 
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
VALUES ('TestScript', 'TEST', 'Test Case 2', 'IN PROGRESS', 'Starting partial recovery test (duplicate title)');

-- TEST CASE 2: Partial Recovery - Film insertion fails but actors may be preserved
-- First, create an actor that will already exist in the test
INSERT INTO Actor (Name, BirthYear, Country)
VALUES ('Leonardo DiCaprio', 1974, 'USA');

-- Count rows before test
DECLARE @FilmCountBefore INT, @ActorCountBefore INT, @FilmActorCountBefore INT;
SELECT @FilmCountBefore = COUNT(*) FROM Film;
SELECT @ActorCountBefore = COUNT(*) FROM Actor;
SELECT @FilmActorCountBefore = COUNT(*) FROM FilmActor;

PRINT 'Before partial recovery test: Films=' + CAST(@FilmCountBefore AS VARCHAR) + 
      ', Actors=' + CAST(@ActorCountBefore AS VARCHAR) + 
      ', FilmActor=' + CAST(@FilmActorCountBefore AS VARCHAR);

-- Log counts before test
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'COUNT', 'Tables', 'INFO', 
        'Before partial recovery test: Films=' + CAST(@FilmCountBefore AS VARCHAR) + 
        ', Actors=' + CAST(@ActorCountBefore AS VARCHAR) + 
        ', FilmActor=' + CAST(@FilmActorCountBefore AS VARCHAR));

-- Create a film that will cause a conflict
INSERT INTO Film (Title, Director, Origin, ReleaseYear)
VALUES ('Duplicate Film', 'Some Director', 'USA', 2020);

-- Now try to insert the same film title with new and existing actors
INSERT INTO @TestActors (Name, BirthYear, Country)
VALUES ('Leonardo DiCaprio', 1974, 'USA'),  -- Already exists
       ('Kate Winslet', 1975, 'UK');        -- New actor

-- Execute procedure with duplicate title - should fail film insertion but preserve actors
BEGIN TRY
    PRINT 'Running Test Case 2: Partial Recovery (Duplicate Title)';
    
    -- This will generate a message but not throw an error that TRY/CATCH can catch
    EXEC InsertFilmAndActorsWithPartialRecovery 
        @Title = 'Duplicate Film', -- This will conflict with existing title
        @Director = 'James Cameron',
        @Origin = 'USA',
        @ReleaseYear = 2022,
        @Actors = @TestActors;
    
    -- If we get here, check if the procedure actually succeeded or just returned a message
    DECLARE @FilmCountCheck INT = (SELECT COUNT(*) FROM Film WHERE Title = 'Duplicate Film' AND Director = 'James Cameron');
    
    IF @FilmCountCheck > 0
    BEGIN
        PRINT 'Test Case 2 unexpectedly succeeded - Film was actually inserted';
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES ('TestScript', 'TEST', 'Test Case 2', 'FAILED', 'Film was inserted despite duplicate title');
    END
    ELSE
    BEGIN
        PRINT 'Test Case 2 partially succeeded - Film was rejected but actors may have been preserved';
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES ('TestScript', 'TEST', 'Test Case 2', 'SUCCESS', 'Partial recovery worked as expected');
    END
END TRY
BEGIN CATCH
    PRINT 'Test Case 2 failed with error: ' + ERROR_MESSAGE();
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'TEST', 'Test Case 2', 'FAILED', 'Unexpected error: ' + ERROR_MESSAGE());
END CATCH

-- Verify partial recovery - film count should be same, actors may have increased
DECLARE @FilmCountAfter INT, @ActorCountAfter INT, @FilmActorCountAfter INT;
SELECT @FilmCountAfter = COUNT(*) FROM Film;
SELECT @ActorCountAfter = COUNT(*) FROM Actor;
SELECT @FilmActorCountAfter = COUNT(*) FROM FilmActor;

PRINT 'After partial recovery test: Films=' + CAST(@FilmCountAfter AS VARCHAR) + 
      ', Actors=' + CAST(@ActorCountAfter AS VARCHAR) + 
      ', FilmActor=' + CAST(@FilmActorCountAfter AS VARCHAR);

-- Log counts after test
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'COUNT', 'Tables', 'INFO', 
        'After partial recovery test: Films=' + CAST(@FilmCountAfter AS VARCHAR) + 
        ', Actors=' + CAST(@ActorCountAfter AS VARCHAR) + 
        ', FilmActor=' + CAST(@FilmActorCountAfter AS VARCHAR));

-- Check if partial recovery worked
IF (@FilmCountAfter = @FilmCountBefore + 1) -- +1 for the film we manually inserted
BEGIN
    PRINT 'FILM INSERTION ROLLED BACK - Film count unchanged (except our manual insert)';
    
    -- Check if new actors were preserved
    IF (@ActorCountAfter > @ActorCountBefore)
    BEGIN
        PRINT 'PARTIAL RECOVERY SUCCESSFUL - New actors were preserved';
        
        -- Log partial recovery success
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES ('TestScript', 'PARTIAL RECOVERY', 'Actor', 'SUCCESS', 
                'New actors preserved while film insertion rolled back. Actor count increased by ' + 
                CAST(@ActorCountAfter - @ActorCountBefore AS VARCHAR));
    END
    ELSE
    BEGIN
        PRINT 'PARTIAL RECOVERY FAILED - No new actors were inserted';
        
        -- Log partial recovery failure
        INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
        VALUES ('TestScript', 'PARTIAL RECOVERY', 'Actor', 'FAILED', 
                'No actors were preserved during partial recovery');
    END
END
ELSE
BEGIN
    PRINT 'PARTIAL RECOVERY FAILED - Film count changed unexpectedly';
    
    -- Log partial recovery failure
    INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
    VALUES ('TestScript', 'PARTIAL RECOVERY', 'All Tables', 'FAILED', 
            'Unexpected changes: Films=' + 
            CAST(@FilmCountAfter - @FilmCountBefore AS VARCHAR) + 
            ', Actors=' + CAST(@ActorCountAfter - @ActorCountBefore AS VARCHAR) + 
            ', FilmActor=' + CAST(@FilmActorCountAfter - @FilmActorCountBefore AS VARCHAR));
END

-- Clear test data for next test
DELETE FROM @TestActors;



-- Log test case start
INSERT INTO ActionLog (ProcedureName, ActionType, ObjectName, Status, Message)
VALUES ('TestScript', 'TEST', 'Test Case 3', 'IN PROGRESS', 'Starting validation test (empty title)');

-- TEST CASE 3: Parameter validation failure - should fail before any inserts
BEGIN TRY
    PRINT 'Running Test Case 3: Empty Title Validation';
    EXEC InsertFilmAndActorsWithPartialRecovery 
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
    
    -- Verify no data was inserted
    DECLARE @FilmCountFinal INT, @ActorCountFinal INT;
    SELECT @FilmCountFinal = COUNT(*) FROM Film;
    SELECT @ActorCountFinal = COUNT(*) FROM Actor;
    
    IF (@FilmCountFinal = @FilmCountBefore + 1 AND @ActorCountFinal = @ActorCountAfter)
    BEGIN
        PRINT 'VALIDATION WORKED - No new data inserted for invalid parameters';
    END
    ELSE
    BEGIN
        PRINT 'VALIDATION FAILED - Data was inserted despite invalid parameters';
    END
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