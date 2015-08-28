dumb-lexer
=
This is a "dumb lexer" in the sense that it uses either a transition state table or an explicit state definiiton. This is for my CS 5630 Compilers class. If you are taking this class or one similar to it, please be sure to ask your professor if you may use this source before you use it.

Miscallaneous
=
"herp derp it uses the -std=c++0x flag!"

Yeah, shut up, I know. This is because the target machine that we has only GCC 4.4 (it actually has GCC 4.8, but you can't use it with Makefiles) which only supports up to the experimental features of C++11. This is nice because it includes stuff like the "auto" keyword (among other things), but not so nice because it is obviously not a great standard to target.

However, if you want to compile this with a more established standard, (e.g. c++11, c++14) you can pass `STD=c++11` or `STD=c++14` as an argument, and it will compile according to that standard.

License
=
MIT License, see the LICENSE file.