# Project for Models and simulations class on BUT FIT Brno
This project is the implemention of an autoregressive model (AR for short).
The AR model was edited to better suit our needs, that means we are using autocorrelation
instead of partial autocorrelation, we are omiting the error factor and we do not use beta0.

## Evaluation
Our project scored 20/20 points.

## Specification of our project
We had to make a model about European Green Deal, so we decided to make a
model to calculate how many electric cars will there be in the Czech republic in 2030. We took data about
electric cars sales prior to November 2021 from the czech website SDA. The result of our calculation
was compared with the estimation of the Czech goverment, published in Národní plán which addressed
demands of the EU on reduction of polution.

## How to run project
You can clone our repo and then use `make run` in the root directory. The data of cars sold
is stored in `./src/input_data.txt` so new value (for example for December 2021) can be
added at the end of this file. Our program contains some hardcoded values. They are described
at the start of `./src/main.cpp`. Program outputs the results of 3 experiments. For further
information there is `dokumentace.pdf` which contains simulation protocol for this project.
It is written in czech.

## Original assignment
### EU Green Deal a jeho dopady na ekonomiku EU
Prostudujte Green Deal a co vlastně má v jednotlivých zemích EU znamenat pro průmysl, dopravu, energetiku, životní prostředí, ekonomiku, apod.
Na modelech ukažte proveditelnost/neproveditelnost GD ve vybraných zemích a aspektech.
