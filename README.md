# SDL2 mandelbrot
Just a little hobby project in fractal rendering.

## installation
- Requires an installation of SDL2 on your include path, or provide your own and link in Makefile.
- Run ```make``` from project root to build the project, ```make test``` to build and instantly test.

### usage
- run the executable
- window can be resized, fullscreen with F11
    - (resets drawing of fractal)
- move around with WASD
- zoom in/out with up/down

#### status
- draws shaded mandelbrot set to a display of arbitrary size
- iteration count is incremented per frame
- navigation implemented (albeit slow due to resetting)

##### planned
- make the colouring stay persistent regardless of the total number of iterations made
- performance optimisation
- (related) multithreading
- smooth user experience
