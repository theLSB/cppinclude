#pragma once

#include <exception>
#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class Log
{
public:
	Log();
	Log( std::ostream & _stream, std::ostream & _errorStream );

	void printLine( std::string_view _line );
	void printWarning( const std::exception & _exception );

private:
	std::ostream & m_stream;
	std::ostream & m_errorStream;
};

//------------------------------------------------------------------------------

}
