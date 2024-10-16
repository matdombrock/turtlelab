// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function col(i) {
    color(255 - i, i ,i/2, 255);
}

function main() { 
    bg(22,22,22);
    teleport(8,200);
    for (var i = 0; i < 256; i+=8) {
        col(i * 2);
        var ii = i / 2;
        upRight(ii);
        downRight(ii);
        left(i)
    }
    for (var i = 0; i < 128; i+=8) {
        col(i * 2);
        var ii = i / 2;
        upRight(ii);
        downRight(ii);
        upLeft(ii);
        downLeft(ii);
        right(ii/4);
    }
    teleport(8,200);
    for (var i = 0; i < 128; i+=8) {
        col(i * 2);
        var ii = i / 2;
        upRight(6);
        upRight(ii);
        downRight(ii);
        upLeft(ii);
        downLeft(ii);
        right(ii/4);
    }
    teleport(8,200);
    for (var i = 0; i < 128; i+=8) {
        col(i * 2);
        var ii = i / 2;
        upRight(ii);
        downRight(ii);
        upLeft(ii);
        downLeft(ii);
        right(ii/4);
        up(ii/2);
    }
}
