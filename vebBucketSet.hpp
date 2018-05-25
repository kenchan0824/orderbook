
#ifndef ORDERBOOK_VEBBUCKETSET_H
#define ORDERBOOK_VEBBUCKETSET_H

#include <unordered_map>
#include "veb.h"

template< typename PriceBucketT = PriceBucket >
class vebBucketSet
{
	using MapType = std::unordered_map<uint64_t, std::shared_ptr<PriceBucketT>>;
public:
	vebBucketSet(int numBits) : m_veb{numBits} {}

    std::shared_ptr<PriceBucketT> find( uint64_t price )
    { 
    	if (m_veb.isMember(price))
    	{
    		auto it = m_map.find(price);
    		return it->second;
    	}
    	else
    		return nullptr;
	}

    std::shared_ptr<PriceBucketT> successor( uint64_t price )
    {
    	auto successor = m_veb.successor(price);
    	if ( successor == -1 )
    		return nullptr;

    	return m_map.find(successor)->second;
    }

    std::shared_ptr<PriceBucketT> predecessor( uint64_t price )
    {
    	auto predecessor = m_veb.predecessor(price);
    	if ( predecessor == -1 )
    		return nullptr;

    	return m_map.find(predecessor)->second;
    }

    std::shared_ptr<PriceBucketT> insert( typename MapType::value_type keyValPair )
    {
        // use the convenience of structured bindings offered by C++17

        auto [item, ok] = m_map.insert( keyValPair );
        if (ok)
        {
        	m_veb.insert(keyValPair.first);
        	return item->second;
        }
        else
        	return std::shared_ptr<PriceBucketT>();
    }

	void remove( uint64_t price ) 
	{
		m_veb.remove(price);  
		m_map.erase(price); 
	}
    
    uint64_t minPrice() { return m_veb.findMin(); }
    uint64_t maxPrice() { return m_veb.findMax(); }

private:
	veb m_veb;
	MapType m_map;
};

#endif