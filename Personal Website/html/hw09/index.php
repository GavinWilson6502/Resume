<?php
const N = 100;

$started = session_start();

if ($started) {
    if (isset($_GET["reset"])) session_unset();

    if (!isset($_SESSION["data"])) $_SESSION["data"] = [];
    if (!isset($_SESSION["result"])) $_SESSION["result"] = "undefined.";

    if (isset($_GET["op"])) {
	switch ($_GET["op"]) {
	    case "ordered":
		$_SESSION["data"] = [];
		for ($i = 0; $i < N; ++$i) $_SESSION["data"][$i] = $i;
		$_SESSION["result"] = N . " ordered numbers were listed";
		break;
	    case "random":
		$_SESSION["data"] = [];
		for ($i = 0; $i < N; ++$i) $_SESSION["data"][$i] = rand(0, N - 1);
		$_SESSION["result"] = N . " random numbers were generated";
		break;
	    case "inc":
		array_walk($_SESSION["data"], fn(&$value) => ++$value);
		$_SESSION["result"] = "data has been incremented";
		break;
	    case "oddify":
		array_walk($_SESSION["data"], fn(&$value) => $value = 2 * $value + 1);
		$_SESSION["result"] = "data has been modified";
		break;
	    case "alleven":
		$test = true;
		foreach ($_SESSION["data"] as $value) {
		    if ($value % 2 !== 0) {
			$test = false;
			break;
		    }
		}
		$_SESSION["result"] = "allEven is " . ($test ? "true" : "false");
		break;
	    case "allodd":
		$test = true;
		foreach ($_SESSION["data"] as $value) {
		    if ($value % 2 === 0) {
			$test = false;
			break;
		    }
		}
		$_SESSION["result"] = "allOdd is " . ($test ? "true" : "false");
		break;
	    case "twelve":
		$v = NULL;
		foreach ($_SESSION["data"] as $value) {
		    if ($value % 12 === 0) {
			$v = $value;
			break;
		    }
		}
		$_SESSION["result"] = is_null($v) ? "no multiple of 12" : "found $v";
		break;
	    case "skiprm":
		$_SESSION["data"] = array_filter(array_values($_SESSION["data"]), fn($key) => $key % 2 === 0, ARRAY_FILTER_USE_KEY);
		$_SESSION["result"] = "data has been skip/removed";
                break;
	    default:
		$_SESSION["result"] = "undefined.";
	}
    }
}
?>
<!DOCTYPE html>
<html>
  <head>
    <title>Server-Side Application</title>
    <link href="./style.css" rel="stylesheet"/>
  </head>
  <body>
    <h2>Data</h2>
    <div id="data">
      <?php
      if (!$started) echo "Failed to start session";
      else if (count($_SESSION["data"]) === 0) echo "nothing yet";
      else echo implode(" ", $_SESSION["data"]);
      ?>
    </div>
    <h2>Result of last command</h2>
    <div id="result">
      <?php
      if (!$started) echo "Failed to start session";
      else echo $_SESSION["result"];
      ?>
    </div>
    <h2>Controls</h2>
    <form>
      <button name="op" value="ordered">Fill ordered</button>
      <button name="op" value="random">Fill random</button>
      <button name="op" value="inc">increment</button>
      <button name="op" value="oddify">oddify</button>
      <button name="op" value="alleven">all even?</button>
      <button name="op" value="allodd">all odd?</button>
      <button name="op" value="twelve">multiple of 12</button>
      <button name="op" value="skiprm">skip/Remove</button>
    </form>
  </body>
</html>
