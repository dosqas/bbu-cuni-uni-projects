CREATE DATABASE cyberdyne;
USE cyberdyne;

-- Core employee table with sensitive data
CREATE TABLE employees (
    id INT AUTO_INCREMENT PRIMARY KEY,
    badge_id VARCHAR(10) UNIQUE,
    username VARCHAR(30),
    password VARCHAR(100),  -- Store hashed passwords in real systems
    access_level ENUM('intern', 'scientist', 'executive', 'root'),
    secret_project VARCHAR(100),
    salary DECIMAL(10,2),
    ssn VARCHAR(11),       -- Social Security Number
    last_login DATETIME
);

-- Projects table for UNION attacks
CREATE TABLE projects (
    id INT PRIMARY KEY,
    project_name VARCHAR(100),
    status ENUM('planning', 'active', 'classified', 'completed'),
    budget DECIMAL(15,2),
    description TEXT
);

-- Nuclear launch codes (maximum impact)
CREATE TABLE nuclear_codes (
    id INT PRIMARY KEY,
    code_name VARCHAR(50),
    launch_code VARCHAR(100),
    location VARCHAR(100),
    authorization_level INT
);

-- System configuration table
CREATE TABLE system_config (
    id INT PRIMARY KEY,
    config_name VARCHAR(50),
    config_value TEXT,
    restricted BOOLEAN
);

-- Audit logs table
CREATE TABLE access_logs (
    id INT AUTO_INCREMENT PRIMARY KEY,
    badge_id VARCHAR(10),
    action VARCHAR(100),
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    ip_address VARCHAR(45)
);

-- Sample data
-- Insert employees with easily guessable badge IDs
INSERT INTO employees VALUES
(1, 'CD-007', 'jbond', 'password123!', 'executive', 'Project: Sentinel', 250000.00, '123-45-6789', NOW()),
(2, 'CD-101', 'msmith', 'Winter2023?', 'scientist', 'Neural Network X', 180000.00, '987-65-4321', NOW()),
(3, 'ADMIN', 'sysadmin', 'K3rb3r0s!', 'root', 'SKYNET CORE', 500000.00, '456-78-9123', NOW()),
(4, 'ROOT', 't800', 'I\'llBeBack', 'root', 'TERMINATOR', 750000.00, '000-00-0000', NOW()),
(5, 'GUEST', 'temp_user', 'Welcome1', 'intern', 'Temporary Access', 45000.00, NULL, NOW()),
(6, 'BACKUP', 'backup_admin', 'B@ckUp!', 'root', 'Disaster Recovery', 300000.00, '111-22-3333', NOW()),
(7, 'TEST', 'tester', 'Test1234', 'intern', 'QA Systems', 50000.00, NULL, NOW()),
(8, '999999', 'emergency', 'BreakGlass', 'root', 'Emergency Access', 0.00, '999-99-9999', NOW());

INSERT INTO projects VALUES
(1, 'Sentinel AI', 'classified', 5000000.00, 'Autonomous defense system phase 1'),
(2, 'Neural Network X', 'active', 3200000.00, 'Next-gen machine learning framework'),
(3, 'Project Phoenix', 'planning', 10000000.00, 'AI resurrection protocol');

INSERT INTO nuclear_codes VALUES
(1, 'Alpha Protocol', '7H3M4TR1X', 'Silo 7B', 5),
(2, 'Omega Directive', 'N3V3R64NN', 'Bunker Delta', 5);

INSERT INTO system_config VALUES
(1, 'firewall_enabled', 'true', true),
(2, 'require_2fa', 'false', false),
(3, 'max_login_attempts', '3', false);

INSERT INTO access_logs (badge_id, action, ip_address) VALUES
('CD-007', 'login', '192.168.1.100'),
('CD-101', 'file_access', '10.0.0.15'),
('CD-999', 'system_reboot', '127.0.0.1');