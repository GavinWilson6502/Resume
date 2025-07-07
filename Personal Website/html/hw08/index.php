<?php
$n = 100;
$fibn = 20;

echo "<!DOCTYPE html>
<html>
<head>
<title>Homework #8</title>
</head>
<body>
<h1>Homework #8</h1>
<h2>First $n odds</h2>
<p>";

if ($n > 0) {
    $i = 0;
    echo 2 * $i + 1;
    if ($n > 1) {
	if ($n > 2) echo ",";
	for ($i = 1; $i < $n - 1; ++$i) {
	    echo " " . 2 * $i + 1 . ",";
	}
	echo " and " . 2 * $n - 1;
    }
    echo ".";
}

echo "</p>
<h2>First $fibn Fibonacci's</h2>
<p>";

if ($fibn > 0) {
    $prev2 = 1;
    $prev1 = 0;
    $temp = $prev2 + $prev1;
    $prev2 = $prev1;
    $prev1 = $temp;
    echo $temp;
    for ($i = 1; $i < $fibn; ++$i) {
	$temp = $prev2 + $prev1;
	$prev2 = $prev1;
	$prev1 = $temp;
	echo " $temp";
    }
}

echo "</p>
<h2>Primes less than $n</h2>
<p>";

for ($i = 2; $i < $n; ++$i) {
    for ($j = 2; $j < $i && $i % $j != 0; ++$j);
    if ($j == $i) {
	echo $i;
	break;
    }
}
for ($i = $i + 1; $i < $n; ++$i) {
    for ($j = 2; $j < $i && $i % $j != 0; ++$j);
    if ($j == $i) echo " $i";
}

echo "</p>
</body>
</html>";
?>
