// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function col(i) {
    color(i / 2,255 - i,255 - i, 255);
}

function main() { 
    bg(22,22,22);
    for (var i = 0; i < 256; i+=8) {
        col(i * 2);
        right(i);
        down(i);
        left(i);
        up(i);
    }
}
