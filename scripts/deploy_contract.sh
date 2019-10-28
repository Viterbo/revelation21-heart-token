#!/bin/bash

force=false
NET=

if [ "$1" == "prod" ]; then
   NET='--url https://telos.eos.barcelona'
fi

if [ "$1" == "test" ]; then
   NET='--url https://testnet.telos.caleos.io'
fi

revelation21_HOME=../src/revelation21
echo "-------- revelation21 ---------"
cd $revelation21_HOME
eosio-cpp -o revelation21.wasm revelation21.cpp --abigen
echo "$ cleos $NET set contract revelation21 $PWD -p revelation21@active"
cleos $NET set contract revelation21 $PWD -p revelation21@active




