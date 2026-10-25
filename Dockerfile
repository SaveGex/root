FROM ubuntu:24.04

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential cmake libboost-regex-dev libmuparser-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY CMakeLists.txt *.cpp *.h ./
COPY tests/ ./tests/

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel 2 \
    && ctest --test-dir build --output-on-failure

# CTest writes logs when rerun inside the container.
RUN chown -R 10001:10001 /app/build
USER 10001:10001
ENTRYPOINT ["/app/build/equation-solver"]
