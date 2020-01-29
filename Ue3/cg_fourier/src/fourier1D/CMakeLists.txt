file(GLOB SRCS ./*.cpp)
file(GLOB HDRS ./*.h)

add_executable(fourier1D ${SRCS} ${HDRS})
