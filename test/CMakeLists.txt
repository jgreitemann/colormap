include_directories(.)

add_executable(palettes palettes.cpp)
add_test(palettes palettes)

add_executable(mandelbrot mandelbrot.cpp)
add_test(mandelbrot mandelbrot)

add_executable(buf buf.cpp)
add_test(buf buf)

add_executable(grid grid.cpp)
add_test(grid grid)
