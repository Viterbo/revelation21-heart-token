# revelation21-heart-token
Source code for the HEART standard eosio token that runs on Telos Network.

This work is an adaptation of the [WUBI project by Fabiana Cecin](https://github.com/fcecin/wubi);



----------
the following are comands from terminal with a local network and a contract version that has setted the April 29th (less than 48hs ago) as initial date for the UBI to take place.


The token creation

```bash
$ cleos push action revelation21 create '["revelation21", "2100000000.0000 HEART"]' -p revelation21@active
executed transaction: b13025a712097c1a64c34a7b6f5b25ea2bc0df090d0f800d1f18973530639314  120 bytes  865 us
#  revelation21 <= revelation21::create         {"issuer":"revelation21","maximum_supply":"2100000000.0000 HEART"}
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos get currency stats revelation21 HEART
{
  "HEART": {
    "supply": "0.0000 HEART",
    "max_supply": "2100000000.0000 HEART",
    "issuer": "revelation21"
  }
}
```
Let's see what happends when ordinary accounts try to open a balance in revelation21 contract 

```bash
$ cleos push action revelation21 open '["alice", "4,HEART", "alice"]' -p alice@active
executed transaction: b4e116926df0d287e94d06c249e1baca1f305359e961f1d590bb81dd9727d161  120 bytes  314 us
#  revelation21 <= revelation21::open           {"owner":"alice","symbol":"4,HEART","ram_payer":"alice"}
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos push action revelation21 open '["bob", "4,HEART", "bob"]' -p bob@active
executed transaction: f220bd77d5b23d2f60ad6c320bf2323f2d326819a16937529f8676bc3726ce08  120 bytes  233 us
#  revelation21 <= revelation21::open           {"owner":"bob","symbol":"4,HEART","ram_payer":"bob"}
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos push action revelation21 open '["tom", "4,HEART", "tom"]' -p tom@active
executed transaction: 78e667b03a95e8037f16e74ca924b688aceb406480c6d7227a0b089d710d9998  120 bytes  225 us
#  revelation21 <= revelation21::open           {"owner":"tom","symbol":"4,HEART","ram_payer":"tom"}
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
```
Now let's see what happends when .jc accounts try to open a balance in revelation21 contract. Note that less than 48h has passed from the initial UBI date (april 29th for this test)

```bash
$ cleos push action revelation21 open '["user1.jc", "4,HEART", "user1.jc"]' -p user1.jc@active
executed transaction: 7e9942dfb85226f696b384e7c7e59b5781dd1f3f8e73f64cc03d151de67d75f3  120 bytes  435 us
#  revelation21 <= revelation21::open           {"owner":"user1.jc","symbol":"4,HEART","ram_payer":"user1.jc"}
#  revelation21 <= revelation21::transfer       {"from":"user1.jc","to":"user1.jc","quantity":"1.0000 HEART","memo":"[HEART-UBI] +1.0000 HEART (next...
#      user1.jc <= revelation21::transfer       {"from":"user1.jc","to":"user1.jc","quantity":"1.0000 HEART","memo":"[HEART-UBI] +1.0000 HEART (next...
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos push action revelation21 open '["user2.jc", "4,HEART", "user2.jc"]' -p user2.jc@active
executed transaction: e0c5357332e743caa6f20308027a05843e816a859445f5178654c86181d6a0d2  120 bytes  435 us
#  revelation21 <= revelation21::open           {"owner":"user2.jc","symbol":"4,HEART","ram_payer":"user2.jc"}
#  revelation21 <= revelation21::transfer       {"from":"user2.jc","to":"user2.jc","quantity":"1.0000 HEART","memo":"[HEART-UBI] +1.0000 HEART (next...
#      user2.jc <= revelation21::transfer       {"from":"user2.jc","to":"user2.jc","quantity":"1.0000 HEART","memo":"[HEART-UBI] +1.0000 HEART (next...
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
```
As you can see, both accounts gain +1.0000 HEART.    
Now let's watch all balances

```bash
$ cleos get currency balance revelation21 alice HEART
0.0000 HEART
$ cleos get currency balance revelation21 bob HEART
0.0000 HEART
$ cleos get currency balance revelation21 tom HEART
0.0000 HEART
$ cleos get currency balance revelation21 user1.jc HEART
1.0000 HEART
$ cleos get currency balance revelation21 user2.jc HEART
1.0000 HEART
$ cleos get currency balance revelation21 user3.jc HEART
```
Now we will try what happneds when transfering HEARTs. Now we just pass 1 HEART to the **user3.jc** that will not trigger any UBI related action.

```bash
$ cleos push action revelation21 transfer '["user2.jc","user3.jc","1.0000 HEART",""]' -p user2.jc@active
executed transaction: c9ef26b130f78d3e1a0a2a4cf9f8bb4c387a070f305e50cb212f4cabb062f038  128 bytes  275 us
#  revelation21 <= revelation21::transfer       {"from":"user2.jc","to":"user3.jc","quantity":"1.0000 HEART","memo":""}
#      user2.jc <= revelation21::transfer       {"from":"user2.jc","to":"user3.jc","quantity":"1.0000 HEART","memo":""}
#      user3.jc <= revelation21::transfer       {"from":"user2.jc","to":"user3.jc","quantity":"1.0000 HEART","memo":""}
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos get currency balance revelation21 user2.jc HEART
0.0000 HEART
$ cleos get currency balance revelation21 user3.jc HEART
1.0000 HEART
```
Now **user3.jc** has positive balance but did't initiated his UBI state.     
Now we transfer from user3.jc to kate and back.

```bash
$ cleos push action revelation21 transfer '["user3.jc","kate","0.1000 HEART",""]' -p user3.jc@active
executed transaction: e26d3e6f551527995944589fbc358feb01a54dacdb4dc3ea1be5b10f2f405c5c  128 bytes  419 us
#  revelation21 <= revelation21::transfer       {"from":"user3.jc","to":"kate","quantity":"0.1000 HEART","memo":""}
#      user3.jc <= revelation21::transfer       {"from":"user3.jc","to":"kate","quantity":"0.1000 HEART","memo":""}
#          kate <= revelation21::transfer       {"from":"user3.jc","to":"kate","quantity":"0.1000 HEART","memo":""}
#  revelation21 <= revelation21::transfer       {"from":"user3.jc","to":"user3.jc","quantity":"1.0000 HEART","memo":"[HEART-UBI] +1.0000 HEART (next...
#      user3.jc <= revelation21::transfer       {"from":"user3.jc","to":"user3.jc","quantity":"1.0000 HEART","memo":"[HEART-UBI] +1.0000 HEART (next...
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos get currency balance revelation21 kate HEART
0.1000 HEART
$ cleos get currency balance revelation21 user3.jc HEART
1.9000 HEART
```
**user3.jc** just gained +1.0000 HEART. He started with 1.0000 HEART and ended with 1.900 HEART after transfer.

```bash
$ cleos push action revelation21 transfer '["kate","user3.jc","0.1000 HEART",""]' -p kate@active
executed transaction: 8c6c6117ff83dd00b448c8be45f868bb7a3de66e2d54183c7cfdd0b94dbf2f13  128 bytes  297 us
#  revelation21 <= revelation21::transfer       {"from":"kate","to":"user3.jc","quantity":"0.1000 HEART","memo":""}
#          kate <= revelation21::transfer       {"from":"kate","to":"user3.jc","quantity":"0.1000 HEART","memo":""}
#      user3.jc <= revelation21::transfer       {"from":"kate","to":"user3.jc","quantity":"0.1000 HEART","memo":""}
warning: transaction executed locally, but may not be confirmed by the network yet         ] 
$ cleos get currency balance revelation21 kate HEART
0.0000 HEART
$ cleos get currency balance revelation21 user3.jc HEART
2.0000 HEART
$ 
```
Now **kate** has returned the 0.1000 HEART to **user3.jc** ending with a balance of 0.0000 HEART because she has not a .jc account name.