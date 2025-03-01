// NAME: API Example
// LANG: JavaScript
// DESCRIPTION: This example demonstrates the use of the API functions in JavaScript
// AUTHOR: Mathieu Dombrock
// CLI: -t

// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc
function main() { 
    bg(22,33,44);
    color(255,255,255);
    teleport(128,64);
    down(64);
    right(64);
    up(64);
    left(64);
    downRight(64);
    downLeft(64);
    upLeft(64);
    upRight(64);
    goBack(7);
    rotateCW(64);
    rotateCCW(32);
    rotate(0);
    forward(32);
    print("Hello, world!");
}
