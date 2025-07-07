<?php
$feedback = "";

$fname = "";
$lname = "";
$passwd = "";
$commVoice = "";
$commSMS = "";
$commEmail = "";
$interestOS = "";
$interestHW = "";
$interestConsulting = "";
$email = "";
$phone = "";
$msg = "";

if (!isset($_GET["fname"])) $feedback .= "<br/>First name is missing";
else if (strlen($_GET["fname"]) < 2) $feedback .= "<br/>First name is shorter than 2 characters";
else $fname = $_GET["fname"];

if (!isset($_GET["lname"])) $feedback .= "<br/>Last name is missing";
else if (strlen($_GET["lname"]) < 2) $feedback .= "<br/>Last name is shorter than 2 characters";
else if (strlen($_GET["lname"]) < 4) $feedback .= "<br/>Last name is shorter than 4 characters";
else $lname = $_GET["lname"];

if (!isset($_GET["comm"]) || $_GET["comm"] === "email") {
    if (isset($_GET["urgency"]) && $_GET["urgency"] === "Urgent") $feedback .= "<br/>Method of communication for urgent requests must be voice or SMS, not email";
    else $commEmail = " checked";

    if (!isset($_GET["email"])) $feedback .= "<br/>Email is missing";
    else if (filter_var($_GET["email"], FILTER_VALIDATE_EMAIL, FILTER_FLAG_EMAIL_UNICODE) === false) $feedback .= "<br/>Email is invalid";
    else {
        $temp = preg_match("/@(my\.)?utsa.edu$/", $_GET["email"]);
        if ($temp === false) $feedback .= "<br/>Email domain validation failed";
        else if ($temp === 0) $feedback .= "<br/>Email does not end in @my.utsa.edu or @utsa.edu";
        else $email = $_GET["email"];
    }
}
else {
    if ($_GET["comm"] === "voice") $commVoice = " checked";
    else if ($_GET["comm"] === "sms") $commSMS = " checked";

    if (!isset($_GET["phone"])) $feedback .= "<br/>Phone number is missing";
    else {
        $temp = preg_match("/^\(\d{3}\) \d{3}-\d{4}$/", $_GET["phone"]);
        if ($temp === false) $feedback .= "<br/>Phone number validation failed";
        else if ($temp === 0) $feedback .= "<br/>Phone number is invalid";
        else $phone = $_GET["phone"];
    }
}

if (!isset($_GET["msg"])) $feedback .= "<br/>Message is missing";
else $msg = $_GET["msg"];

$invalid = $feedback !== "";
if ($invalid) {
    $feedback = "The following fields have invalid input:" . $feedback;
    if (isset($_GET["passwd"])) $passwd = $_GET["passwd"];
    if (preg_match("/(^i|&i)nterest=o(s&|s$)/", $_SERVER["QUERY_STRING"])) $interestOS = " checked";
    if (preg_match("/(^i|&i)nterest=h(w&|w$)/", $_SERVER["QUERY_STRING"])) $interestHW = " checked";
    if (preg_match("/(^i|&i)nterest=consultin(g&|g$)/", $_SERVER["QUERY_STRING"])) $interestConsulting = " checked";
}
else $feedback = "Information was successfully submitted and well received";
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Client/Server Input Validation</title>
      <style>
        form>* {
          display: block;
        }
      </style>
      <link href="./style.css" rel="stylesheet"/>
      <script src="./validation.js"></script>
  </head>
  <body>
    <p id="feedback"><?= $feedback ?></p>
    <?php if ($invalid) echo '
    <form id="mainform">
      <label>
        First name
        <input name="fname" value="' . $fname . '" required minlength="2" pattern="[\'\-A-Za-z]*"/>
      </label>
      <label>
        Last name
        <input name="lname" value="' . $lname . '" required minlength="2" pattern="[\'\-A-Za-z]*"/>
      </label>
      <label>
        Password
        <input type="password" name="passwd" value="' . $passwd . '"/>
      </label>
      <fieldset>
        <legend>Preferred method of communication</legend>
        <label>
          <input type="radio" name="comm" value="voice"' . $commVoice . '/>
          Voice
        </label>
        <label>
          <input type="radio" name="comm" value="sms"' . $commSMS . '/>
          SMS
        </label>
        <label>
          <input type="radio" name="comm" value="email"' . $commEmail . '/>
          Email
        </label>
      </fieldset>
      <fieldset>
        <legend>Products/services of interest</legend>
        <label>
          <input type="checkbox" name="interest" value="os"' . $interestOS . '/>
          Wing Cap OS
        </label>
        <label>
          <input type="checkbox" name="interest" value="hw"' . $interestHW . '/>
          M Series HW
        </label>
        <label>
          <input type="checkbox" name="interest" value="consulting"' . $interestConsulting . '/>
          Consulting
        </label>
      </fieldset>
      <label>
        Email
        <input type="email" name="email" value="' . $email . '" required/>
      </label>
      <label>
        Phone number
        <input name="phone" value="' . $phone . '" required pattern="\(\d{3}\) \d{3}-\d{4}"/>
      </label>
      <label>
        Message
        <input name="msg" value="' . $msg . '" required/>
      </label>
      <div class="spacer"></div>
      <div>
        <button name="urgency" value="Normal">Normal</button>
        <button name="urgency" value="Urgent">Urgent</button>
      </div>
      <button type="reset" value="Reset">Reset</button>
    </form>';
    ?>
  </body>
</html>
