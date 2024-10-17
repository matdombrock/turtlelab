// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function polygon(sides, radius) {
    rotate(0)
    radius *= 2
    var sideLen = 2 * radius * Math.sin(Math.PI / sides);
    // sideLen = Math.round(sideLen);
    left(Math.round(sideLen/2));
    for (var i = 0; i < sides; i++) {
        forward(sideLen);
        rotateCW(Math.ceil(256 / sides));
    }
    goBack((sides * 2) + 3);
}

function main() {
    teleport(128,128); 
    bg(22,22,22);
    for (var i = 0; i < 16; i++) {
        var c = i * 16;
        color(255 - c, 255, c, 255); 
        polygon(3 + i, 16);
        up(8)
    }
}
