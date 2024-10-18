#pragma once
#include "../lib/s7/s7.h"
#include "LangBase.h"
#include "Native.h"

namespace SchemeBinds {
    Native native;
    SDL_Renderer *ren;
    s7_scheme *s7;

    void init(SDL_Renderer *renderer) {
        ren = renderer;
        s7 = s7_init();
    }

    s7_pointer print(s7_scheme *sc, s7_pointer args) {
        s7_pointer arg = s7_car(args);
        if (s7_is_string(arg)) {
            native.print(s7_string(arg));
        } else if (s7_is_integer(arg)) {
            native.print(s7_integer(arg));
        } else if (s7_is_real(arg)) {
            native.print(s7_real(arg));
        } else {
            native.print("Unknown type");
        }
        return s7_nil(sc);
    }

    s7_pointer forward(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.forward(n);
        return s7_nil(sc);
    }

    s7_pointer rotate(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.rotate(n);
        return s7_nil(sc);
    }

    s7_pointer rotateCW(s7_scheme *sc, s7_pointer args) {
        float a = s7_real(s7_car(args));
        native.rotateCW(a);
        return s7_nil(sc);
    }

    s7_pointer rotateCCW(s7_scheme *sc, s7_pointer args) {
        float a = s7_real(s7_car(args));
        native.rotateCCW(a);
        return s7_nil(sc);
    }

    s7_pointer up(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.up(n);
        return s7_nil(sc);
    }
    s7_pointer down(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.down(n);
        return s7_nil(sc);
    }
    s7_pointer left(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.left(n);
        return s7_nil(sc);
    }
    s7_pointer right(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.right(n);
        return s7_nil(sc);
    }
    s7_pointer upRight(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.upRight(n);
        return s7_nil(sc);
    }
    s7_pointer upLeft(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.upLeft(n);
        return s7_nil(sc);
    }
    s7_pointer downRight(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.downRight(n);
        return s7_nil(sc);
    }
    s7_pointer downLeft(s7_scheme *sc, s7_pointer args) {
        int n = s7_integer(s7_car(args));
        native.downLeft(n);
        return s7_nil(sc);
    }
    s7_pointer teleport(s7_scheme *sc, s7_pointer args) {
        native.teleport(s7_integer(s7_car(args)), s7_integer(s7_cadr(args)));
        return s7_nil(s7);
    }
    s7_pointer goBack(s7_scheme *sc, s7_pointer args) {
        native.goBack(s7_integer(s7_car(args)));
        return s7_nil(s7);
    }
    s7_pointer color(s7_scheme *sc, s7_pointer args) {
        int r = s7_integer(s7_car(args));
        int g = s7_integer(s7_cadr(args));
        int b = s7_integer(s7_caddr(args));
        int a = s7_is_null(sc, s7_cdddr(args)) ? 255 : s7_integer(s7_cadddr(args)); // Default alpha to 255 if not provided
        native.color(r, g, b, a);
        return s7_nil(s7);
    }
    s7_pointer bg(s7_scheme *sc, s7_pointer args) {
        native.bg(s7_integer(s7_car(args)), s7_integer(s7_cadr(args)), s7_integer(s7_caddr(args)));
        return s7_nil(s7);
    }
    void setBinds() {
        s7_define_function(s7, "print", print, 1, 0, false, "Prints a string");
        s7_define_function(s7, "forward", forward, 1, 0, false, "Moves forward");
        s7_define_function(s7, "rotate", rotate, 1, 0, false, "Rotates absolute");
        s7_define_function(s7, "rotate-cw", rotateCW, 1, 0, false, "Rotates Clockwise");
        s7_define_function(s7, "rotate-ccw", rotateCCW, 1, 0, false, "Rotates Counter Clockwise");
        s7_define_function(s7, "up", up, 1, 0, false, "Moves up");
        s7_define_function(s7, "down", down, 1, 0, false, "Moves down");
        s7_define_function(s7, "left", left, 1, 0, false, "Moves left");
        s7_define_function(s7, "right", right, 1, 0, false, "Moves right");
        s7_define_function(s7, "up-right", upRight, 1, 0, false, "Moves up and right");
        s7_define_function(s7, "up-left", upLeft, 1, 0, false, "Moves up and left");
        s7_define_function(s7, "down-right", downRight, 1, 0, false, "Moves down and right");
        s7_define_function(s7, "down-left", downLeft, 1, 0, false, "Moves down and left");
        s7_define_function(s7, "teleport", teleport, 2, 0, false, "Teleports to a location");
        s7_define_function(s7, "go-back", goBack, 1, 0, false, "Moves back");
        s7_define_function(s7, "color", color, 3, 1, false, "Sets the color");
        s7_define_function(s7, "bg", bg, 3, 0, false, "Sets the background color");
    }

    std::string prependExtras(const std::string &str) {
        // Define some helpful scheme native functions
        std::string extras = R"(
    (define (displayln v) (display v)(newline))
    (define (% a b) (modulo a b))
    (define (loop n fn)
        (let l ((i 0))
            (when (< i n) 
                    (fn i)(l (+ i 1)))))
        )";
        return extras + str;
    }
}// namespace SchemeBinds

class Scheme : public LangBase {
public:
    Scheme() {}
    ~Scheme() {}
    void load(const std::string &fileContents, SDL_Renderer *renderer) override {
        file = fileContents;
        file = SchemeBinds::prependExtras(file);
        SchemeBinds::init(renderer);
        SchemeBinds::setBinds();
        std::cout << "Scheme initialized" << std::endl;
    }
    void run() override{
        std::string env = "";
        // Scheme is expected to return 0 if everything is ok
        std::string fileEnv = env + file + " 0";
        s7_pointer result = s7_load_c_string(SchemeBinds::s7, fileEnv.c_str(), fileEnv.size());
        std::string resString = s7_object_to_c_string(SchemeBinds::s7, result);
        DBG("Scheme result: " + resString);
        if (resString != "0") {
            Log("ERROR: Issue with Scheme code");
            exit(1);
        }
        s7_quit(SchemeBinds::s7);
    }
    void process(RunState state, CLIOpts opts) override {
        SchemeBinds::native.process(state, opts, SchemeBinds::ren);
    }
};