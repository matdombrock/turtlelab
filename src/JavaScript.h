#pragma once
#include "../lib/duktape/duktape.h"
#include "Native.h"
#include "LangBase.h"

namespace JavaScriptBinds {
    Native native;
    SDL_Renderer *ren;
    duk_context *ctx;

    void init(SDL_Renderer *renderer) {
        ren = renderer;
        ctx = duk_create_heap_default();
    }

    duk_ret_t print(duk_context *ctx) {
        if (duk_is_string(ctx, 0)) {
            native.print(duk_get_string(ctx, 0));
        } else if (duk_is_number(ctx, 0)) {
            native.print(duk_get_number(ctx, 0));
        } else {
            native.print("Unsupported type");
        }
        return 0;
    }

    duk_ret_t forward(duk_context *ctx) {
        int n = duk_require_int(ctx, 0);
        native.forward(n);
        return 0;
    }

    duk_ret_t rotate(duk_context *ctx) {
        int a = duk_require_int(ctx, 0);
        native.rotate(a);
        return 0;
    }

    duk_ret_t rotateCW(duk_context *ctx) {
        int a = duk_require_int(ctx, 0);
        native.rotateCW(a);
        return 0;
    }

    duk_ret_t rotateCCW(duk_context *ctx) {
        int a = duk_require_int(ctx, 0);
        native.rotateCCW(a);
        return 0;
    }
    
    duk_ret_t up(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.up(n);
        return 0;
    }
    duk_ret_t down(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.down(n);
        return 0;
    }
    duk_ret_t left(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.left(n);
        return 0;
    }
    duk_ret_t right(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.right(n);
        return 0;
    }
    duk_ret_t upRight(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.upRight(n);
        return 0;
    }
    duk_ret_t upLeft(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.upLeft(n);
        return 0;
    }
    duk_ret_t downRight(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.downRight(n);
        return 0;
    }
    duk_ret_t downLeft(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.downLeft(n);
        return 0;
    }
    duk_ret_t teleport(duk_context *ctx) {
        int x = duk_require_int(ctx, 0);
        int y = duk_require_int(ctx, 1);
        native.teleport(x, y);
        return 0;
    }
    duk_ret_t goBack(duk_context *ctx) {
        int n = duk_opt_int(ctx, 0, 1);
        native.goBack(n);
        return 0;
    }
    duk_ret_t color(duk_context *ctx) {
        int r = duk_require_int(ctx, 0);
        int g = duk_require_int(ctx, 1);
        int b = duk_require_int(ctx, 2);
        int a = duk_opt_int(ctx, 3, 255); // Default alpha value is 255 if not provided
        native.color(r, g, b, a);
        return 0;
    }
    duk_ret_t bg(duk_context *ctx) {
        native.bg(duk_require_int(ctx, 0), duk_require_int(ctx, 1), duk_require_int(ctx, 2));
        return 0;
    }
    void setBinds() {
        duk_push_global_object(ctx);
        duk_push_c_function(ctx, print, 1);
        duk_put_prop_string(ctx, -2, "print");
        duk_push_c_function(ctx, forward, 1);
        duk_put_prop_string(ctx, -2, "forward");
        duk_push_c_function(ctx, rotate, 1);
        duk_put_prop_string(ctx, -2, "rotate");
        duk_push_c_function(ctx, rotateCW, 1);
        duk_put_prop_string(ctx, -2, "rotateCW");
        duk_push_c_function(ctx, rotateCCW, 1);
        duk_put_prop_string(ctx, -2, "rotateCCW");
        duk_push_c_function(ctx, up, 1);
        duk_put_prop_string(ctx, -2, "up");
        duk_push_c_function(ctx, down, 1);
        duk_put_prop_string(ctx, -2, "down");
        duk_push_c_function(ctx, left, 1);
        duk_put_prop_string(ctx, -2, "left");
        duk_push_c_function(ctx, right, 1);
        duk_put_prop_string(ctx, -2, "right");
        duk_push_c_function(ctx, upRight, 1);
        duk_put_prop_string(ctx, -2, "upRight");
        duk_push_c_function(ctx, upLeft, 1);
        duk_put_prop_string(ctx, -2, "upLeft");
        duk_push_c_function(ctx, downRight, 1);
        duk_put_prop_string(ctx, -2, "downRight");
        duk_push_c_function(ctx, downLeft, 1);
        duk_put_prop_string(ctx, -2, "downLeft");
        duk_push_c_function(ctx, teleport, 2);
        duk_put_prop_string(ctx, -2, "teleport");
        duk_push_c_function(ctx, goBack, 1);
        duk_put_prop_string(ctx, -2, "goBack");
        duk_push_c_function(ctx, color, 4);
        duk_put_prop_string(ctx, -2, "color");
        duk_push_c_function(ctx, bg, 3);
        duk_put_prop_string(ctx, -2, "bg");
        duk_pop(ctx);
    }

    std::string prependExtras(const std::string &str) {
        std::string extras =  R"()";
        return extras + str;
    }
}

class JavaScript : public LangBase {
public:
    JavaScript() {}
    ~JavaScript() {
        duk_destroy_heap(JavaScriptBinds::ctx);
    }
    void load(const std::string &fileContents, SDL_Renderer *renderer) override {
        file = fileContents;
        file = JavaScriptBinds::prependExtras(file);
        JavaScriptBinds::init(renderer);
        JavaScriptBinds::setBinds();
        std::cout << "JavaScript initialized" << std::endl;
    }
    void run() override{
        // std::string env = "var ticks = " + std::to_string(ticks) + ";";
        std::string env = "";
        std::string fileEnv = env + file;
        duk_push_string(JavaScriptBinds::ctx, fileEnv.c_str());
        if (duk_peval(JavaScriptBinds::ctx) != 0) {
            std::cerr << "Error: " << duk_safe_to_string(JavaScriptBinds::ctx, -1) << std::endl;
            exit(1);
        }
        duk_pop(JavaScriptBinds::ctx);
        duk_push_global_object(JavaScriptBinds::ctx);
        duk_get_prop_string(JavaScriptBinds::ctx, -1, "main");
        if (duk_pcall(JavaScriptBinds::ctx, 0) != 0) {
            std::cerr << "Error: " << duk_safe_to_string(JavaScriptBinds::ctx, -1) << std::endl;
            exit(1);
        }
        duk_pop(JavaScriptBinds::ctx);
    }
    void process(int index, CLIOpts opts) override{
        JavaScriptBinds::native.process(index, opts, JavaScriptBinds::ren);
    }
};