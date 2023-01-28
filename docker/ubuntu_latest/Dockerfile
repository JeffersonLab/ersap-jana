FROM ubuntu:latest

LABEL author="xmei@jlab.org"
LABEL description="Put xmsg-cpp, ersap-cpp, ersap-java and ersap-jana env together"

ENV DEPS=/deps
WORKDIR $DEPS

USER root

# some general software
RUN apt update -y && \
        apt -y install --no-install-recommends ca-certificates git build-essential cmake libzmq5-dev libprotobuf-dev protobuf-compiler && \
        rm -rf /var/lib/apt/lists/*

# install jdk-14
ADD https://download.java.net/java/GA/jdk14/076bab302c7b4508975440c56f6cc26a/36/GPL/openjdk-14_linux-x64_bin.tar.gz openjdk.tar.gz
RUN tar xvf openjdk.tar.gz && \
    rm -rf openjdk.tar.gz && \
    mv jdk-14 /opt/
ENV JAVA_HOME=/opt/jdk-14
ENV PATH=$PATH:$JAVA_HOME/bin

# build and install ersap-java
# refer to https://github.com/JeffersonLab/ersap-java/blob/main/docker/Dockerfile
ENV ERSAP_HOME="/usr/local/ersap"
ENV PATH="${ERSAP_HOME}/bin:${PATH}"
RUN git clone https://github.com/JeffersonLab/ersap-java.git && \
    cd ersap-java && \
    ./gradlew && \
    ./gradlew deploy
VOLUME ["${ERSAP_HOME}/data/input", "${ERSAP_HOME}/data/output", "${ERSAP_HOME}/log"]

# build and intstall xMsg
RUN git clone https://github.com/JeffersonLab/xmsg-cpp.git && \
    cd xmsg-cpp && \
    mkdir install && \
    ./configure --prefix=${ERSAP_HOME} && \
    make && \
    make install

# build and install ersap-cpp
RUN git clone https://github.com/JeffersonLab/ersap-cpp.git && \
    cd ersap-cpp && \
    mkdir install && \
    ./configure --prefix=${ERSAP_HOME} && \
    make && \
    make install
RUN rm -rf $DEPS/ersap-* $DEPS/xmsg-cpp

ENV LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${ERSAP_HOME}/lib

# build and install JANA2
# see the below error msg, lock down to v.2.0.6 now
# CMake Error at /deps/JANA2/install/lib/cmake/JANA/JANAConfig.cmake:31 (get_target_property):
#  get_target_property() called with non-existent target "JANA::jana2".
# Call Stack (most recent call first):
#  CMakeLists.txt:42 (find_package)
ENV JANA_HOME=$DEPS/JANA2/install
ENV PATH=$PATH:$JANA_HOME/bin
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$JANA_HOME/plugins
RUN git clone -b v2.0.6 http://github.com/JeffersonLab/JANA2 && \
    cd JANA2 -&& \
    mkdir build install && \
    cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=$JANA_HOME && \
    make -j8 install && \
    cd .. && rm -rf build

#what is this for? copied from https://github.com/JeffersonLab/ersap-java/blob/main/docker/Dockerfile
EXPOSE 7771-7775

WORKDIR /app
