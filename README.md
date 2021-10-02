# LBBS

## What is it?
LBBS is a Meson-inspired build system that intends to be extreamly flexible and extensible (like Scons) but without sacrificing speed and simplicity. It uses lua to acheive this.

## Name
Like Meson, it was also named after a subatomic particle. I first thought about photon(light and fast) but there was already a pretty popular project named photon. so I used lbbs.

# Advantages

## Small
The code is less than 600 lines, of code and the stripped binary is ~460kb(on an intel i3 x86_64 machine with `-fno-rtti -fno-exceptions`)

99% of the work is done by STL, sol2, and luaJIT.

## Written in c++
LBBS is written c++ instead of something like python. This is a bit counterintuitive since c++ is not nearly as portable as python or as easy to understand.

LBBS makes up for this by using lua(jit). lua is designed to extend and to be extended.

This project also gives an example of modern c++17 code. It reduces the portability but is way easier to use. Modern c++ is quite differant from c++98.

## Extensibility
Due to its use of lua(jit) you can extend LBBS in whatever language you want as long as it can compile into a shared library, anything that compiles to lua or lua itself(pretty op).

This means you can extend LBBS in pretty much any language eg c++, c, go, rust, typescript etc

## Turing completeness
This is as bad as it is good. This means that one can do more things with LBBS but that also means that a lot of people will reinvent the entire thing in pure lua(aka shoot themselves in the foot) or not make use of libraries provided by the project(eg using `Rule.new` and `Rule:generate` instead of using a language module)

## Easier to deal with multiple languages

``` lua
local cc = require'lang.c.compiler'
cc.executable(...)
local rust = require'lang.rust.compiler'
-- Languages can use whatever they want and aren't forced to use executables
rust.crate(...)
local cpython = require'cpython-lbbs' -- example of a module not part of lbbs
cpython.executable(...)
```
