#include "application/tools/app_report_settings_loader.hpp"

#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "reporter/api/rp_factory.hpp"
#include "reporter/api/rp_settings.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <vector>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

ReportSettingsLoader::ReportSettingsLoader(
	reporter::Factory & _reporterFactory
)
	:	m_reporterFactory{ _reporterFactory }
{
}

//------------------------------------------------------------------------------

ReportSettingsLoader::SettingsPtr ReportSettingsLoader::load(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile
)
{
	auto settingsPtr = createSettings();
	INTERNAL_CHECK_WARRING( settingsPtr );
	if( !settingsPtr )
	{
		return nullptr;
	}

	reporter::Settings & settings = *settingsPtr;

	const auto maxFiles = loadMaxFilesCount( _arguments, _configurationFile );
	const auto maxDetails = loadMaxDetailsCount( _arguments, _configurationFile );
	const bool showStdFiles = loadShowStdFiles( _arguments, _configurationFile );

	settings.setMaxFilesCount( maxFiles );
	settings.setMaxDetailsCount( maxDetails );
	settings.setShowStdFiles( showStdFiles );

	return settingsPtr;
}

//------------------------------------------------------------------------------

ReportSettingsLoader::ReporterKinds ReportSettingsLoader::loadReports(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile
)
{
	if( auto reportsOpt = _arguments.getReporterKinds(); reportsOpt )
	{
		return *reportsOpt;
	}

	if( _configurationFile != nullptr )
	{
		if( auto kindOpt = _configurationFile->getReporterKinds(); kindOpt )
		{
			return *kindOpt;
		}
	}

	return _arguments.getDefaultReporterKinds();
}

//------------------------------------------------------------------------------

ReportSettingsLoader::SettingsPtr ReportSettingsLoader::createSettings()
{
	return m_reporterFactory.createSettings();
}

//------------------------------------------------------------------------------

ReportSettingsLoader::CountType ReportSettingsLoader::loadMaxFilesCount(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile
)
{
	return getValue(
		_arguments,
		&ParserArgWrapper::getReportLimit,
		&ParserArgWrapper::getDefaultReportLimit,
		_configurationFile,
		&ConfigurationFile::getReportLimit
	);
}

//------------------------------------------------------------------------------

ReportSettingsLoader::CountType ReportSettingsLoader::loadMaxDetailsCount(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile
)
{
	return getValue(
		_arguments,
		&ParserArgWrapper::getReportDetailsLimit,
		&ParserArgWrapper::getDefaultReportDetailsLimit,
		_configurationFile,
		&ConfigurationFile::getReportDetailsLimit
	);
}

//------------------------------------------------------------------------------

bool ReportSettingsLoader::loadShowStdFiles(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile
)
{
	return getValue(
		_arguments,
		&ParserArgWrapper::getShowStdFile,
		&ParserArgWrapper::getDefaultShowStdfile,
		_configurationFile,
		&ConfigurationFile::getShowStdFiles
	);
}

//------------------------------------------------------------------------------

template< typename _ValueType >
_ValueType ReportSettingsLoader::getValue(
	const ParserArgWrapper & _arguments,
	std::optional< _ValueType > ( ParserArgWrapper::*_getValueFromArg )() const,
	_ValueType ( ParserArgWrapper::*_getDefaultValueFromArg )() const,
	const ConfigurationFile * _configurationFile,
	std::optional< _ValueType > (ConfigurationFile::*_getValueFromFile)() const
)
{
	if( auto valueFromArgOpt = ( _arguments.*_getValueFromArg )(); valueFromArgOpt )
	{
		return *valueFromArgOpt;
	}

	if( _configurationFile )
	{
		auto valueFromFileOpt = ( *_configurationFile.*_getValueFromFile )();
		if( valueFromFileOpt )
		{
			return *valueFromFileOpt;
		}
	}

	return ( _arguments.*_getDefaultValueFromArg )();
}

//------------------------------------------------------------------------------

}
