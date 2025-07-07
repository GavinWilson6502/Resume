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

$sql = "SELECT officeCode, addressLine1, addressLine2, city, state, postalCode, country, territory FROM offices";
$result = $conn->query($sql);

$feedback = "";
$ajax = "";
$firstName = "";
$lastName = "";
$jobTitle = "";
$email = "";
$office = null;
if (isset($_GET["submit"])) {
    if (!isset($_GET["firstName"])) $feedback .= "<br/>First name is missing";
    else if (strlen($_GET["firstName"]) < 2) $feedback .= "<br/>First name is shorter than 2 characters";
    else {
        $temp = preg_match("/[^'\-A-Za-z]/", $_GET["firstName"]);
        if ($temp === false) $feedback .= "<br/>First name input validation failed";
        else if ($temp !== 0) $feedback .= "<br/>First name contains characters other than alphabet characters, hyphens, and apostrophes";
        else $firstName = $_GET["firstName"];
    }

    if (!isset($_GET["lastName"])) $feedback .= "<br/>Last name is missing";
    else if (strlen($_GET["lastName"]) < 2) $feedback .= "<br/>Last name is shorter than 2 characters";
    else {
        $temp = preg_match("/[^'\-A-Za-z]/", $_GET["lastName"]);
        if ($temp === false) $feedback .= "<br/>Last name input validation failed";
        else if ($temp !== 0) $feedback .= "<br/>Last name contains characters other than alphabet characters, hyphens, and apostrophes";
        else $lastName = $_GET["lastName"];
    }

    if (!isset($_GET["jobTitle"])) $feedback .= "<br/>Job title is missing";
    else if (strlen($_GET["jobTitle"]) < 6) $feedback .= "<br/>Job title is shorter than 6 characters";
    else {
        $temp = preg_match("/[^\-0-9A-Za-z]/", $_GET["jobTitle"]);
        if ($temp === false) $feedback .= "<br/>Job title input validation failed";
        else if ($temp !== 0) $feedback .= "<br/>Job title contains characters other than alphabet characters, numerals, and hyphens";
        else $jobTitle = $_GET["jobTitle"];
    }

    if (!isset($_GET["email"])) $feedback .= "<br/>Email is missing";
    else if (filter_var($_GET["email"], FILTER_VALIDATE_EMAIL, FILTER_FLAG_EMAIL_UNICODE) === false) $feedback .= "<br/>Email is invalid";
    else {
        $temp = preg_match("/\.com$/", $_GET["email"]);
        if ($temp === false) $feedback .= "<br/>Email top-level domain validation failed";
        else if ($temp === 0) $feedback .= "<br/>Email does not end in .com";
        else {
            $temp = $conn->query("SELECT email FROM employees");
            while ($t = $temp->fetch_assoc()) {
                if ($_GET["email"] === $t["email"]) {
                    $ajax = 'Email "' . $_GET["email"] . '" is already associated with an employee in the database';
                    break;
                }
            }
            if ($ajax === "") $email = $_GET["email"];
        }
    }

    if (isset($_GET["office"])) $office = $_GET["office"];

    if ($feedback === "") {
        $insert = $conn->prepare("INSERT INTO employees (employeeNumber, lastName, firstName, extension, email, officeCode, reportsTo, jobTitle) VALUES (?, ?, ?, \"x0000\", ?, ?, NULL, ?);");
        $insert->bind_param("isssss", rand(2001, getrandmax()), $lastName, $firstName, $email, $office, $jobTitle);
        if ($insert->execute()) {
            $conn->close();
            header("Location: ./");
            die();
        }
        else $feedback = "Error: failed to execute database query";
    }
    else $feedback = "The following fields have invalid input:" . $feedback;
}
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Add New Employee</title>
    <style>
      form>* {
        display: block;
      }
    </style>
    <script src="./ajax.js"></script>
  </head>
  <body>
    <p><?= $feedback ?></p>
    <p id="ajax"><?= $ajax ?></p>
    <form>
      <label>
        First Name
        <input name="firstName" value="<?= $firstName ?>" required minlength="2" pattern="['\-A-Za-z]*"/>
      </label>
      <label>
        Last Name
        <input name="lastName" value="<?= $lastName ?>" required minlength="2" pattern="['\-A-Za-z]*"/>
      </label>
      <label>
        Job Title
        <input name="jobTitle" value="<?= $jobTitle ?>" required minlength="4" pattern="[\-0-9A-Za-z]*"/>
      </label>
      <label>
        Email
        <input type="email" id="email" name="email" value="<?= $email ?>" required/>
      </label>
      <label>
        Office
        <select name="office">
          <?php
          while ($row = $result->fetch_assoc()) {
              echo '<option value="' . $row['officeCode'] . '"';
              if (isset($office) && $office === $row['officeCode']) echo ' selected';
              echo '>';
              echo $row['addressLine1'];
              if (!is_null($row['addressLine2'])) echo ', ' . $row['addressLine2'];
              echo ', ' . $row['city'] . ', ';
              if (!is_null($row['state'])) $row['state'] . ' ';
              echo $row['postalCode'];
              echo ', ' . $row['country'];
              echo ', ' . $row['territory'];
              echo '</option>';
          }
          ?>
        </select>
      </label>
      <button type="reset">Reset</button>
      <button name="submit">Submit</button>
    </form>
  </body>
</html>
<?php
$conn->close();
?>
