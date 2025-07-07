<?php
// this code should be saved in hw11/partone.php
$servername = "localhost";
$username = "hw11";
$password = "[redacted]";
$dbname = "hw11";

// Create a connection
try {
    $conn = new mysqli($servername, $username, $password, $dbname);
} catch (Exception $e) {
    die("Connection failed: " . $e);
}

// Execute the query
$sql = "SELECT E.firstName, E.lastName, O.city, O.state, O.country FROM employees as E, offices as O where E.officeCode = O.officeCode";
$result = $conn->query($sql);

// go over the results
if ($result->num_rows > 0) {
  echo "<table><tr><th></th><th>Last</th><th>First</th><th>City</th><th>State</th><th>Country</th></tr>" ;
  $k=0;
  while($row = $result->fetch_assoc()) {
    echo '<tr>' ; 
    echo '<td>' . ++$k . ')</td>' ; 
    foreach(['lastName','firstName','city','state','country'] as $key) {
        echo '<td>' . $row[$key] . '</td>' ; 
    }
    echo '</tr>' ; 
  }
  echo '</table>' ;
} else {
  echo "0 results";
}

$conn->close();    
?>

