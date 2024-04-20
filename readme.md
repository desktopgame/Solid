# Solid

# Setup
````bat
cd src\Solid\mod\vcpkg
bootstrap-vcpkg.bat
vcpkg install directxtex
````

# Build
````bat
cd Solid
cmake -S src -B build -G "Unix Makefiles"
make -C build
````