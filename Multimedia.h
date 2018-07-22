// #####################################
// #<<<<<< Mci Class Functions >>>>>>>>#
// #####################################
// #<<<<<<<< Include Files >>>>>>>>>>>>#
// #####################################
#include <MMSystem.h>
#include <Windows.h>
#include <DigitalV.h>
//extern "C" {
//#include <WaveMix.h>
//};

// #####################################
// #<<<<<<<<<<<< Defines >>>>>>>>>>>>>>#
// #####################################
#define TYPE_MIDI 	"sequencer"
#define TYPE_CD		"cdaudio"
#define TYPE_WAVE		"waveaudio"
#define TYPE_AVI		"AVIVideo"

// #####################################
// #<<<<<<<< Include Files >>>>>>>>>>>>#
// #####################################
void ShowError(DWORD dwError);

// #####################################
// #<<<<< Midi Sequencer Class >>>>>>>>#
// #####################################
class MCISEQ
{
public:
   MCI_OPEN_PARMS		MciOpenParms;
   MCI_PLAY_PARMS		MciPlayParms;
   MCI_GENERIC_PARMS	MciGenParms;
	UINT 					wDeviceID;
	DWORD 				dwReturn;

   MCISEQ();
   ~MCISEQ();
   
	int OpenMIDI(HWND hWindow, LPSTR file);
	int PlayMIDI(HWND hWindow);
	int StopMIDI(HWND hWindow);
	int CloseMIDI(HWND hWindow);
	int PauseMIDI(HWND hWindow);
	int ResumeMIDI(HWND hWindow);
};

// #####################################
// #<<<<<<<<< Wave Mix Class >>>>>>>>>>#
// #####################################
/*class WAVEMIX
{
private:
	MIXCONFIG		WaveConfig;
	MIXPLAYPARAMS	PlayParams;
	WAVEMIXINFO		MixInfo;
	HANDLE			MixSession;

public:

	// Load / Un-Load WaveMix
	WAVEMIX();
   ~WAVEMIX();

   // Open / Play Wave File
   LPMIXWAVE 	OpenWaveFile(char *File, int Channel);
   int 			PlayWaveFile(LPMIXWAVE WaveFile, int Channel);

   // Free Wave File
   int	FreeWaveFile(LPMIXWAVE WaveFile);
};*/

