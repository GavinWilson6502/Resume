<?php
$servername = "localhost";
$username = "hw11";
$password = "[redacted]";
$dbname = "hw11";

try {
    $conn = new mysqli($servername, $username, $password, $dbname);
} catch (Exception $e) {
    die("Connection failed: " . $e);
}

$result = $conn->query("SELECT email FROM employees");
$emails = array();
while ($row = $result->fetch_assoc()) {
    $emails[] = $row["email"];
}
echo '{"emails":';
echo json_encode($emails);
echo '}';

$conn->close();
?>
