#! /bin/bash

# run ./build.sh --docker-build to execute the build in a docker container
if [ $1 ] && [ $1 == "--docker-build" ]; then
  CURRENT_UID=$(id -u):$(id -g)

  echo "--- Building Docker Container"
  docker build ./docker -t atari-vcs-build-env:latest

  echo "--- Executing Build"
  docker run -it --user $CURRENT_UID \
    --rm -v $(pwd):/app \
    atari-vcs-build-env:latest ./build.sh
  exit
fi


###############################################################################
# CONFIGURATION
###############################################################################

###############################################################################
# Variables used for terminal coloring
###############################################################################
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

###############################################################################
# Helper functions
###############################################################################
function halt_build() {
 printf "\n${RED}######################${NC}\n"
 printf "${RED}# ${YELLOW}** Build failed ** ${RED}#\n"
 printf "${RED}######################${NC}\n"
 printf "Error:${NC} %s\n" "$1"
 exit 1
}

###############################################################################

###############################################################################
# Build & Run Unit Tests
###############################################################################
printf "\n--- Build & Test\n"

# Check if running in a container
if grep docker /proc/1/cgroup -qa; then
  mkdir -p cmake-build-docker && pushd cmake-build-docker
else
  mkdir -p cmake-build && pushd cmake-build
fi

cmake -DCMAKE_BUILD_TYPE=Coverage ../
make coverage || halt_build "build or test(s) failed"

cmake ../
make all

printf "\n+++ Check Test Coverage\n"

#Annoying grep to get coverage summary from index.html
cov=($(grep -oP -m 2 '("headerTableEntry" )[^ ]*' atari-2600-tests/coverage/index.html | cut -d '>' -f2))

printf "\n${GREEN}#############################${NC}\n"
printf "Line Coverage: %.2f%%\n" ${cov[0]}
printf "Branch Coverage: %.2f%%\n" ${cov[1]}
printf "${GREEN}#############################${NC}\n"

printf "\n--- Build Complete\n"

printf "\n${GREEN}#############################${NC}\n"
printf "${GREEN}# ${YELLOW}** Build Successful 👍 ** ${GREEN}#\n"
printf "${GREEN}#############################${NC}\n"
