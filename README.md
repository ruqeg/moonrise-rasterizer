<p align="center">
  <img src="https://github.com/ruqeg/moonrise-rasterizer/blob/main/moonrise-rasterizer.jpeg" width="350">
</p>

-----

Rasterizer written in С from scratch

Features:
* Draw primitives by triangles ( outline, filled, colored )
* Perspective projection and mini math library
* Geometry rendering optimization
* * Clip models outside of the camera's field of view
* * Clip back faces
* * Сropping primitives at the edge of camera planes
* * Bounding sphere
* Textures optimization
* * Mipmap ( use gamma correction box-filtering )
* * bilinear-trilinear filtering and ect.
* Vertext/Fragment shaders


### Build Examples/Library
-----

```bash
# Compile flags:
# -  MRE_BUILD_EXAMPLE_ROTATING_CUBE
# -  MRE_BUILD_EXAMPLE_TEXTURED_CUBE
# -  BUILD_SHARED_LIBS
# Build rotating cube example & autocompile library
mkdir build && cd build
cmake -DMRE_BUILD_EXAMPLE_TEXTURED_CUBE=ON .. && make
./example/example_exe
```


### Shaders docs
-----
Vertex shader
```c
in  - Vec3
out - Vec3
v   - current vertex
```

Fragment shader
```c
in     - Vec3
out    - Vec3
fragp  - current frag global position
cord   - frag window cords ( x, y, -1 < z < 1 )
```


### Used libraries
-----

| Program | Library |
| --- | -------------- |
| Rasterizer | NONE |
| Example | [SDL](https://github.com/libsdl-org/SDL), [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) |


### Acknowledgements
-----
* [Computer Graphics from Scratch](https://www.gabrielgambetta.com/computer-graphics-from-scratch/) was very helpful in developing the rasterizer.


### Collage of screenshots
-----
<img src="https://github.com/ruqeg/moonrise-rasterizer/blob/main/example_photo.jpeg" width="350">
