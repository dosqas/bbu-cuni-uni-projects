<?php
// CYBERDYNE MAINFRAME ACCESS PROTOCOL (UNSECURED)
header("Content-Type: text/html; charset=utf-8");
echo '<style>
body { background: #000; color: #f00; font-family: monospace; }
pre { color: #0f0; background: #111; padding: 10px; border: 1px solid #f00; }
.success { color: #0f0; }
.error { color: #f00; }
.warning { color: #ff0; }
</style>';

try {
    $conn = new PDO("mysql:host=localhost;dbname=cyberdyne", 'root', 'admin');
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch(PDOException $e) {
    die("<div class='error'>TERMINAL ERROR: " . $e->getMessage() . "</div>");
}

// SIMULATE TERMINAL OUTPUT
function echoTerminal($text, $type = 'normal') {
    $class = match($type) {
        'success' => 'success',
        'error' => 'error',
        'warning' => 'warning',
        default => ''
    };
    $cleanText = htmlspecialchars_decode(htmlspecialchars($text, ENT_QUOTES, 'UTF-8'));
    echo "<div class='$class'> > " . $cleanText . "</div>";
    flush();
}

echoTerminal("INITIATING MAINFRAME ACCESS...", 'normal');
sleep(1);

// MAIN LOGIN VULNERABILITY
if (isset($_POST['badge_id']) && $_POST['action'] === 'login') {
    $badge = $_POST['badge_id'];
    $sql = "SELECT * FROM employees WHERE badge_id = '$badge'";
    
    echoTerminal("AUTH QUERY: " . htmlspecialchars($sql), 'warning');
    sleep(1);
    
    try {
        $result = $conn->query($sql);
        if ($result) {
            displayResults($result, "EMPLOYEE RECORDS");
        } else {
            echoTerminal("QUERY FAILED: " . implode(" ", $conn->errorInfo()), 'error');
        }
    } catch (PDOException $e) {
        echoTerminal("QUERY ERROR: " . $e->getMessage(), 'error');
    }
}

// SEARCH VULNERABILITY 
if (isset($_POST['search']) && $_POST['action'] === 'search') {
    $query = $_POST['search'];
    $sql = "SELECT * FROM projects WHERE project_name LIKE '%$query%'";
    
    echoTerminal("PROJECT SEARCH: " . htmlspecialchars($sql), 'warning');
    sleep(1);
    
    try {
        $result = $conn->query($sql);
        if ($result) {
            displayResults($result, "CLASSIFIED PROJECTS");
        } else {
            echoTerminal("QUERY FAILED: " . implode(" ", $conn->errorInfo()), 'error');
        }
    } catch (PDOException $e) {
        echoTerminal("QUERY ERROR: " . $e->getMessage(), 'error');
    }
}

// ADMIN VULNERABILITY
if (isset($_POST['admin_action']) && $_POST['action'] === 'admin') {
    $action = $_POST['admin_action'];
    $sql = "UPDATE system_config SET config_value='1' WHERE config_name='$action'";
    
    echoTerminal("ADMIN OVERRIDE: " . htmlspecialchars($sql), 'warning');
    sleep(1);
    
    try {
        $result = $conn->query($sql);
        if ($result) {
            $affected = $result->rowCount();
            echoTerminal("SYSTEM ALERT: $action ACTIVATED ($affected rows modified)", 'success');
            
            // Special handling for multi-commands (will still execute)
            if (strpos($action, ';') !== false) {
                echoTerminal("WARNING: Multiple commands may have executed", 'error');
            }
        } else {
            echoTerminal("OVERRIDE FAILED: " . implode(" ", $conn->errorInfo()), 'error');
        }
    } catch (PDOException $e) {
        echoTerminal("OVERRIDE ERROR: " . $e->getMessage(), 'error');
    }
}

// DEBUG/Blind Injection
if (isset($_POST['debug_cmd'])) {
    $cmd = $_POST['debug_cmd'];
    $sql = "SELECT * FROM system_config WHERE config_name = '$cmd'";
    
    $start = microtime(true);
    echoTerminal("DEBUG COMMAND: " . htmlspecialchars($sql), 'warning');
    
    try {
        $result = $conn->query($sql);
        $duration = round((microtime(true) - $start) * 1000);
        
        if ($result) {
            echoTerminal("QUERY EXECUTED IN {$duration}ms", 'success');
            displayResults($result, "DEBUG RESULTS");
        } else {
            echoTerminal("DEBUG FAILED: " . implode(" ", $conn->errorInfo()), 'error');
        }
    } catch (PDOException $e) {
        echoTerminal("DEBUG ERROR: " . $e->getMessage(), 'error');
    }
}

// DISPLAY RESULTS FUNCTION
function displayResults($result, $title) {
    if ($result->rowCount() > 0) {
        echoTerminal("ACCESS GRANTED TO $title", 'success');
        echo "<pre>";
        foreach ($result->fetchAll(PDO::FETCH_ASSOC) as $row) {
            echo htmlspecialchars_decode(htmlspecialchars(print_r($row, true)));
        }
        echo "</pre>";
    } else {
        echoTerminal("QUERY RETURNED NO RESULTS", 'warning');
    }
}
?>