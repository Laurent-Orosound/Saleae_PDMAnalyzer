#include "PDMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


PDMAnalyzerSettings::PDMAnalyzerSettings()
:	mClockChannel( UNDEFINED_CHANNEL ),
    mDataChannel( UNDEFINED_CHANNEL),
    mBitsPerSample( 64 ),
    mOffsetStart( 0 )
{
	mClockChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mClockChannelInterface->SetTitleAndTooltip( "Clock", "Clock" );
	mClockChannelInterface->SetChannel( mClockChannel );

	mDataChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mDataChannelInterface->SetTitleAndTooltip( "Data", "PDM Clock" );
	mDataChannelInterface->SetChannel( mDataChannel );

	mBitsPerSampleInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mBitsPerSampleInterface->SetTitleAndTooltip( "Bits per sample",  "Specify the number of individual bits that form a sample." );
	mBitsPerSampleInterface->SetMax( 255 );
	mBitsPerSampleInterface->SetMin( 1 );
	mBitsPerSampleInterface->SetInteger( mBitsPerSample );

	mOffsetStartInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mOffsetStartInterface->SetTitleAndTooltip( "Clock offset start",  "Specify the number of clock ticks to ignore at the beginning of the signal" );
	mOffsetStartInterface->SetMax( 255 );
	mOffsetStartInterface->SetMin( 0 );
	mOffsetStartInterface->SetInteger( mOffsetStart );

	AddInterface( mClockChannelInterface.get() );
	AddInterface( mDataChannelInterface.get() );
	AddInterface( mBitsPerSampleInterface.get() );
	AddInterface( mOffsetStartInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mClockChannel, "Clock", false );
	AddChannel( mDataChannel, "Data", false );
}

PDMAnalyzerSettings::~PDMAnalyzerSettings()
{
}

bool PDMAnalyzerSettings::SetSettingsFromInterfaces()
{
	mClockChannel = mClockChannelInterface->GetChannel();
	mDataChannel = mDataChannelInterface->GetChannel();
	mBitsPerSample = mBitsPerSampleInterface->GetInteger();
	mOffsetStart = mOffsetStartInterface->GetInteger();

	ClearChannels();
	AddChannel( mClockChannel, "Clock", true );
	AddChannel( mDataChannel, "Data", true );

	return true;
}

void PDMAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mClockChannelInterface->SetChannel( mClockChannel );
	mDataChannelInterface->SetChannel( mDataChannel );
	mBitsPerSampleInterface->SetInteger( mBitsPerSample );
	mOffsetStartInterface->SetInteger( mOffsetStart );
}

void PDMAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mClockChannel;
    text_archive >> mDataChannel;
	text_archive >> mBitsPerSample;
	text_archive >> mOffsetStart;

	ClearChannels();
	AddChannel( mClockChannel, "Clock", true );
    AddChannel( mDataChannel, "Data", true);

	UpdateInterfacesFromSettings();
}

const char* PDMAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mClockChannel;
    text_archive << mDataChannel;
	text_archive << mBitsPerSample;
	text_archive << mOffsetStart;

	return SetReturnString( text_archive.GetString() );
}
