#include "json/test/fixtures/json_test_fixture.hpp"

#include "json/api/json_object.hpp"
#include "json/api/json_value.hpp"
#include "json/api/json_array.hpp"

#include <boost/test/unit_test.hpp>

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
7. Not exist attribute

------------------------------------------------------------------------------*/

namespace json::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(JsonTests, JsonFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(simple_json_1)
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
		BOOST_REQUIRE( projectDirPtr );
		const JsonValue & projectDir = *projectDirPtr;
		BOOST_CHECK_EQUAL( projectDir.asString(), "./src" );
	}
	{

		BOOST_TEST_PASSPOINT();
		auto fileExtentionsPtr = json.getAttributeValue( "file_extensions" );
		BOOST_REQUIRE( fileExtentionsPtr );
		const JsonValue & fileExtentionsValue = *fileExtentionsPtr;
		auto fileExtentionsArrayPtr = fileExtentionsValue.asArray();
		BOOST_REQUIRE( fileExtentionsArrayPtr );
		const JsonArray & fileExtentionsArray = *fileExtentionsArrayPtr;
		BOOST_REQUIRE_EQUAL( fileExtentionsArray.getSize(), 2 );
		{
			auto cppExtentionsPtr = fileExtentionsArray.at( 0 );
			BOOST_REQUIRE( cppExtentionsPtr );
			const JsonValue & cppExtention = *cppExtentionsPtr;
			BOOST_CHECK_EQUAL( cppExtention.asString(), "*.cpp" );
		}
		{
			auto hppExtentionsPtr = fileExtentionsArray.at( 1 );
			BOOST_REQUIRE( hppExtentionsPtr );
			const JsonValue & hppExtention = *hppExtentionsPtr;
			BOOST_CHECK_EQUAL( hppExtention.asString(), "*.hpp" );
		}
	}
	{
		BOOST_TEST_PASSPOINT();
		auto includeDirsPtr = json.getAttributeValue( "include_dirs" );
		BOOST_REQUIRE( includeDirsPtr );
		const JsonValue & includeDirsValue = *includeDirsPtr;
		auto includeDirsArrayPtr = includeDirsValue.asArray();
		BOOST_REQUIRE( includeDirsArrayPtr );
		const JsonArray & includeDirsArray = *includeDirsArrayPtr;
		BOOST_REQUIRE_EQUAL( includeDirsArray.getSize(), 3 );
		{
			auto dir1Ptr = includeDirsArray.at( 0 );
			BOOST_REQUIRE( dir1Ptr );
			const JsonValue & dir1 = *dir1Ptr;
			BOOST_CHECK_EQUAL( dir1.asString(), "." );
		}
		{
			auto dir2Ptr = includeDirsArray.at( 1 );
			BOOST_REQUIRE( dir2Ptr );
			const JsonValue & dir2 = *dir2Ptr;
			BOOST_CHECK_EQUAL( dir2.asString(), "/usr/include/" );
		}
		{
			auto dir3Ptr = includeDirsArray.at( 2 );
			BOOST_REQUIRE( dir3Ptr );
			const JsonValue & dir3 = *dir3Ptr;
			BOOST_CHECK_EQUAL( dir3.asString(), "./3rd-part/cpp-std-fwd-master/" );
		}
	}
	{
		BOOST_TEST_PASSPOINT();
		auto ignoreDirsPtr = json.getAttributeValue( "ignore_dirs" );
		BOOST_REQUIRE( ignoreDirsPtr );
		const JsonValue & ignoreDirsValue = *ignoreDirsPtr;
		auto ignoreDirsArrayPtr = ignoreDirsValue.asArray();
		BOOST_REQUIRE( ignoreDirsArrayPtr );
		const JsonArray & ignoreDirsArray = *ignoreDirsArrayPtr;
		BOOST_REQUIRE_EQUAL( ignoreDirsArray.getSize(), 1 );
		{
			auto dir1Ptr = ignoreDirsArray.at( 0 );
			BOOST_REQUIRE( dir1Ptr );
			const JsonValue & dir1 = *dir1Ptr;
			BOOST_CHECK_EQUAL( dir1.asString(), "./3rd-part" );
		}
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(bool_2)
{
	// Init
	createJsonFile( R"({ "atr1":true, "atr2":false })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	{
		BOOST_TEST_PASSPOINT();
		auto atr1Ptr = json.getAttributeValue( "atr1" );
		BOOST_REQUIRE( atr1Ptr );
		const JsonValue & atr1 = *atr1Ptr;
		BOOST_CHECK_EQUAL( atr1.asBool(), true );
	}

	{
		BOOST_TEST_PASSPOINT();
		auto atr2Ptr = json.getAttributeValue( "atr2" );
		BOOST_REQUIRE( atr2Ptr );
		const JsonValue & atr2 = *atr2Ptr;
		BOOST_CHECK_EQUAL( atr2.asBool(), false );
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(int_3)
{
	// Init
	createJsonFile( R"({ "val":42 })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "val" );
	BOOST_REQUIRE( atrPtr );
	const JsonValue & val = *atrPtr;
	BOOST_CHECK_EQUAL( val.asInt(), 42 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(string_4)
{
	// Init
	createJsonFile( R"({ "name": "str" })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "name" );
	BOOST_REQUIRE( atrPtr );
	const JsonValue & val = *atrPtr;
	BOOST_CHECK_EQUAL( val.asString(), "str" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(object_5)
{
	// Init
	createJsonFile( R"({ "object" : { "name" : "str" } })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "object" );
	BOOST_REQUIRE( atrPtr );

	auto objectPtr = atrPtr->asObject();
	BOOST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		BOOST_REQUIRE( namePtr );

		BOOST_CHECK_EQUAL( namePtr->asString(), "str" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(array_bool_6_1)
{
	// Init
	createJsonFile( R"({ "array" : [true,false] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	BOOST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	BOOST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		BOOST_REQUIRE( valPtr );
		BOOST_CHECK_EQUAL( valPtr->asBool(), true );
	}
	{
		auto valPtr = array.at( 1 );
		BOOST_REQUIRE( valPtr );
		BOOST_CHECK_EQUAL( valPtr->asBool(), false );
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(array_int_6_2)
{
	// Init
	createJsonFile( R"({ "array" : [42,43] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	BOOST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	BOOST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		BOOST_REQUIRE( valPtr );
		BOOST_CHECK_EQUAL( valPtr->asInt(), 42 );
	}
	{
		auto valPtr = array.at( 1 );
		BOOST_REQUIRE( valPtr );
		BOOST_CHECK_EQUAL( valPtr->asInt(), 43 );
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(array_string_6_3)
{
	// Init
	createJsonFile( R"({ "array" : ["str1","str2"] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	BOOST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	BOOST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		BOOST_REQUIRE( valPtr );
		BOOST_CHECK_EQUAL( valPtr->asString(), "str1" );
	}
	{
		auto valPtr = array.at( 1 );
		BOOST_REQUIRE( valPtr );
		BOOST_CHECK_EQUAL( valPtr->asString(), "str2" );
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(array_object_6_4)
{
	// Init
	createJsonFile( R"({ "array" : [ { "name": "str1" }, { "name": "str2" }] })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto attributePtr = json.getAttributeValue( "array" );
	BOOST_REQUIRE( attributePtr );

	auto arrayPtr = attributePtr->asArray();
	BOOST_REQUIRE( arrayPtr );
	const JsonArray & array = *arrayPtr;

	BOOST_REQUIRE_EQUAL( array.getSize(), 2 );
	{
		auto valPtr = array.at( 0 );
		BOOST_REQUIRE( valPtr );

		auto objectPtr = valPtr->asObject();
		BOOST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		BOOST_CHECK_EQUAL( namePtr->asString(), "str1" );
	}
	{
		auto valPtr = array.at( 1 );
		BOOST_REQUIRE( valPtr );

		auto objectPtr = valPtr->asObject();
		BOOST_REQUIRE( objectPtr );

		auto namePtr = objectPtr->getAttributeValue( "name" );
		BOOST_CHECK_EQUAL( namePtr->asString(), "str2" );
	}

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(not_exist_attribute_7)
{
	// Init
	createJsonFile( R"({ "name": "str" })" );

	// Run
	const JsonObject & json = loadJson();

	// Check
	auto atrPtr = json.getAttributeValue( "surname" );
	BOOST_REQUIRE( !atrPtr );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
