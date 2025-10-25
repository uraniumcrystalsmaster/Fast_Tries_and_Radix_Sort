#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "X-fast_Trie.h"

TEST_CASE("Trie insertion", "[insertion]"){
	XFastTrie<int> xft(64);
	size_t key2insert = 51;
	int value2insert = 33;
	xft.insert(key2insert, value2insert);

	REQUIRE(xft.contains(51));
}