#include "json/test/fixtures/json_test_fixture.hpp"

#include "json/api/json_array.hpp"
#include "json/api/json_object.hpp"
#include "json/api/json_value.hpp"

#include "test_tools/test_macros.hpp"

/*------------------------------------------------------------------------------

TEST PLAN:

1. Simple json
2. Bool
3. Int
4. String
5. Object
6. Array
	6.1 bool
	6.2 int
	6.3 string
	6.4 object
	6.5 root of document with objects
7. Not exist attribute

------------------------------------------------------------------------------*/

namespace json::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( JsonTests, JsonFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_simple_json )
{
	// Init
	createJsonFile(
	R"({
		"project_dir" : "./src",
		"file_extensions" : ["*.cpp", "*.hpp"],
		"include_dirs" : [".", "/usr/include/", "./3rd-part/cpp-std-fwd-master/"],
		"ignore_dirs" : ["./3rd-part"]
	})"
	);

	// Run
	const JsonObject & json = loadJson();

	// Check
	{
		BOOST_TEST_PASSPOINT();
		auto projectDirPtr = json.getAttributeValue( "project_dir" );
		TEST_REQUIRE( projectDirPtr );
		const JsonValue & projectDir = *projectDirPtr;
		TEST_CHECK_EQUAL( projectDir.asString(), "./src" );
	}
	{
		BOOST_TEST_PASSPOINT();
		auto fileExtensionsPtr = json.getAttributeValue( "file_extensions" );
		TEST_REQUIRE( fileExtensionsPtr );
		const JsonValue & fileExtensionsValue = *fileExtensionsPtr;
		auto fileExtensionsArrayPtr = fileExtensionsValue.asArray();
		TEST_REQUIRE( fileExtensionsArrayPtr );
		const JsonArray & fileExtensionsArray = *fileExtensionsArrayPtr;
		BOOST_REQUIRE_EQUAL( fileExtensionsArray.getSize(), 2 );
		{
			auto cppExtensionsPtr = fileExtensionsArray.at( 0 );
			TEST_REQUIRE( cppExtensionsPtr );
			const JsonValue & cppExtension = *cppExtensionsPtr;
			TEST_CHECK_EQUAL( cppExtension.asString(), "*.cpp" );
		}
		{
			auto hppExtensionsPtr = fileExtensionsArray.at( 1 );
			TEST_REQUIRE( hppExtensionsPtr );
			const JsonValue & hppExtension = *hppExtensionsPtr;
			TEST_CHECK_EQUAL( hppExtension.asString(), "*.hpp" );
		}
	}
	{
		BOOST_TEST_PASSPOINT();
		auto includeDirsPtr = json.getAttributeValue( "include_dirs" );
		TEST_REQUIRE( includeDirsPtr );
		const JsonValue & includeDirsValue = *includeDirsPtr;
		auto includeDirsArrayPtr = includeDirsValue.asArray();
		TEST_REQUIRE( includeDirsArrayPtr );
		const JsonArray & includeDirsArray = *includeDirsArrayPtr;
		BOOST_REQUIRE_EQUAL( includeDirsArray.getSize(), 3 );
		{
			auto dir1Ptr = includeDirsArray.at( 0 );
			TEST_REQUIRE( dir1Ptr );
			const JsonValue & dir1 = *dir1Ptr;
			TEST_CHECK_EQUAL( dir1.asString(), "." );
		}
		{
			auto dir2Ptr = includeDirsArray.at( 1 );
			TEST_REQUIRE( dir2Ptr );
			const JsonValue & dir2 = *dir2Ptr;
			TEST_CHECK_EQUAL( dir2.asString(), "/usr/include/" );
		}
		{
			auto dir3Ptr = includeDirsArray.at( 2 );
			TEST_REQUIRE( dir3Ptr );
			const JsonValue & dir3 = *dir3Ptr;
			TEST_CHECK_EQUAL( dir3.asString(), "./3rd-part/cpp-std-fwd-master/" );
		}
	}
	{
		BOOST_TEST_PASSPOINT();
		auto ignoreDirsPtr = json.getAttributeValue( "ignore_dirs" );
		TEST_REQUIRE( ignoreDirsPtr );
		const JsonValue & ignoreDirsValue = *ignoreDirsPtr;
		auto ignoreDirsArrayPtr = ignoreDirsValue.asArray();
		TEST_REQUIRE( ignoreDirsArrayPtr );
		const JsonArray & ignoreDirsArray = *ignoreDirsArrayPtr;
		BOOST_REQUIRE_EQUAL( ignoreDirsArray.getSize(), 1 );
		{
			auto dir1Ptr = ignoreDirsArray.at( 0 );
			TEST_REQUIRE( dir1Ptr );
			const JsonValue & dir1 = *dir1Ptr;
			TEST_CHECK_EQUAL( dir1.asString(), "./3rd-part" );
		}
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t2_bool )
{
	// Init
	createJsonFile( R"({ "atr1":true, "atr2":false })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	{
		BOOST_TEST_PASSPOINT();
		auto atr1Ptr = json.getAttributeValue( "atr1" );
		TEST_REQUIRE( atr1Ptr );
		const JsonValue & atr1 = *atr1Ptr;
		TEST_CHECK_EQUAL( atr1.asBool(), true );
	}

	{
		BOOST_TEST_PASSPOINT();
		auto atr2Ptr = json.getAttributeValue( "atr2" );
		TEST_REQUIRE( atr2Ptr );
		const JsonValue & atr2 = *atr2Ptr;
		TEST_CHECK_EQUAL( atr2.asBool(), false );
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t3_int )
{
	// Init
	createJsonFile( R"({ "val":42 })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "val" );
	TEST_REQUIRE( atrPtr );
	const JsonValue & val = *atrPtr;
	TEST_CHECK_EQUAL( val.asInt(), 42 );
}

//------------------------------------------------------------------------------

TEST_CASE( t4_string )
{
	// Init
	createJsonFile( R"({ "name": "str" })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "name" );
	TEST_REQUIRE( atrPtr );
	const JsonValue & val = *atrPtr;
	TEST_CHECK_EQUAL( val.asString(), "str" );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_object )
{
	// Init
	createJsonFile( R"({ "object" : { "name" : "str" } })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "object" );
	TEST_REQUIRE( atrPtr );

	auto objectPtr = atrPtr->asObject();
	TEST_REQUIRE( objectPtr );

	auto namePtr = objectPtr->getAttributeValue( "name" );
	TEST_REQUIRE( namePtr );

	TEST_CHECK_EQUAL( namePtr->asString(), "str" );
}

//------------------------------------------------------------------------------

TEST_CASE( t6_1_array_bool )
{
	// Init
	createJsonFile( R"({ "array" : [true,false] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	TEST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	TEST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		TEST_REQUIRE( valPtr );
		TEST_CHECK_EQUAL( valPtr->asBool(), true );
	}
	{
		auto valPtr = array.at( 1 );
		TEST_REQUIRE( valPtr );
		TEST_CHECK_EQUAL( valPtr->asBool(), false );
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t6_2_array_int )
{
	// Init
	createJsonFile( R"({ "array" : [42,43] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	TEST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	TEST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		TEST_REQUIRE( valPtr );
		TEST_CHECK_EQUAL( valPtr->asInt(), 42 );
	}
	{
		auto valPtr = array.at( 1 );
		TEST_REQUIRE( valPtr );
		TEST_CHECK_EQUAL( valPtr->asInt(), 43 );
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t6_3_array_string )
{
	// Init
	createJsonFile( R"({ "array" : ["str1","str2"] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	TEST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	TEST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		TEST_REQUIRE( valPtr );
		TEST_CHECK_EQUAL( valPtr->asString(), "str1" );
	}
	{
		auto valPtr = array.at( 1 );
		TEST_REQUIRE( valPtr );
		TEST_CHECK_EQUAL( valPtr->asString(), "str2" );
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t6_4_array_object )
{
	// Init
	createJsonFile( R"({ "array" : [ { "name": "str1" }, { "name": "str2" }] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	TEST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	TEST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		TEST_REQUIRE( valPtr );

		auto objectPtr = valPtr->asObject();
		TEST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		TEST_CHECK_EQUAL( namePtr->asString(), "str1" );
	}
	{
		auto valPtr = array.at( 1 );
		TEST_REQUIRE( valPtr );

		auto objectPtr = valPtr->asObject();
		TEST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		TEST_CHECK_EQUAL( namePtr->asString(), "str2" );
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t6_4_array_root_of_object_with_objects )
{
	// Init
	createJsonFile(
	R"([
	{
		"name" : "str1"
	},
	{
		"name" : "str2"
	}
	])"
	);

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto arrayValuePtr = json.asValue();
	TEST_REQUIRE( arrayValuePtr );

	auto arrayPtr = arrayValuePtr->asArray();
	TEST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		TEST_REQUIRE( valPtr );

		auto objectPtr = valPtr->asObject();
		TEST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		TEST_CHECK_EQUAL( namePtr->asString(), "str1" );
	}
	{
		auto valPtr = array.at( 1 );
		TEST_REQUIRE( valPtr );

		auto objectPtr = valPtr->asObject();
		TEST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		TEST_CHECK_EQUAL( namePtr->asString(), "str2" );
	}
}

//------------------------------------------------------------------------------

TEST_CASE( t7_not_exist_attribute )
{
	// Init
	createJsonFile( R"({ "name": "str" })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "surname" );
	TEST_REQUIRE( !atrPtr );
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
