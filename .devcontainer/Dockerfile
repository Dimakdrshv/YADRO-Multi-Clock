FROM ubuntu:24.04

# Install the necessary packages
RUN apt update
RUN apt install -y --no-install-recommends \
    wget \
    ca-certificates \
    git \
    make \
    cmake \
    ninja-build \
    python3 \
    openocd \
    picocom \
    zsh \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

ENV GIT_SSL_NO_VERIFY=true

# Disable printing "Entering directory" message in GNU Make
ENV GNUMAKEFLAGS=--no-print-directory

# Install Oh My Zsh
RUN sh -c "$(wget --no-check-certificate -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended && \
    git clone https://github.com/zsh-users/zsh-autosuggestions ~/.oh-my-zsh/custom/plugins/zsh-autosuggestions
ENV SHELL=/bin/zsh

# Create dir for MIK32 utils
RUN mkdir mik32_utils
WORKDIR /mik32_utils

# Download and extract toolchain for MIK32
RUN wget --no-check-certificate https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack/releases/download/v14.2.0-3/xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz && \
    tar -xvzf xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz && \
    rm -rf xpack-riscv-none-elf-gcc-14.2.0-3-linux-x64.tar.gz

# Set toolchain path for build system
ENV MIK32_TOOLCHAIN_DIR=/mik32_utils/xpack-riscv-none-elf-gcc-14.2.0-3/bin
ENV MIK32_UPLOADER_DIR=/mik32_utils/mik32-uploader

# Download MIK32 uploader
RUN git clone https://github.com/MikronMIK32/mik32-uploader.git

# Config git for ignore file mode and line endings (otherwise there will be a lot of false changes in the files)
RUN git config --global core.filemode false && \
    git config --global core.autocrlf true
