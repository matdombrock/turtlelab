// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

function main() {
    teleport(128, 0); 
    bg(22,22,22);
    color(3255,255,255);
    // Each rotation unit is 1/256 of a full rotation
    // So 256 units is a full rotation
    // This will draw a circle over 256 iterations (slow)
    for (var i = 0; i < 256; i++) {
        rotateCW(1);
        forward(1);
    }
    // However we don't need 256 iterations to draw a circle at this resolution
    // We can use much smaller numbers
    // This will draw a circle over 64 iterations (faster)
    teleport(128, 128);
    color(255,0,0);
    for (var i = 0; i < 64; i++) {
        rotateCW(4);
        forward(4);
    }
    // We can also use a different radius to draw a circle
    // This will draw a circle with a radius of 32
    teleport(128, 0);
    color(0,255,0);
    for (var i = 0; i < 32; i++) {
        rotateCW(8);
        forward(8);
    }
    // Lets define a function to draw a polygon with a given radius
    // A circle is really just a polygon with an infinite number of sides
    // At this resolution we can get away with a very small number of sides and still get a circle
    // Note: The larger the radius, the more sides you will need to get a circular shape
    function polygon(radius, sides) {
        for (var i = 0; i < sides; i++) {
            rotateCW(256 / sides);
            forward(radius);
        }
    }
    // Now we can draw a circle with a radius of 16
    teleport(128, 0);
    color(0,0,255);
    polygon(16, 32);
    color(0,0,255);
    polygon(9, 8);
}
