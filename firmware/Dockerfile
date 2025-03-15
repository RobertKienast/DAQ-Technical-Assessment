
FROM ubuntu:22.04 AS dev
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /app

RUN apt-get update && \
    apt-get install -y python3-pip \
    wget autoconf libtool git make g++ valgrind \
    libboost-all-dev \
    libxml2-dev libxslt-dev \ 
    catch2 \ 
    cmake can-utils

COPY . .


#RUN git clone https://github.com/catchorg/Catch2.git tests/lib && \
#    cd tests/lib/Catch2 && \ 
#    cmake -Bbuild -H. -DBUILD_TESTING=OFF && \
#    cmake --build build --target install


RUN rm -rf modules/dbcppp
# Clone dbcppp
RUN git clone --recursive https://github.com/xR3b0rn/dbcppp.git modules/dbcppp
RUN cd modules/dbcppp && git submodule update --init --recursive
# Clean the build directory before building dbcppp
RUN rm -rf modules/dbcppp/build && mkdir modules/dbcppp/build

RUN cd modules/dbcppp/build && cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cd modules/dbcppp/build && make -j
RUN cd modules/dbcppp/build && make RunTests
RUN cd modules/dbcppp/build && make install
RUN cd modules/dbcppp/build && ldconfig


#RUN rm -rf solution/main_unit_tests/lib/dbcppp
#RUN git clone --recursive https://github.com/xR3b0rn/dbcppp.git solution/main_unit_tests/lib/dbcppp
#RUN cd solution/main_unit_tests/lib/dbcppp && mkdir build
#RUN cd solution/main_unit_tests/lib/dbcppp/build && cmake -DCMAKE_BUILD_TYPE=Release ..
#RUN cd solution/main_unit_tests/lib/dbcppp/build && make -j
#RUN cd solution/main_unit_tests/lib/dbcppp/build && make RunTests
#RUN cd solution/main_unit_tests/lib/dbcppp/build && make install
#RUN cd solution/main_unit_tests/lib/dbcppp/build && ldconfig

RUN cd tests/lib && git clone https://github.com/catchorg/Catch2.git 




RUN find . -name "CMakeCache.txt" -delete

RUN cd build/ && rm CMakeCache.txt || true

RUN cd build/ && cmake .. 

RUN cd build/ && make -j    

#RUN cd tests && rm build -rf 
#RUN cd tests && mkdir build
##RUN cd tests/build && cmake ..
#RUN cd tests/build && make


#ENTRYPOINT ["./build/solution/answer > output.txt"]
#ENTRYPOINT ["./tests/build/unit_tests"]
ENTRYPOINT ["/usr/bin/bash"]

