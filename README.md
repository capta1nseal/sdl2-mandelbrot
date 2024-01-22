# SDL2 mandelbrot
Just a little hobby project in fractal rendering.

## installation
- Requires an installation of SDL2 on your include path, or provide your own and link in Makefile.
- Run ```make``` from project root to build the project, ```make test``` to build and instantly run.

### usage
- run the executable
- window can be resized, toggle fullscreen with F11
    - (everything will be recalculated)
- move around with WASD keys
- zoom in/out with up/down arrow keys
- zoom in and centre on click by left-clicking

#### status
- draws mandelbrot set
- greyscale colouring with smooth histogram shading
- iteration count is incremented per frame
- basic navigation implemented
- colouring is persistent regardless of iteration count
- pretty slow. rendered on CPU

##### planned
- render in colour using an HSV to RGB transform
- performance optimisation:
    - render with openGL shaders instead of CPU pixel manipulation
    - (related) multithreading
- smooth user experience
