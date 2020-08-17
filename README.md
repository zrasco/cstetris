# cstetris
Tetris clone I made while teaching CS at the local community college here in Vegas. Dependencies: g++ and ncurses

Use the make command by itself to build. You will need to install the current dev library of ncurses for this to work.... at the time of this writing, I was able to successfully build with lib32ncurses5-dev.

To build/run in Ubuntu 18.04:

```
apt-get install build-essential lib32ncurses5-dev
make
./tetris
```
