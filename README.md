# SDL3 mandelbrot
Just a little hobby project in fractal rendering.

## installation
- Requires an installation of SDL3 on your include path, or provide your own and link in Makefile.
- Run `make` from project root to build the project, `make test` to build and instantly run. The flag `-j<n>` can be used to set the number of threads to use for the build, where `<n>` is the number of threads.

### usage
- run the executable
- window can be resized, toggle fullscreen with F11
- move around with WASD keys
- zoom in/out with up/down arrow keys
- zoom in and centre on click by left-clicking
    - resizing or zooming may result in needing to wait a moment until enough iterations are recalculated to be able to see anything
- keys 1-4 select a shading function. works instantly and doesn't need any recalculations.

#### status
- draws mandelbrot set
- greyscale colouring with smooth histogram shading
- iteration count is incremented per frame
- basic navigation implemented
- pretty slow. rendered on CPU

##### planned
- performance optimisation:
    - render with openGL shaders instead of CPU pixel manipulation
    - multithreading
- smooth user experience
    - speed up rendering enough (maybe add initial low-quality pass(es)) to make navigation feel snappy
    - GUI for changing between colouring algorithms, zooming, etc.
