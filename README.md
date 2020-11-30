#### 模型加载
c加载obj和mtl文件简单实现模型的局部光照、颜色纹理载入

#### CMake
```javascript
// windos MinGW
cd build
cmake -G "MinGW Makefiles" ..
make
./main

// Linux GNU
cd build
cmake ..
make
./main

// windos msvc
cd build
cmake ..
devenv main.sln /build debug      
cd ./build/debug
./main

// 摄像机
上下左右
wsad
```

