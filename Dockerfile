# Use an official Debian as a parent image
FROM debian:12

# Install dependencies
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    build-essential

# Set the working directory
WORKDIR /app

# Clone the repository
RUN git clone https://github.com/AnKozhevnikov/cpp-course-fluid-hw2 .

# Set the build directory as the working directory
WORKDIR /app/build

# Run CMake to configure the project
RUN cmake ..

# Build the project
RUN make

# Run the built executable
CMD ["./fluid", "--p-type=FIXED(32,16)", "--v-type=FIXED(32,16)", "--v-flow-type=FIXED(32,16)", "--source=field.txt"]