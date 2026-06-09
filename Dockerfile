FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-base-dev-tools \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libpq-dev \
    libpq5 \
    postgresql-client \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 \
             .. \
    && make -j$(nproc)

FROM ubuntu:22.04 AS test

RUN apt-get update && apt-get install -y \
    libpq5 \
    postgresql-client \
    libxcb-xinerama0 \
    libxcb-xkb1 \
    libxkbcommon-x11-0 \
    libgl1-mesa-glx \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    libqt6network6 \
    libdouble-conversion3 \
    libglib2.0-0 \
    libpcre2-16-0 \
    libxcb-xfixes0 \
    libxcb-randr0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-render-util0 \
    libxcb-shape0 \
    libxcb-util1 \
    libxcb-xkb1 \
    libxkbcommon-x11-0 \
    && rm -rf /var/lib/apt/lists/*

RUN ldconfig

COPY --from=builder /app/build/tests/run_tests /usr/local/bin/run_tests
ENV QT_QPA_PLATFORM=minimal

CMD ["/usr/local/bin/run_tests"]

FROM ubuntu:22.04 AS runtime

RUN apt-get update && apt-get install -y \
    libpq5 \
    libgl1-mesa-glx \
    libxcb-xinerama0 \
    libxcb-xkb1 \
    libxkbcommon-x11-0 \
    libqt6core6 \
    libqt6gui6 \
    libqt6widgets6 \
    postgresql-client \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/warehouse_app /usr/local/bin/warehouse_app

COPY docker/wait-for-postgres.sh /usr/local/bin/wait-for-postgres.sh
RUN chmod +x /usr/local/bin/wait-for-postgres.sh

ENTRYPOINT ["/usr/local/bin/wait-for-postgres.sh"]
CMD ["/usr/local/bin/warehouse_app"]