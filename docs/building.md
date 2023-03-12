\page building Building controller
\tableofcontents

# Setting up the enviornment

The following instructions describe the instructions to build `controller` on a Raspberry Pi, assuming a
Raspberry Pi OS installation.

First, make sure your system is up-to-date and install the necessary packages from the repo.

```shell
sudo apt update
sudo apt upgrade

sudo apt install build-essential cmake git python3 wget
```

Because our scheduler uses the C++20 `<latch>` feature, which is not implemented in the older version of `GCC` offered
by Raspberry Pi OS, we will have to grab a portable compiler environment. Run the following commands in the directory
in which you wish to install this environment (`$HOME`, `/opt`, `$HOME/opt`) are good choices.

```shell
# Grab the environment

wget http://musl.cc/armv7l-linux-musleabihf-native.tgz

# Unpack the environment

tar xvfz armv7l-linux-musleabihf-native.tgz

# Set environment variables for future use

export MOBILE_PREFIX_PATH="$PWD/armv7l-linux-musleabihf-native"

echo "export MOBILE_PREFIX_PATH=\"$MOBILE_PREFIX_PATH\"" >> "$HOME/.profile"
echo 'export MOBILE_CC="$MOBILE_PREFIX_PATH/bin/gcc"' >> "$HOME/.profile"
echo 'export MOBILE_CXX="$MOBILE_PREFIX_PATH/bin/g++"' >> "$HOME/.profile"
echo 'export MOBILE_LD_PATH="$MOBILE_PREFIX_PATH/lib"' >> "$HOME/.profile"
```

\note You should now reset your system (or log out and log back in) for the environment variables to take effect.

# Building and installing dependencies

## OpenCV

```shell
# Grab the source code

wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.x.zip

# Unpack the source code

unzip opencv.zip
unzip opencv_contrib.zip

# Create build directory and switch into it

mkdir -p build-opencv && cd build-opencv

# Configure

CC="$MOBILE_CC" CXX="$MOBILE_CXX" cmake -DOPENCV_EXTRA_MODULES_PATH=../opencv_contrib-4.x/modules ../opencv-4.x

# Build

cmake --build .

# Install

cmake --install . --prefix "$MOBILE_PREFIX_PATH"
```

## sockpp

```shell
wget -O sockpp.tar.gz https://github.com/fpagliughi/sockpp/archive/refs/tags/v0.8.1.tar.gz
tar xvfz sockpp.tar.gz && mv sockpp-0.8.1 sockpp

# Create build directory and switch into it

mkdir sockpp/build && cd sockpp/build

# Configure

CC="$MOBILE_CC" CXX="$MOBILE_CXX" cmake ..

# Build

cmake --build .

# Install

cmake --install . --prefix "$MOBILE_PREFIX_PATH"
```

# Building controller

```shell
git clone --recurse-submodules https://github.com/Electrum-Romania/Bosch-Controller
cd Bosch-Controller

# Generate headers

cd src
python3 ./generate_options.py
cd ..

# Configure

mkdir build && cd build
CC="$MOBILE_CC" CXX=$MOBILE_CXX" cmake ..

# Build

cmake --build .
```

# Compilation options

The controller program has a number of compile-time configuration options you may wish to change. This can be done
when running the `generate_options.py` script. For example to change the address and port of the command socket you can
run the script as follows:

```shell
python3 ./generate_options.py -D command_socket_host=\"192.168.0.25\" -D command_socket_port=3003
```

\note The double-quotes of string parameters should be escaped in the shell, like above.

\sa StaticOptions

# Running controller

To run the controller issue the following command in the build directory:

```shell
LD_LIBRARY_PATH="$MOBILE_LD_PATH" ./controller
```

\warning Running the executable normally will lead to dynamic linking errors.
