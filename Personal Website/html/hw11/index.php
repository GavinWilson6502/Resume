<?php
$servername = "localhost"; $username = "hw11"; $password = "[redacted]"; $dbname = "hw11";


// Create a connection
try {
  $conn = new mysqli($servername, $username, $password, $dbname);
} catch (Exception $e) {
  die("Connection failed: " . $e);
}


function open_page($title) {echo <<<EOS
  <!DOCTYPE html>
  <html>
    <head>
      <link rel="icon" href="data:,">
        <title>Dr.Z -- $title page</title>
      <link rel="stylesheet" href="./style.css"/>
    </head>
    <body>
      <div class="banner"><h1>Customer Information</h1></div>
      <img class="logo" src="./logo.png" alt="A logo consisting of a red stencil of a newsboy cap with wings"/>
EOS ;
}
function close_page() {echo '</body></html>';}


function generateHomePage() {
  global $conn ;
   ?>
  <form method="get">
    <input type="hidden" name="mode" value="customer"/>
    <label for="customer">Choose a customer:</label>
    <select name="customer">
      <option value="">--Select a Customer--</option>
      <?php
        $sql = "SELECT * from customers order by customerName" ;
        $result = $conn->query($sql);
        while($row = $result->fetch_assoc()) {
          echo "<option value=\"{$row['customerNumber']}\">{$row['customerName']}</option>" ;
        }
      ?>
    </select>
    <input type="submit" name="submit" value="go"/>
  </form>
<?php
}


function generateCustomerPage() {
  global $conn ;
  // Customer
  $customerID = $_POST['customer'] ?? $_GET['customer'] ?? "" ;
  if (!$customerID) return generateHomePage() ;
  $sql = sprintf("select * from `customers` where `customerNumber`='%s'", $conn->real_escape_string($customerID)) ;
  $result = $conn->query($sql);
  if (!$result->num_rows) {
    return generateHomePage() ;
  }
  $row = $result->fetch_assoc() ;
  /*echo "<code><pre>" . print_r($row,true) . "</pre></code>" ;*/
  echo "<h2>Details</h2><ul>";
  foreach ($row as $key => $value) echo "<li>$key: $value</li>";
  echo "</ul>";
  // Sales Rep
  $sql = sprintf("select `lastName`, `firstName`, `extension`, `email` from `employees` where `employeeNumber`='%s'",
    $conn->real_escape_string($row["salesRepEmployeeNumber"]));
  $result = $conn->query($sql);
  echo "<h2>Sales Representative</h2>";
  if ($result->num_rows) {
      echo "<ul>";
      foreach ($result->fetch_assoc() as $key => $value) echo "<li>$key: $value</li>";
      echo "</ul>";
  }
  else echo "<p>None assigned</p>";
  // Orders
  $sql = sprintf("select * from `orders` where `customerNumber`='%s'", $conn->real_escape_string($customerID)) ;
  $result = $conn->query($sql);
  /*$all = $result->fetch_all() ;
  echo "<code><pre>" . print_r($all,true) . "</pre></code>" ;*/
  ?>
  <h2>Orders</h2>
  <form method="get">
    <input type="hidden" name="mode" value="<?= $_POST["mode"] ?? $_GET["mode"] ?? "" ?>"/>
    <input type="hidden" name="customer" value="<?= $customerID ?>"/>
    <input type="hidden" name="submit" value="<?= $_POST["submit"] ?? $_GET["submit"] ?? "" ?>"/>
    <label for="order">Choose an order:</label>
    <select name="order">
      <option value="">--Select an Order--</option>
      <?php
        while($row = $result->fetch_assoc()) {
          echo "<option value=\"{$row['orderNumber']}\">" . implode(" ", $row) . "</option>";
        }
      ?>
    </select>
    <input type="submit" name="submitorder" value="go"/>
  </form>
  <?php
  // Payments
  $sql = sprintf("select * from `payments` where `customerNumber`='%s'", $conn->real_escape_string($customerID)) ;
  $result = $conn->query($sql);
  /*echo "<code><pre>" . print_r($result->fetch_all(),true) . "</pre></code>" ;*/
  echo "<h2>Payments</h2><table>";
  echo array_reduce($result->fetch_fields(), fn($carry, $item) => $carry . "<th>" . $item->name . "</th>", "<tr>") . "</tr>";
  while ($row = $result->fetch_assoc()) {
    echo "<tr><td>" . implode("</td><td>", $row) . "</td></tr>";
  }
  echo "</table>";
}


$router = [
  "home" => 'generateHomePage' ,
  "customer" => 'generateCustomerPage',
] ;


$MODE = $_POST['mode'] ?? $_GET['mode'] ?? 'home' ;
if (!array_key_exists($MODE,$router)) $MODE = "home" ;


open_page($MODE) ;
$router[$MODE]() ;
close_page() ;
?>
