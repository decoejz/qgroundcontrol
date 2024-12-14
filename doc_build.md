# QGroundControl

## Building on Macos Sequoia (M3)

1. Follow the installation from the [documentation page](https://docs.qgroundcontrol.com/master/en/qgc-dev-guide/getting_started/)

2. Clone the repository and update submodules:

> ```bash
> git submodule update --recursive https://github.com/decoejz/qgroundcontrol.git
>
> git submodule update --recursive
> ```

2. I had to disable the `QGC_ENABLE_GST_VIDEOSTREAMING` optional feature. For that, access `root/cmake/CustomOptions.cmake` and change (or add) the line `option(QGC_ENABLE_GST_VIDEOSTREAMING "Enable GStreamer Video Backend" OFF)` from ON to OFF

3. Before building, I had to copy a file that was missing, probably because the submodule update didn\`t worked properly. For that, inside `root/resources/SDL_GameControllerDB` create a file named `gamecontrollerdb.txt` and paste the content from the [original file](https://github.com/mdqinc/SDL_GameControllerDB/blob/e15eac7b43d0527b475409b2a488681c5bbea1ea/gamecontrollerdb.txt)

4. Now it is possible to build. In the root of the project run:

> ```
> cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
> 
> cmake --build build --config Debug
> ```

5. Now just run the app that it will work just fine. The app file is `root/build/QGroundControl`