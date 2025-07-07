"use strict";

function checkEmail(r) {
  const email = document.getElementById("email");
  const msg = document.getElementById("ajax");
  if (email !== null && r.emails.includes(email.value)) {
    if (msg !== null) msg.innerHTML = "Email \"" + email.value + "\" is already associated with an employee in the database";
    email.value = "";
  }
  else if (msg !== null) msg.innerHTML = "";
}
function jsonRejected() {
  const msg = document.getElementById("ajax");
  if (msg !== null) msg.innerHTML = "Failed to parse data while checking for conflicting email addresses";
}

function checkResponse(r) {
  if (!r.ok) {
    const msg = document.getElementById("ajax");
    if (msg !== null) msg.innerHTML = "Couldn't check database for conflicting email addresses";
    return;
  }
  r.json().then(checkEmail, jsonRejected);
}
function fetchRejected() {
  const msg = document.getElementById("ajax");
  if (msg !== null) msg.innerHTML = "Couldn't check database for conflicting email addresses";
}

function liveServerValidate() {
  const email = document.getElementById("email");
  if (email !== null && email.validity.valid)
    fetch("./ajax.php").then(checkResponse, fetchRejected);
}

window.addEventListener("load", function() {
  const email = document.getElementById("email");
  if (email !== null) email.addEventListener("input", liveServerValidate);
});
