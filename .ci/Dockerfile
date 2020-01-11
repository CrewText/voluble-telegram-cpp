FROM gcc:latest
RUN apt-get update
RUN apt-get install zlib1g-dev libssl-dev libsqlite3-dev libsqlite3-0 gperf build-essential clang git -y

RUN wget https://github.com/Kitware/CMake/releases/download/v3.16.2/cmake-3.16.2.tar.gz
RUN tar -xgf cmake-3.16.2.tar.gz
WORKDIR cmake-3.16.2
RUN ./bootstrap
RUN make
RUN make install

RUN git clone https://github.com/fnc12/sqlite_orm.git sqlite_orm
RUN mkdir -p sqlite_orm/__bld
WORKDIR sqlite_orm/__bld
RUN CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DSqliteOrm_BuildTests=OFF ..
RUN cmake --build .
RUN make install
WORKDIR ../..

RUN git clone https://github.com/gabime/spdlog.git
RUN mkdir -p spdlog/__bld
WORKDIR spdlog/__bld
RUN CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake .. && make -j
RUN make install
WORKDIR ../..

RUN git clone https://github.com/tdlib/td
RUN mkdir -p td/__bld
WORKDIR td/__bld
RUN CC=/usr/bin/clang CXX=/usr/bin/clang++ cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cmake --build .
RUN make install