// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function col(i) {
    color(i,255 - i,255 - i, 255);
}

function main() { 
    bg(55,55,55);
    teleport(0,0);
    for (var i = 0; i < 256; i++) {
        col(i);
        right(i);
        down(i);
        left(i);
        up(i);
    }
}
