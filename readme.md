# Path tracer

For UI and drawing the render I use [ImGui](https://github.com/ocornut/imgui) with GLFW and OpenGL3 backend and
additional libraries for saving the image: [STB image](https://github.com/nothings/stb)
and [tinyfiledialogs](https://github.com/native-toolkit/libtinyfiledialogs)

## Implementation

Implemented following the [Ray Tracing in One weekend](https://raytracing.github.io/) guide by Peter Shirley.

Features:

- Monte Carlo sampling
- Lambertian/Glass/Metal Material
- Sphere/Rectangle Diffuse Light
- Russian roulette optimizer
- Oren-Nayar for roughness and angle of incidence

### Improvements:

- Focused rendering
- De-noising

Some results: (Note: its a WIP, focusing works on choosing the closest sphere)

![](Results/50_100.jpg)
![](Results/30_100_800.jpg)

- Adding new meshes
- Faster rendering (parallelization)
- Positioning of the camera

## Old Results

### 91 samples, 30 sample depth

Russian roulette

![Russian roulette result](Results/russianroulette.jpg)

Final render with nearly 500 samples per pixel in a 800:800 picture

![500 samples per pixel](Results/final_render.jpg)

## Development

### Windows

Opening the solution file should work

### Linux

TBD
