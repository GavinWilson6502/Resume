"use strict";

window.addEventListener("load", function() {
  let n = window.prompt("how many color segments?");
  if (n === null) return;
  try {
    n = Number(n);
    if (n < 2 || n === NaN || Math.abs(n) === Infinity) {
      n = 2;
    }
  }
  catch {
    n = 2;
  }

  let hw = document.createElement("div");
  hw.classList.add("hw");
  hw.innerHTML = "<h1>Hello, World!</h1>";
  let buttons = document.createElement("div");
  for (let r = 0; r < n; ++r) {
    for (let g = 0; g < n; ++g) {
      for (let b = 0; b < n; ++b) {
        let temp = document.createElement("button");
        temp.setAttribute("type", "button");
        temp.innerHTML = (r * n * n + g * n + b);
        temp.style.backgroundColor = "#" + ("00" + Math.round(255 * r / (n - 1)).toString(16)).slice(-2)
                                         + ("00" + Math.round(255 * g / (n - 1)).toString(16)).slice(-2)
                                         + ("00" + Math.round(255 * b / (n - 1)).toString(16)).slice(-2);
        buttons.append(temp);
      }
    }
  }
  buttons.addEventListener("click", function() {
    let el;
    for (el = event.target; el.tagName !== "BUTTON"; el = el.parentElement) {
      if (el === event.currentTarget) return;
    }
    hw.style.backgroundColor = el.style.backgroundColor;
  });
  document.body.append(hw);
  document.body.append(buttons);
});
