<?php
// CYBERDYNE MAINFRAME ACCESS PROTOCOL (SECURE)
header("Content-Type: text/html; charset=utf-8");
header("X-Content-Type-Options: nosniff");
header("X-Frame-Options: DENY");

echo '<style>
body { background: #000; color: #f00; font-family: monospace; }
pre { color: #0f0; background: #111; padding: 10px; border: 1px solid #f00; }
.success { color: #0f0; }
.error { color: #f00; }
.warning { color: #ff0; }
</style>';

// Secure database connection
try {
    // In a real-world scenario, use environment variables or a secure vault for credentials
    // and ensure the database connection is secure.
    $conn = new PDO("mysql:host=localhost;dbname=cyberdyne", 'root', 'admin', [
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_EMULATE_PREPARES => false,
        PDO::MYSQL_ATTR_MULTI_STATEMENTS => false
    ]);
} catch(PDOException $e) {
    error_log("Database connection failed: " . $e->getMessage());
    die("<div class='error'>SYSTEM ERROR: Authentication services unavailable</div>");
}

// Secure terminal output
function echoTerminal($text, $type = 'normal') {
    $class = match($type) {
        'success' => 'success',
        'error' => 'error',
        'warning' => 'warning',
        default => ''
    };
    echo "<div class='$class'> > " . htmlspecialchars($text, ENT_QUOTES, 'UTF-8') . "</div>";
    flush();
}

echoTerminal("INITIATING MAINFRAME ACCESS...", 'normal');
sleep(1);

// Input validation function
function validateInput($input, $pattern, $maxLength = 100) {
    $input = trim($input);
    if (strlen($input) > $maxLength) return false;
    return preg_match($pattern, $input);
}

// SECURE LOGIN
if (isset($_POST['badge_id']) && $_POST['action'] === 'login') {
    $badge = $_POST['badge_id'];
    
    if (!validateInput($badge, '/^[A-Z0-9-]{3,10}$/')) {
        echoTerminal("INVALID BADGE ID FORMAT", 'error');
        exit;
    }

    try {
        $stmt = $conn->prepare("SELECT * FROM employees WHERE badge_id = :badge");
        $stmt->bindParam(':badge', $badge, PDO::PARAM_STR);
        $stmt->execute();
        
        echoTerminal("AUTH QUERY: SELECT * FROM employees WHERE badge_id = ?", 'warning');
        sleep(1);
        
        displayResults($stmt, "EMPLOYEE RECORDS");
    } catch(PDOException $e) {
        error_log("Login error: " . $e->getMessage());
        echoTerminal("AUTHENTICATION ERROR", 'error');
    }
}

// SECURE SEARCH
if (isset($_POST['search']) && $_POST['action'] === 'search') {
    $query = $_POST['search'];
    
    if (!validateInput($query, '/^[a-zA-Z0-9 ]{1,50}$/')) {
        echoTerminal("INVALID SEARCH QUERY", 'error');
        exit;
    }

    try {
        $stmt = $conn->prepare("SELECT * FROM projects WHERE project_name LIKE CONCAT('%', :query, '%')");
        $stmt->bindParam(':query', $query, PDO::PARAM_STR);
        $stmt->execute();
        
        echoTerminal("PROJECT SEARCH: SELECT * FROM projects WHERE project_name LIKE ?", 'warning');
        sleep(1);
        
        displayResults($stmt, "CLASSIFIED PROJECTS");
    } catch(PDOException $e) {
        error_log("Search error: " . $e->getMessage());
        echoTerminal("SEARCH ERROR", 'error');
    }
}

// SECURE ADMIN ACTIONS (WHITELIST APPROACH)
if (isset($_POST['admin_action']) && $_POST['action'] === 'admin') {
    $allowed_actions = ['disable_firewall', 'enable_root', 'wipe_logs'];
    $action = $_POST['admin_action'];
    
    if (!in_array($action, $allowed_actions)) {
        echoTerminal("UNAUTHORIZED ADMIN ACTION", 'error');
        exit;
    }

    try {
        $stmt = $conn->prepare("UPDATE system_config SET config_value='1' WHERE config_name = :action");
        $stmt->bindParam(':action', $action, PDO::PARAM_STR);
        $stmt->execute();
        
        echoTerminal("ADMIN OVERRIDE: UPDATE system_config SET config_value='1' WHERE config_name = ?", 'warning');
        sleep(1);
        
        $affected = $stmt->rowCount();
        echoTerminal("SYSTEM ALERT: $action ACTIVATED ($affected rows modified)", 'success');
    } catch(PDOException $e) {
        error_log("Admin action failed: " . $e->getMessage());
        echoTerminal("ADMIN OVERRIDE FAILED", 'error');
    }
}

// SECURE DEBUG FUNCTION (DISABLED IN PRODUCTION)
if (isset($_POST['debug_cmd']) && $_SERVER['REMOTE_ADDR'] === '127.0.0.1') {
    $allowed_commands = ['status', 'version', 'ping'];
    $cmd = $_POST['debug_cmd'];
    
    if (!in_array($cmd, $allowed_commands)) {
        echoTerminal("DEBUG COMMAND NOT ALLOWED", 'error');
        exit;
    }

    try {
        $stmt = $conn->prepare("SELECT * FROM system_config WHERE config_name = :cmd");
        $stmt->bindParam(':cmd', $cmd, PDO::PARAM_STR);
        
        $start = microtime(true);
        $stmt->execute();
        $duration = round((microtime(true) - $start) * 1000);
        
        echoTerminal("DEBUG COMMAND: SELECT * FROM system_config WHERE config_name = ?", 'warning');
        echoTerminal("QUERY EXECUTED IN {$duration}ms", 'success');
        
        displayResults($stmt, "DEBUG RESULTS");
    } catch(PDOException $e) {
        error_log("Debug error: " . $e->getMessage());
        echoTerminal("DEBUG ERROR", 'error');
    }
} elseif (isset($_POST['debug_cmd'])) {
    echoTerminal("DEBUG MODE DISABLED", 'error');
}

// SECURE RESULTS DISPLAY
function displayResults($stmt, $title) {
    $results = $stmt->fetchAll(PDO::FETCH_ASSOC);
    if (count($results) > 0) {
        echoTerminal("ACCESS GRANTED TO $title", 'success');
        echo "<pre>";
        foreach ($results as $row) {
            // Sanitize all output
            foreach ($row as &$value) {
                $value = htmlspecialchars($value, ENT_QUOTES, 'UTF-8');
            }
            print_r($row);
        }
        echo "</pre>";
    } else {
        echoTerminal("QUERY RETURNED NO RESULTS", 'warning');
    }
}
?>