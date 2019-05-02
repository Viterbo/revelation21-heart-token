/**
 * This is a derivative of eosio.token that pays (issues) one token per day per account since 
 *   the day the account is opened or makes its first transfer. The UBI is paid on open() and 
 *   on transfer() for the originating ("from") account.
 * 
 * When an UBI is paid, it is paid for all of the unclaimed days in the past up to the next 
 *   30 days so one does not have to claim them every day to accumulate a significant balance.
 *
 * To create an uncapped supply token, just set the max_supply to 2^62 which is the maximum 
 *   supported (taking into account the digits spent on the token precision). For a precision
 *   of 4, that's around 461 trillion tokens. 
 */
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/transaction.hpp>

#include <string>

namespace eosiosystem {
    class system_contract;
}

namespace eosio {

    using std::string;

    class [[eosio::contract("revelation21")]] token : public contract {
        public:
            using contract::contract;

            [[eosio::action]]
            void create(name issuer, asset maximum_supply);

            [[eosio::action]]
            void issue( name to, asset quantity, string memo );

            [[eosio::action]]
            void retire( asset quantity, string memo );

            [[eosio::action]]
            void transfer( name from, name to, asset quantity, string  memo );

            [[eosio::action]]
            void open( name owner, const symbol& symbol, name ram_payer );

            [[eosio::action]]
            void close( name owner, const symbol& symbol );
    
            static asset get_supply( name token_contract_account, symbol_code sym_code ) {
                stats statstable( token_contract_account, sym_code.raw() );
                const auto& st = statstable.get( sym_code.raw() );
                return st.supply;
            }

            static asset get_balance( name token_contract_account, name owner, symbol_code sym_code ) {
                accounts accountstable( token_contract_account, owner.value );
                const auto& ac = accountstable.get( sym_code.raw() );
                return ac.balance;
            }

        private:
            typedef uint16_t time_type;

            struct [[eosio::table]] account {
                asset     balance;
                uint64_t primary_key()const { return balance.symbol.code().raw(); }
            };

            struct [[eosio::table]] currency_stats {
                asset    supply;
                asset    max_supply;
                name     issuer;

                uint64_t primary_key()const { return supply.symbol.code().raw(); }
            };

            typedef eosio::multi_index< "accounts"_n, account > accounts;
            typedef eosio::multi_index< "stat"_n, currency_stats > stats;

            void sub_balance( name owner, asset value );
            void add_balance( name owner, asset value, name ram_payer );

            // Unfortunately, we need to waste a ton of space to store an extra 16 bits per user account
            //    to conform to the interface expected by eosio.token.
            // As with the "account" struct, the scope is recycled as the token holder, and the primary
            //    key of our structure is the token symbol code.
            struct [[eosio::table]] extra {
                uint64_t  symbol_code_raw;
                time_type last_claim_day;

                uint64_t primary_key()const { return symbol_code_raw; }
            };

            typedef eosio::multi_index< "extras"_n, extra > extras;

            void create_extra_record( name owner, name ram_payer, uint64_t sym_code_raw );

            void try_ubi_claim( name from, const symbol& sym, name payer, stats& statstable, const currency_stats& st );

            void log_claim( name claimant, asset claim_quantity, time_type next_last_claim_day, time_type lost_days );
        
            static int64_t get_precision_multiplier ( const symbol& symbol ) {
                int64_t precision_multiplier = 1;
                for (int i=0; i<symbol.precision(); ++i) {
                    precision_multiplier *= 10;
                }
                return precision_multiplier;
            }

            static string days_to_string( int64_t days );
        
            static time_type get_today() { return (time_type)(current_time() / 86400000000); }

            // Before deploying this contract to your blockchain, make sure this function is doing what you want.
            // If you don't have KYC or any sort of ID check or de-duplication mechanism, this just returns true.
            // Otherwise, you will want to check whether an account likely belongs to an uniquified person or not.
            // Accounts not authorized to receive UBI at one point in time can be authorized to receive it at any
            //   point in the future (and have that authority randomly rescinded and granted again an unlimited
            //   number of times, at any time) and the contract will continue to work normally. Users who are
            //   later KYC'd will be able to claim up to "max_past_claim_days" days of back pay.
            bool can_claim_UBI( name claimant ) {
                // for HEART token UBI username MUST end with ".jc"
                string str = claimant.to_string();
                int len = str.size();
                if (str[len-3] != '.') return false;
                if (str[len-2] != 'j') return false;
                if (str[len-1] != 'c') return false;
                return true;
            }
        
            // Set this to true if your blockchain allows an user to freely create and destroy the types of accounts
            //   that will be authorized to claim UBI.
            // This will ensure that newly created token accounts have a two-day grace period before they can claim
            //   any UBI (that is measured as whole days, so if you create an account at 11:59:59 PM, you have to wait
            //   one day plus one second, which is why we require two whole days). That's a bit inconvenient for the
            //   user, but it is necessary to avoid completely unbounded, zero-cost money printing.
            static const bool unbounded_UBI_account_creation = false; 
            
            // When UBI can be claimed, claim this amount of days. "1" means today's income only, and from 2 and
            //   onwards you are granting advance payments for future days.
            // For non-KYC, unbounded-account-creation EOSIO public deployments, you want to set this to 1.
            // For a proper KYC chain where users can't continuously create and destroy their KYC accounts to
            //   reset them, you can set this to 30 to grant users an entire month's worth of UBI at a time.
            // A value of zero (or less) is not supported by the current code.
            static const int64_t claim_days = 1;

            // Unclaimed UBI accumulates to this maximum days.
            static const int64_t max_past_claim_days = 36000;
        };

} /// namespace eosio
