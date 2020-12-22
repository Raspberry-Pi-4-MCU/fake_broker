fake broker 
===

This repository contains fake broker.

## Feature:
- using tcp.
- not support ssl.

## Depend:

  - Install sockpp
```
git clone https://github.com/fpagliughi/sockpp
cd sockpp
mkdir build; cd build
cmake ..
make
sudo make install
sudo ldconfig
```

  - Install json
```
git clone https://github.com/nlohmann/json.git
cd json && cmake . && make -j4 && sudo make install
sudo ldconfig
```

## Install:
```
mkdir build
cd build
cmake ..
make
```


# Refenerce
* sockpp: https://github.com/fpagliughi/sockpp.git
* json: https://github.com/nlohmann/json.git
