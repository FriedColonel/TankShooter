#### Install SDL2

```bash
sudo apt-get install libsdl2-dev
```

#### Install SDL2-ttf

```bash
sudo apt-get install libsdl2-ttf-dev g++ gdb cmake ninja-build pkg-config
```

#### Install SDL2-mixer

```bash
sudo apt install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev libsdl2-mixer-2.0-0 -y
```

#### Install SDL2-image

```bash
sudo apt install libjpeg-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 -y
```

### Thêm file cmake

- Tạo 3 fouder `SDL2_image`, `SDL2_ttf`, `SDL2_mixer` ở đường dẫn `/usr/lib/x86_64-linux-gnu/cmake/`
- Lần lượt trong từng folder thêm một file tương ứng với từng folder ví dụ: `/usr/lib/x86_64-linux-gnu/cmake/SDL2_image/sdl2_image-config.cmake`

#### sdl2_image-config.cmake

```bash
set(prefix "/usr")
set(exec_prefix "${prefix}")
set(libdir "${prefix}/lib/x86_64-linux-gnu")
set(SDL2_IMAGE_PREFIX "/usr")
set(SDL2_IMAGE_EXEC_PREFIX "/usr")
set(SDL2_IMAGE_INCLUDE_DIRS "${prefix}/include/SDL2_image")
set(SDL2_IMAGE_LIBRARIES "-lSDL2_image")
```

#### sdl2_ttf-config.cmake

```bash
set(prefix "/usr")
set(exec_prefix "${prefix}")
set(libdir "${prefix}/lib/x86_64-linux-gnu")
set(SDL2_TTF_PREFIX "/usr")
set(SDL2_TTF_EXEC_PREFIX "/usr")
set(SDL2_TTF_INCLUDE_DIRS "${prefix}/include/SDL2_ttf")
set(SDL2_TTF_LIBRARIES "-lSDL2_ttf")
```

#### sdl2_mixer-config.cmake

```bash
set(prefix "/usr")
set(exec_prefix "${prefix}")
set(libdir "${prefix}/lib/x86_64-linux-gnu")
set(SDL2_MIXER_PREFIX "/usr")
set(SDL2_MIXER_EXEC_PREFIX "/usr")
set(SDL2_MIXER_INCLUDE_DIRS "${prefix}/include/SDL2_mixer")
set(SDL2_MIXER_LIBRARIES "-lSDL2_mixer")
```
