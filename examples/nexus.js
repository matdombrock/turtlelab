// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function col(i) {
    color(i, 255 -i , i, 255);
}
function coin() {
    return Math.random() < 0.5;
}
function ups(i) {
    col(i * 32);
    teleport(128,256);
    for (var ii = 0; ii < 32; ii++) {
        if (coin()) {
            upRight(8);
        }
        else {
            upLeft(8);
        }
    }
}
function downs(i) {
    col(i * 32);
    teleport(128,0);
    for (var ii = 0; ii < 32; ii++) {
        if (Math.random() < 0.5) {
            downRight(8);
        }
        else {
            downLeft(8);
        }
    }
}
function main() { 
    bg(22,33,22);
    for (var i = 0; i < 16; i++) {
        ups(i);
    }
    for (var i = 0; i < 32; i++) {
        downs(i);
        ups(i);
    }
    for (var i = 0; i < 16; i++) {
        downs(i);
    }
}
