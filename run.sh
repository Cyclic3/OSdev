#dd if=/dev/zero of=/dev/sda3
./clean.sh && ./iso.sh && VirtualBox --startvm "Q" --debug
#./qemu.sh
