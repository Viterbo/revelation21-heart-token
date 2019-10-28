NET=

if [ "$1" == "prod" ]; then
   NET='--url https://telos.eos.barcelona'
fi

if [ "$1" == "test" ]; then
   NET='--url https://testnet.telos.caleos.io'
fi


echo "-- accounts -------------------------------------"
echo "- gqydoobuhege -"
cleos $NET get table revelation21 gqydoobuhege accounts
echo "- heztanrrgyge -"
cleos $NET get table revelation21 heztanrrgyge accounts
echo "- fiscalesvote -"
cleos $NET get table revelation21 fiscalesvote accounts
echo "- viterbotelos -"
cleos $NET get table revelation21 viterbotelos accounts

echo "-- extras -------------------------------------"
echo "- gqydoobuhege -"
cleos $NET get table revelation21 gqydoobuhege extras
echo "- heztanrrgyge -"
cleos $NET get table revelation21 heztanrrgyge extras
echo "- fiscalesvote -"
cleos $NET get table revelation21 fiscalesvote extras
echo "- viterbotelos -"
cleos $NET get table revelation21 viterbotelos extras



# echo "-- accounts -------------------------------------"
# echo "- telosdouglas -"
# cleos $NET get table revelation21 telosdouglas accounts
# echo "- islandcrypto -"
# cleos $NET get table revelation21 islandcrypto accounts
# echo "- viterbotelos -"
# cleos $NET get table revelation21 viterbotelos accounts
# 
# echo "-- claims -------------------------------------"
# echo "- telosdouglas -"
# cleos $NET get table revelation21 telosdouglas claims
# echo "- islandcrypto -"
# cleos $NET get table revelation21 islandcrypto claims
# echo "- viterbotelos -"
# cleos $NET get table revelation21 viterbotelos claims


# echo "-- users -------------------------------------"
# cleos $NET get table acorntkntest acorntkntest users
# 
# echo "-- accounts -------------------------------------"
# echo "- gqydoobuhege -"
# cleos $NET get table acorntkntest gqydoobuhege accounts
# echo "- heztanrrgyge -"
# cleos $NET get table acorntkntest heztanrrgyge accounts
# echo "- fiscalesvote -"
# cleos $NET get table acorntkntest fiscalesvote accounts
# 
# echo "-- claims -------------------------------------"
# echo "- gqydoobuhege -"
# cleos $NET get table acorntkntest gqydoobuhege claims
# echo "- heztanrrgyge -"
# cleos $NET get table acorntkntest heztanrrgyge claims
# echo "- fiscalesvote -"
# cleos $NET get table acorntkntest fiscalesvote claims
