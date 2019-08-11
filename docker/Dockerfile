# Use an official gcc image as base
FROM gcc:7.2

# Install deps
RUN apt-get update && apt-get install -y \
    cmake \
    gcovr

# Set entry point
WORKDIR /app
