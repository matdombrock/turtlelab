// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function col(i) {
    color(i,255 - i,255 - i, 255);
}

function main() { 
    bg(75,55,55);
    teleport(8,8);
    for (var z = 0; z < 2; z++) {
        for (var x = 0; x < 48; x++) {
            col(x * 5);
            right(3);
            down(32);
            right(3);
            up(32);
        }
        right(3);
        down(64);
        for (var x = 0; x < 48; x++) {
            col(x * 5);
            left(3);
            down(32);
            left(3);
            up(32);
        }
        left(3);
        down(64);
    }
}
