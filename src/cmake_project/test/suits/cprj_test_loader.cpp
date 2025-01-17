#include "cmake_project/test/fixtures/cprj_test_loader_fixture.hpp"

#include "test_tools/test_macros.hpp"

#include <set>

/*------------------------------------------------------------------------------

TEST PLAN
1. One file
2. Several files
3. Includes

------------------------------------------------------------------------------*/

namespace cmake_project::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( CMakeProjectLoaderTests, LoaderFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_one_file )
{
	// Init
	addDefaultComandForFile( "file.cpp" );

	// Run
	load();

	// Check
	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( { "file.cpp" } );

	TEST_CHECK_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t2_several_files )
{
	// Init
	addDefaultComandForFile( "file1.cpp" );
	addDefaultComandForFile( "file2.cpp" );

	// Run
	load();

	// Check
	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( { "file1.cpp", "file2.cpp" } );

	TEST_CHECK_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t3_includes )
{
	// Init
	addComandWithIncludes( "file1.cpp", { "./lib1" } );
	addComandWithIncludes( "file2.cpp", { "./lib2", "./lib3" } );
	addComandWithIncludes( "file3.cpp", {} );
	addComandWithIncludes( "file4.cpp", { "./lib4" } );

	// Run
	load();

	// Check
	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( {
		"file1.cpp",
		"file2.cpp",
		"file3.cpp",
		"file4.cpp"
	} );

	TEST_REQUIRE_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);

	{
		const auto resultIncludes = getResultsIncludes( "file1.cpp" );
		const auto exceptedIncludes = toExceptedIncludes( { "./lib1" } );

		TEST_CHECK_EQUAL_COLLECTIONS(
			resultIncludes.begin(),
			resultIncludes.end(),
			exceptedIncludes.begin(),
			exceptedIncludes.end()
		);
	}
	{
		const auto resultIncludes = getResultsIncludes( "file2.cpp" );
		const auto exceptedIncludes = toExceptedIncludes( { "./lib2", "./lib3" } );

		TEST_CHECK_EQUAL_COLLECTIONS(
			resultIncludes.begin(),
			resultIncludes.end(),
			exceptedIncludes.begin(),
			exceptedIncludes.end()
		);
	}
	{
		const auto resultIncludes = getResultsIncludes( "file3.cpp" );
		const auto exceptedIncludes = toExceptedIncludes( {} );

		TEST_CHECK_EQUAL_COLLECTIONS(
			resultIncludes.begin(),
			resultIncludes.end(),
			exceptedIncludes.begin(),
			exceptedIncludes.end()
		);
	}
	{
		const auto resultIncludes = getResultsIncludes( "file4.cpp" );
		const auto exceptedIncludes = toExceptedIncludes( { "./lib4" } );

		TEST_CHECK_EQUAL_COLLECTIONS(
			resultIncludes.begin(),
			resultIncludes.end(),
			exceptedIncludes.begin(),
			exceptedIncludes.end()
		);
	}
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
