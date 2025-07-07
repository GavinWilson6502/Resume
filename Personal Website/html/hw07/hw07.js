"use strict";

const numrows = 4, numcols = 4;
const numshuffles = 128;

let pieces = Array(numrows);
let rows = Array(numrows);
let blankR = Math.floor(Math.random() * numrows);
let blankC = Math.floor(Math.random() * numcols);
let blank2;

function slide(r, c) {
  rows[r].replaceChild(blank2, pieces[r][c]);
  rows[blankR].replaceChild(pieces[r][c], pieces[blankR][blankC]);
  rows[r].replaceChild(pieces[blankR][blankC], blank2);
  let temp = pieces[r][c];
  pieces[r][c] = pieces[blankR][blankC];
  pieces[blankR][blankC] = temp;
  blankR = r;
  blankC = c;
}

function shuffle(n) {
  for (let i = 0; i < n; ++i) {
    let rand = Math.random();
    if (rand < 0.5) {
      if (rand < 0.25 && blankR !== 0 || blankR === numrows - 1) {
        slide(blankR - 1, blankC);
      }
      else {
        slide(blankR + 1, blankC);
      }
    }
    else {
      if (rand < 0.75 && blankC !== 0 || blankC === numcols - 1) {
        slide(blankR, blankC - 1);
      }
      else {
        slide(blankR, blankC + 1);
      }
    }
  }
}

window.addEventListener("load", function() {
  const board = document.getElementById("board");

  for (let i = 0; i < numrows; ++i) {
    pieces[i] = [];
    rows[i] = document.createElement("div");
    for (let j = 0; j < numcols; ++j) {
      let image = document.createElement("img");
      image.setAttribute("src", "hw07.jpg");
      image.setAttribute("alt", i * numrows + j + 1);
      image.style.width = numrows * 100 + "%";
      image.style.left = -j * 100 + "%";
      let ratio = image.naturalHeight / image.naturalWidth;
      pieces[i][j] = document.createElement("span");
      pieces[i][j].style.width = 100 / numrows + "%";
      pieces[i][j].style.marginTop = ratio * i * -(100 / numrows) + "%";
      pieces[i][j].style.marginBottom = ratio * (100 / numrows - 100 + i * 100 / numrows) + "%";
      pieces[i][j].append(image);
      rows[i].append(pieces[i][j]);
    }
    board.append(rows[i]);
  }
  pieces[blankR][blankC].style.visibility = "hidden";
  blank2 = pieces[blankR][blankC].cloneNode(true);
  shuffle(numshuffles);

  board.addEventListener("mousedown", (event) => event.preventDefault());

  board.addEventListener("click", function(event) {
    let el;
    for (el = event.target; !pieces.some((element) => element.includes(el)); el = el.parentElement) {
      if (el === event.currentTarget) return;
    }
    let r = pieces.findIndex((element) => element.includes(el));
    if (r < 0) return;
    let c = pieces[r].findIndex((element) => element === el);
    if (c < 0) return;
    if (r === blankR && Math.abs(c - blankC) === 1
        || c === blankC && Math.abs(r - blankR) === 1) {
      slide(r, c);
    }
  });
});
