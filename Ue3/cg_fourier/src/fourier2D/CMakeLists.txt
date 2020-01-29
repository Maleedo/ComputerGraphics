add_executable(filter fourier.h fourier.cpp image.h image.cpp filter.cpp)
target_link_libraries(filter lodePNG)

add_executable(hybrid fourier.h fourier.cpp image.h image.cpp hybrid.cpp)
target_link_libraries(hybrid lodePNG)
