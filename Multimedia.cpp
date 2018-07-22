// #####################################
// #<<<<<< Mci Class Functions >>>>>>>>#
// #####################################
// #<<<<<<<< Include Files >>>>>>>>>>>>#
// #####################################
#include <Windows.h>
#include <mmsystem.h>
#include "Multimedia.h"

// #####################################
// #<<<<<<< Report Midi Error >>>>>>>>>#
// #####################################
void ShowError(DWORD dwError)
{
	char szErrorBuf[250];

   MessageBeep(MB_ICONEXCLAMATION);
   if(mciGetErrorString(dwError, szErrorBuf, MAXERRORLENGTH))
		MessageBox(GetFocus(), szErrorBuf, "MCI Error", MB_ICONEXCLAMATION);
	else
		MessageBox(GetFocus(), "Unknown Error", "MCI Error", MB_ICONEXCLAMATION);
};

// ##########################################################
// #<<<<<<<<<<<<<<<<< Midi Sequencer Class >>>>>>>>>>>>>>>>>#
// ##########################################################
// #####################################
// #<<<<<<<<< Initate Class >>>>>>>>>>>#
// #####################################
MCISEQ::MCISEQ()
{
};

// #####################################
// #<<<<<<<<< Destroy Class >>>>>>>>>>>#
// #####################################
MCISEQ::~MCISEQ()
{
};

// #####################################
// #<<<<<<<<<< Open Midi >>>>>>>>>>>>>>#
// #####################################
int MCISEQ::OpenMIDI(HWND hWindow, LPSTR MidiFile)
{
	MciOpenParms.lpstrDeviceType 	= TYPE_MIDI;
	MciOpenParms.lpstrElementName = MidiFile;

   dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
		(DWORD)(LPMCI_OPEN_PARMS) &MciOpenParms);

	if(dwReturn)
      ShowError(dwReturn);

   wDeviceID = MciOpenParms.wDeviceID;

   return 0;
};

// #####################################
// #<<<<<<<<<<<<< Play Midi >>>>>>>>>>>#
// #####################################
int MCISEQ::PlayMIDI(HWND hWindow)
{
   MciPlayParms.dwCallback = (DWORD)hWindow;

	dwReturn = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY,
					(DWORD)(LPMCI_PLAY_PARMS)&MciPlayParms);
	if(dwReturn)
		ShowError(dwReturn);

	return 0;
};

// #####################################
// #<<<<<<<<<<<<<< Stop Midi >>>>>>>>>>#
// #####################################
int MCISEQ::StopMIDI(HWND hWindow)
{
	dwReturn = mciSendCommand(wDeviceID, MCI_STOP, MCI_WAIT,
		(DWORD)(LPMCI_GENERIC_PARMS)&MciGenParms);

	if(dwReturn)
		ShowError(dwReturn);

	return 0;
};

// #####################################
// #<<<<<<<<<<<<< Pause Midi >>>>>>>>>>#
// #####################################
int MCISEQ::PauseMIDI(HWND hWindow)
{
	dwReturn = mciSendCommand (wDeviceID, MCI_PAUSE, MCI_WAIT,
		(DWORD)(LPMCI_GENERIC_PARMS)&MciGenParms);

	if(dwReturn)
   	ShowError(dwReturn);

	return 0;
};

// #####################################
// #<<<<<<<<<<<<< Resume Midi >>>>>>>>>#
// #####################################
int MCISEQ::ResumeMIDI(HWND hWindow)
{
	dwReturn = mciSendCommand (wDeviceID, MCI_RESUME, MCI_WAIT,
		(DWORD)(LPMCI_GENERIC_PARMS)&MciGenParms);

	if(dwReturn)
		ShowError(dwReturn);

	return 0;
};

// #####################################
// #<<<<<<<<<<<< Close Midi >>>>>>>>>>>#
// #####################################
int MCISEQ::CloseMIDI(HWND hWindow)
{
	dwReturn = mciSendCommand(wDeviceID, MCI_CLOSE, MCI_WAIT,
		(DWORD)(LPMCI_GENERIC_PARMS)&MciGenParms);

	if(dwReturn)
   	ShowError(dwReturn);

    return 0;
};

// ##########################################################
// #<<<<<<<<<<<<<<<<<<<< Wave Mix Class >>>>>>>>>>>>>>>>>>>>#
// ##########################################################
// #####################################
// #<<<<<<<<< Initate Class >>>>>>>>>>>#
// #####################################
/*WAVEMIX::WAVEMIX()
{
	WaveConfig.wSize 			= sizeof(MIXCONFIG);
	WaveConfig.dwFlags 		= WMIX_CONFIG_CHANNELS;
	WaveConfig.wChannels 	= 2; // Stereo

	MixSession = WaveMixConfigureInit(&WaveConfig);
};

// #####################################
// #<<<<<<<<< Destroy Class >>>>>>>>>>>#
// #####################################
WAVEMIX::~WAVEMIX()
{
	WaveMixCloseChannel(MixSession, 0, WMIX_ALL);
};

// #####################################
// #<<<<<<<<< Open Wave File >>>>>>>>>>#
// #####################################
LPMIXWAVE WAVEMIX::OpenWaveFile(char *File, int Channel)
{
	LPMIXWAVE	WaveFile;

	WaveFile = WaveMixOpenWave(MixSession, File, NULL, WMIX_FILE);
	WaveMixOpenChannel(MixSession, Channel, WMIX_OPENSINGLE);
};

// #####################################
// #<<<<<<<<< Play Wave File >>>>>>>>>>#
// #####################################
int WAVEMIX::PlayWaveFile(LPMIXWAVE WaveFile, int Channel)
{
	PlayParams.wSize 			= sizeof(MIXPLAYPARAMS);
	PlayParams.hMixSession 	= MixSession;
	PlayParams.hWndNotify 	= NULL;
	PlayParams.dwFlags 		= WMIX_CLEARQUEUE | WMIX_HIPRIORITY;
	PlayParams.wLoops 		= 0;
	PlayParams.lpMixWave 	= WaveFile;
	PlayParams.iChannel		= Channel;
	WaveMixPlay(&PlayParams);

   return 0;
};

// #####################################
// #<<<<<<<<< Free Wave File >>>>>>>>>>#
// #####################################
int WAVEMIX::FreeWaveFile(LPMIXWAVE WaveFile)
{
	WaveMixFreeWave(MixSession, WaveFile);
};*/
