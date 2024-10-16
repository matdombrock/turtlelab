// This uses the DuktapeJS engine to run JavaScript code
// It only supports ES5, so no let, const, or arrow functions etc

// The code runs only once at startup so calling Math.random() will always return the same value
// This is because the seed for the random number generator is always the same 
function main() { 
    // Generate 1024 random dots
    for (var i = 0; i < 1024; i+=8) {
        var r = Math.random() * 255;
        var g = Math.random() * 255;
        var b = Math.random() * 255;
        color(r, g, b, 255);
        var x = Math.random() * 256;
        var y = Math.random() * 256;
        teleport(x, y);    
    }
}
