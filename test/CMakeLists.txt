include_directories(..)

add_executable(palettes palettes.cpp)
add_test(palettes palettes)

add_executable(mandelbrot mandelbrot.cpp)
add_test(mandelbrot mandelbrot)
