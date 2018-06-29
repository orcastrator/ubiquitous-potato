Server project to host ReSTful API calls to Bible verses

Uses Bible verses stored in JSON format
Includes mechanism for parsing reference strings

git submodule init
git submodule update
cd restbed
git submodule init
git submodule update
cd dependency/openssl
# I can't remember the config command to build openssl
mkdir build
cmake ..
cmake --build .
...
cd docopt.cpp
mkdir build
cmake .. -DCMAKE_INSTALL_PREFIX=../install #to be unified for both restbed and docopt.cpp repositories
cmake --build .

To set up project:
 - cmake .. -DBOOST_PATH=/path/to/boost/directory 

To build:
 - cmake --build .
