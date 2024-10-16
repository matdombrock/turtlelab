// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function col(i) {
    color(i / 2,255 - i,255 - i, 255);
}

function main() { 
    bg(22,22,22);
    for (var i = 0; i < 8 * 8; i+=8) {
        downRight(i);
        for (var ii = i; ii < 256; ii+=8) {
            col(ii * 2);
            right(ii);
            down(ii);
            left(ii);
            up(ii);
        }
    }
}
