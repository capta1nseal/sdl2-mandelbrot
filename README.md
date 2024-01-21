# SDL2 mandelbrot
Just a little hobby project in fractal rendering.

## installation
- Requires an installation of SDL2 on your include path, or provide your own and link in Makefile.
- Run ```make``` from project root to build the project, ```make test``` to build and instantly run.

### usage
- run the executable
- window can be resized, fullscreen with F11
    - (everything will be recalculated)
- move around with WASD keys
- zoom in/out with up/down arrow keys

#### status
- draws shaded mandelbrot set to a display of arbitrary size
- iteration count is incremented per frame
- navigation implemented (albeit slow due to recalculating on evey movement)
- pretty slow. every pixel is drawn as a point through SDL every frame.

##### planned
- make the colouring stay persistent regardless of the total number of iterations made
- performance optimisation
    - fill an SDL pixel grid with the correct colours then render, instead of rendering points separately
    - (related) multithreading
- smooth user experience
