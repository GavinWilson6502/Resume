"use strict";

function odds(n) {
  return Array.from(Array(n), (x, i) => 2 * i + 1);
}
function fibi(n) {
  let result = Array.from(Array(n), () => undefined);
  result.forEach((x, i, a) => { a[i] = (i < 2) ? 1 : (a[i - 1] + a[i - 2]); });
  return result;
}
function primes(n) {
  let result = [];
  for (let i = 2; i < n; ++i) {
    if (!result.some((x) => i % x === 0)) result.push(i);
  }
  return result;
}
function stringify(array, join=((x) => x.join(" "))) {
  return join(array.map((x) => x.toString()));
}
function oxford(strings) {
  if (strings.length > 1) strings[strings.length - 1] = "and " + strings[strings.length - 1];
  return strings.join(strings.length == 2 ? " " : ", ") + ".";
}

document.getElementById("odds").innerHTML = stringify(odds(100), oxford);
document.getElementById("fibo").innerHTML = stringify(fibi(100));
document.getElementById("primes").innerHTML = stringify(primes(100));
