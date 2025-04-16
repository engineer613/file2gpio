# Use Debian Stretch as the base
FROM debian:stretch

# Avoid interactive prompts during apt install
ENV DEBIAN_FRONTEND=noninteractive

# Replace default APT sources with Debian archive (Stretch is EOL)
RUN sed -i 's|http://deb.debian.org/debian|http://archive.debian.org/debian|g' /etc/apt/sources.list && \
    sed -i '/security.debian.org/d' /etc/apt/sources.list && \
    echo 'Acquire::Check-Valid-Until "false";' > /etc/apt/apt.conf.d/99no-check-valid-until


# Install necessary packages: cross-compilers, cmake, vim, etc.
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf \
    cmake \
    vim \
    git \
    libgpiod-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /project

# Copy your entire project into the container
COPY . /project

# Default to bash so you can explore or override CMD during `docker run`
CMD ["bash"]

