#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include "test_tools/test_macros.hpp"

/*------------------------------------------------------------------------------

TEST PLAN:
1. Resolved includes
2. Unresolved includes
3. System includes
4. Relative paths

------------------------------------------------------------------------------*/

namespace reporter::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( DumpReporterTests, ReporterFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_includes )
{
	// Init
	const std::string classA = toPath( "/test_project/class_a.hpp" );
	const std::string classB = toPath( "/test_project/class_b.hpp" );
	const std::string main = toPath( "/test_project/main.cpp" );

	addInclude( main, classA, IncludeStatus::Resolved, IncludeType::User );
	addInclude( main, classB, IncludeStatus::Resolved, IncludeType::User );

	// Run
	std::string result = runDumpReporter();

	// Check
	BOOST_CHECK_EQUAL( result,
		"1 : " + classA + " ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : " + main + " ( type : user include status : resolved )\n"
		"2 : " + classB + " ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : " + main + " ( type : user include status : resolved )\n"
		"3 : " + main + " ( type: project file )\n"
		"\tIncludes:\n"
		"\t\t1 : " + classA + " ( type : user include status : resolved )\n"
		"\t\t2 : " + classB + " ( type : user include status : resolved )\n"
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t2_unresolved_includes )
{
	// Init
	const std::string classA = "class_a.hpp";
	const std::string classB = "class_b.hpp";
	const std::string main = toPath( "/test_project/main.cpp" );

	addInclude( main, classA, IncludeStatus::Unresolved, IncludeType::User );
	addInclude( main, classB, IncludeStatus::Unresolved, IncludeType::User );

	// Run
	std::string result = runDumpReporter();

	// Check

	BOOST_CHECK_EQUAL( result,
		"1 : " + classA + " ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : " + main + " ( type : user include status : unresolved )\n"
		"2 : " + classB + " ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : " + main + " ( type : user include status : unresolved )\n"
		"3 : " + main + " ( type: project file )\n"
		"\tIncludes:\n"
		"\t\t1 : " + classA + " ( type : user include status : unresolved )\n"
		"\t\t2 : " + classB + " ( type : user include status : unresolved )\n"
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t3_system_includes )
{
	// Init
	const std::string main = toPath( "/test_project/main.cpp" );
	addInclude( main, "string", IncludeStatus::Unresolved, IncludeType::System );
	addInclude( main, "memory", IncludeStatus::Unresolved, IncludeType::System );

	// Run
	std::string result = runDumpReporter();

	// Check
	BOOST_CHECK_EQUAL( result,
		"1 : memory ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : " + main + " ( type : system include status : unresolved )\n"
		"2 : string ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : " + main + " ( type : system include status : unresolved )\n"
		"3 : " + main + " ( type: project file )\n"
		"\tIncludes:\n"
		"\t\t1 : string ( type : system include status : unresolved )\n"
		"\t\t2 : memory ( type : system include status : unresolved )\n"
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t4_relative_paths )
{
	// Init
	const std::string classA = toPath( "/test_project/class_a.hpp" );
	const std::string classB = toPath( "/test_project/class_b.hpp" );
	const std::string main = toPath( "/test_project/main.cpp" );

	setProjectDir( "/test_project/" );

	addInclude( main, classA, IncludeStatus::Resolved, IncludeType::User );
	addInclude( main, classB, IncludeStatus::Resolved, IncludeType::User );

	// Run
	std::string result = runDumpReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"1 : class_a.hpp ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : main.cpp ( type : user include status : resolved )\n"
		"2 : class_b.hpp ( type: project file )\n"
		"\tIncluded by:\n"
		"\t\t1 : main.cpp ( type : user include status : resolved )\n"
		"3 : main.cpp ( type: project file )\n"
		"\tIncludes:\n"
		"\t\t1 : class_a.hpp ( type : user include status : resolved )\n"
		"\t\t2 : class_b.hpp ( type : user include status : resolved )\n"
	);
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
