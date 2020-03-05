#ifndef SOUNDLAB_MAIN_H
#define SOUNDLAB_MAIN_H
#include "PainterEngine_Startup.h"

#define PX_SOUNDLAB_WINDOW_WIDTH 1024
#define PX_SOUNDLAB_DEFAULT_TIMEDOMAIN_SIZE 2048
#define PX_SOUNDLAB_SOUNDANALYSIS_INDEX 0
#define PX_SOUNDLAB_SOUNDCAPTURE_INDEX 1
#define PX_SOUNDLAB_SOUNDCAPTURE_BLOCKSIZE 2048
#define PX_SOUNDLAB_SOUNDCAPTURE_BLOCKCOUNT 16
#define PX_SOUNDLAB_SOUNDCAPTURE_BLOCKWAIT  4
#define SOUNDLAB_TRAIN_MFCC_SAMPLE_FACTOR_COUNT 24
#define SOUNDLAB_TRAIN_MFCC_ARRAY_COUNT 128
#define SOUBDLAB_TRAIN_TARGET_EPOCH 10000
typedef enum
{
	SOUNDLAB_MAIN_STATUS_STOP,
	SOUNDLAB_MAIN_STATUS_PLAY,
	SOUNDLAB_MAIN_STATUS_ANALYSISING,
	SOUNDLAB_MAIN_STATUS_TUNING,
	SOUNDLAB_MAIN_STATUS_RECORDING,
	SOUNDLAB_MAIN_STATUS_ANN,
}SOUNDLAB_MAIN_STATUS;


typedef enum
{
	SOUNDLAB_MAIN_DISPLAY_STFT,
	SOUNDLAB_MAIN_DISPLAY_SPECTRUM,
	SOUNDLAB_MAIN_DISPLAY_CEPSTRUM,
	SOUNDLAB_MAIN_DISPLAY_ADAPT,
	SOUNDLAB_MAIN_DISPLAY_RECORDING,
	SOUNDLAB_MAIN_DISPLAY_ANN,
}SOUNDLAB_MAIN_DISPLAY;

typedef struct
{
	px_double mfcc_factor[SOUNDLAB_TRAIN_MFCC_SAMPLE_FACTOR_COUNT];
}SOUNDLAB_TRAIN_MFCC_SAMPLE;


typedef struct  
{
	px_dword magic;//sinc
	px_double pitchshift;
	px_double filter[PX_SOUNDLAB_WINDOW_WIDTH];
}SOUNDLAB_FILTER_HEADER;

typedef struct  
{
	PX_Instance *Ins;
	px_int    wave_max;
	px_int    wave_min;
	px_dword  offset;
	px_double PowerThreshold;
	px_int    TimeDomainDurationCount;
	px_double rotAnimationAngle;
	PX_Tuning  DebugTuning;
	px_texture tex_play,tex_pause,tex_file,tex_reset,tex_record,tex_recording,tex_recordfinish,tex_ann,tex_sampling,tex_save;
	px_texture tex_Spectrum_renderTarget;
	px_texture tex_Spectrum;

	PX_Object *root;
	PX_Object *btn_OpenFile,*btn_Play,*btn_Record,*btn_Pause,*btn_reset,*btn_stft,*btn_spectrum,*btn_cepstrum,*btn_adapt,*btn_ann,*btn_save;
	PX_Object *btn_AdaptApply,*btn_FilterMode,*btn_ResetAdapt,*btn_RecordFinish;
	PX_Object *btn_AnnReset;
	PX_Object *SliderBar_PitchShift,*SliderBar_TimeScale;
	PX_Object *SpectrumCursor;
	PX_Object *Map_TimeDomain;
	PX_Object *Map_PreviewTimeDomain;
	PX_Object *Map_Spectrum;
	PX_Object *Map_Cepstrum;
	PX_Object *Map_Adapt;
	PX_Object *Map_Ann;
	PX_Object *FilterEditor;

	px_byte   *sourcePCM;
	px_int    sourcePCMSize;

	px_bool   bFilterMode;

	px_double SpectrumX[PX_SOUNDLAB_WINDOW_WIDTH];
	px_double SpectrumY[PX_SOUNDLAB_WINDOW_WIDTH];

	px_double PreviewSpectrumX[PX_SOUNDLAB_WINDOW_WIDTH];
	px_double PreviewSpectrumY[PX_SOUNDLAB_WINDOW_WIDTH];

	px_double CepstrumX[PX_SOUNDLAB_WINDOW_WIDTH];
	px_double CepstrumY[PX_SOUNDLAB_WINDOW_WIDTH];

	px_double SnapshotSampleData[PX_SOUNDLAB_WINDOW_WIDTH*8];


	px_double ann_time[100];
	px_int	 currentAnnOffset;
	volatile px_int   AnnEpoch;
	volatile px_int   AnnSucceeded;
	px_bool EnterTestMode;
	px_int TestIndex;
	px_double trainloss[100];
	px_double testloss[100];

	PX_ANN ann;

	volatile px_double schedule;
	volatile px_double process_memories;
	px_char lastError[64];
	volatile px_bool bDone;
	px_double MaxPower;
	PX_SoundData SoundData;
	PX_FontModule fm;
	PX_MessageBox messagebox;
	PX_MFCC mfcc;
	SOUNDLAB_TRAIN_MFCC_SAMPLE mfccsample[SOUNDLAB_TRAIN_MFCC_ARRAY_COUNT];
	px_int mfccsamplecount;
	volatile px_double currenttrainloss,currenttestloss;
	volatile px_bool bTrain,bTest;
	volatile px_bool bTrainRun;

	px_char recordCache[1024*1024*16];//32M for record
	px_int recorderWCursorByte;

	px_char MessageText[128];
	px_char FilePath[MAX_PATH];
	px_byte RecorderCache[PX_SOUNDLAB_SOUNDCAPTURE_BLOCKSIZE*PX_SOUNDLAB_SOUNDCAPTURE_BLOCKCOUNT*2];//16 blocks
	PX_SoundData recorderSoundData;
	px_double TimeDomainDataVertical[PX_SOUNDLAB_DEFAULT_TIMEDOMAIN_SIZE],TimeDomainDataHorizontal[PX_SOUNDLAB_DEFAULT_TIMEDOMAIN_SIZE];
	px_double PreviewTimeDomainDataVertical[PX_SOUNDLAB_WINDOW_WIDTH],PreviewTimeDomainDataHorizontal[PX_SOUNDLAB_WINDOW_WIDTH];
	SOUNDLAB_MAIN_STATUS status;
	SOUNDLAB_MAIN_DISPLAY displayMode;
}SoundLab_Main;


px_bool SoundLab_Main_Initialize(PX_Instance *Ins,SoundLab_Main *pMain);
px_void SoundLab_Main_Update(SoundLab_Main *pMain,px_dword elpased);
px_void SoundLab_Main_Render(px_surface *psurface,SoundLab_Main *pMain,px_dword elpased);
px_void SoundLab_Main_PostEvent(SoundLab_Main *pMain,PX_Object_Event e);

typedef struct
{
	px_int xOffset,yOffset;
	px_bool bshow;
	SoundLab_Main *pMain;
}SoundLab_SpectrumCursor;
PX_Object * SoundLab_SpectrumCursorCreate(px_memorypool *mp,PX_Object *Parent,SoundLab_Main *pMain);
#endif