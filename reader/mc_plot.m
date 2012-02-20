close all
clear

load -ascii block_count.mat
plot(block_count(6:128,[14 15 16 56]+1))
wait
