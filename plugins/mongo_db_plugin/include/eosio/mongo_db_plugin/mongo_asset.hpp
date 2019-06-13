/**
 *  @file
 *  @copyright defined in eos/LICENSE
 */
#pragma once

#include <eosio/chain/asset.hpp>
#include <fc/reflect/reflect.hpp>

namespace eosio { namespace chain {

  template <typename T>
  inline fc::variant variant_from_stream(fc::datastream<const char*>& ds) {
    T temp;
    fc::raw::unpack( ds, temp );
    return fc::variant(temp);
  }

  template <typename T>
  auto pack_unpack() {
    return std::make_pair<abi_serializer::unpack_function, abi_serializer::pack_function>(
      [](fc::datastream<const char*>& ds, bool isarr, bool isopt) -> fc::variant {
        if (isarr) {
          return variant_from_stream<vector<T>>(ds);
        } else if (isopt) {
          return variant_from_stream<optional<T>>(ds);
        }
        return variant_from_stream<T>(ds);
      },
      [](const fc::variant& var, fc::datastream<char*>& ds, bool isarr, bool isopt) {
        if (isarr) {
          return fc::raw::pack(ds, var.as<vector<T>>());
        } else if (isopt) {
          return fc::raw::pack(ds, var.as<optional<T>>());
        } else {
          return fc::raw::pack(ds, var.as<T>());
        }
      }
    );
  }

  struct mongo_asset : fc::reflect_init {
    public:
      asset to_asset() const { return asset(amount, sym); }
      friend struct fc::reflector<mongo_asset>;

      void reflector_init() const {
        auto tmp = to_asset();
      }
    private:
      share_type amount;
      symbol sym;
  };

}} /// namespace eosio::chain

FC_REFLECT(eosio::chain::mongo_asset, (amount)(sym))

