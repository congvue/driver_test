export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
make clean
make
sudo cp clock.ko ~/nfs/wyf/clock/

