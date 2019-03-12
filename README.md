# loop-roi
loop-roi is a tool for editing short loop video. You can select an area to keep the pixels still inside. This feature is useful to cancel mouth movements of characters in Japanese anime.

#### Dependencies

##### macOS

``` Shell
# Qt 5
brew install qt5

# OpenCV
# Install from Homebrew
brew install opencv@3
# Install from source compile
wget https://github.com/opencv/opencv/archive/3.4.5.zip -O opencv-3.4.5.zip
unzip opencv-3.4.5.zip
cd opencv-3.4.5
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j $(nproc)
sudo make install  
```

> If you want to deploy a production package, you should install opencv from source compile. opencv from Homebrew include ffmpeg, it means that the size of package will be very large (I test it is about 200MB) if  you deploy with opencv from Homebrew, while the size of package deployed from source compile without ffmpeg is only about 90MB.

#### Compile & Build 

##### Qt Creator

Open `loop-roi.pro` with Qt Creator and run it.

#### Deploy

Switch build configuration to Release and build.

```Shell
# deploy with macdeployqt
macdeployqt path/to/build-dir/loop-roi.app
# copy dylibs
# currently you should copy these dylibs: core imgproc highgui videoio
cp -R /usr/local/lib/libopencv_<component>.<version>.dylib path/to/build-dir/loop-roi.app/Contents/Frameworks/
# if you deploy with opencv from Homebrew, maybe need more dylibs for ffmpeg
```

## License

loop-roi is published under GPL 3.0 License. See the LICENSE file for more.