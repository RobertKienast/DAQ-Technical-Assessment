# Brainstorming

This file is used to document your thoughts, approaches and research conducted across all tasks in the Technical Assessment.

## Firmware
Stage 1: 
(Disclaimer): for the first part, setting up docker and CMake 
I used chatGPT as I haven't used those things (docker first time) at all.

For the start I didn't fully understand the dbcppp library depth
and how much we could use it, so I at the start I made my own 
CAN decoder (which didn't work with big endian but worked for little endian). This doesn't now work because I changed the structs. 
Then realized how much I was meant to use the library and got that working.
Honestly most of my time was just gettting dbcppp working.

Honestly I didn't read through the spec sheet too much and just realized that
the output is not stdout (which makes sense) so a dodgey solution will be done

NOTE 
you may have to pipe the input
by doing 
./answer > output.txt

Stage 3:
Sorry about this but start the docker enviroment and connect to it by bash
then do to the tests/build and so
sudo docker exec -it {container-name} bash (99% it will firmware-firmware-1)
(not the one in just build as all the rel files are broken)
cd tests/build
CMake ..
Make
./unit_tests

## Spyder

## Cloud