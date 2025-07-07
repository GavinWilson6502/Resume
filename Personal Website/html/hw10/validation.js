"use strict";

function onCommChanged(event) {
  const comm = event.currentTarget.querySelector("[type=radio][name=comm]:checked");
  const emailField = event.currentTarget.querySelectorAll("[name=email]");
  const phoneField = event.currentTarget.querySelectorAll("[name=phone]");
  if (event.type === "reset" || comm === null || comm.getAttribute("value") === "email") {
    emailField.forEach((currentValue) => currentValue.removeAttribute("disabled"));
    phoneField.forEach((currentValue) => currentValue.setAttribute("disabled", ""));
  }
  else {
    emailField.forEach((currentValue) => currentValue.setAttribute("disabled", ""));
    phoneField.forEach((currentValue) => currentValue.removeAttribute("disabled"));
  }
}

function validate(event) {
  const feedback = document.getElementById("feedback");
  if (feedback === null) return;
  feedback.innerHTML = "The following fields have invalid input:";
  if (event.type === "reset") {
    feedback.innerHTML += "<br/>First name"
                        + "<br/>Last name"
                        + "<br/>Email"
                        + "<br/>Message";
  }
  else if (event.currentTarget.checkValidity()) feedback.innerHTML = "";
}

window.addEventListener("load", function() {
  const form = document.getElementById("mainform");
  if (form === null) return;

  form.addEventListener("change", onCommChanged);
  form.addEventListener("reset", onCommChanged);
  form.dispatchEvent(new Event("reset"));
  form.addEventListener("invalid", (event) => {
    const feedback = document.getElementById("feedback");
    if (feedback === null || feedback.innerHTML === "") return;
    let name = null;
    switch (event.target.getAttribute("name")) {
      case "fname":
        name = "First name";
        break;
      case "lname":
        name = "Last name";
        break;
      case "email":
        name = "Email";
        break;
      case "phone":
        name = "Phone number";
        break;
      case "msg":
        name = "Message";
        break;
    }
    if (!feedback.innerHTML.includes(name)) feedback.innerHTML += "<br/>" + name;
  }, true);
  form.addEventListener("input", validate);
  form.addEventListener("change", validate);
  form.addEventListener("reset", validate);

  const feedback = document.getElementById("feedback");
  if (feedback !== null && feedback.innerHTML === "") {
    form.dispatchEvent(new Event("reset"));
  }
});
