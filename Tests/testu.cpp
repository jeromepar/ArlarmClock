/*
 * testu.cpp
 *
 *  Created on: 25 juin 2017
 *      Author: jj
 */

#include "catch.hpp"




unsigned int Factorial( unsigned int number ) {
	return number <= 1 ? number : Factorial(number-1)*number;
}

SCENARIO("Factorial computation","[factorial]"){
	GIVEN("A factorial implementation"){

		WHEN("call is made"){
			THEN( "values are fine" ) {
				REQUIRE(Factorial(0) == 1);
				REQUIRE(Factorial(1) == 1);
				REQUIRE( Factorial(2) == 2 );
			}
		}
	}
}
