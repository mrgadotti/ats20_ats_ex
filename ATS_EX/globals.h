#pragma once

uint32_t g_storeTime = 0;

bool g_voltagePinConnnected = false;
bool g_ssbLoaded = false;
bool g_fmStereo = true;

bool g_cmdVolume = false;
bool g_cmdStep = false;
bool g_cmdBw = false;
bool g_cmdBand = false;
bool g_settingsActive = false;
bool g_sMeterOn = false;
bool g_displayOn = true;
bool g_displayRDS = false;
bool g_rdsSwitchPressed = false;
bool g_seekStop = false;
uint32_t g_lastAdjustmentTime = 0;

uint8_t g_muteVolume = 0;
int g_currentBFO = 0;

// Encoder buttons
SimpleButton  btn_Bandwidth(BANDWIDTH_BUTTON);
SimpleButton  btn_BandUp(BAND_BUTTON);
SimpleButton  btn_BandDn(SOFTMUTE_BUTTON);
SimpleButton  btn_VolumeUp(VOLUME_BUTTON);
SimpleButton  btn_VolumeDn(AVC_BUTTON);
SimpleButton  btn_Encoder(ENCODER_BUTTON);
SimpleButton  btn_AGC(AGC_BUTTON);
SimpleButton  btn_Step(STEP_BUTTON);
SimpleButton  btn_Mode(MODE_SWITCH);

volatile int g_encoderCount = 0;

//Frequency tracking
uint16_t g_currentFrequency;
uint16_t g_previousFrequency;

enum SettingType
{
    ZeroAuto,
    Num,
    Switch,
    SwitchAuto
};

//Constant part of a setting (kept in flash). Its value lives in g_settingParam
struct SettingsItem
{
    char name[4];
    uint8_t type;
    void (*manipulateCallback)(int8_t);
};

void doAttenuation(int8_t v);
void doSoftMute(int8_t v);
void doBrightness(int8_t v);
void doSSBAVC(int8_t v = 0);
void doAvc(int8_t v);
void doSync(int8_t v = 0);
void doDeEmp(int8_t v = 0);
void doSWUnits(int8_t v = 0);
void doSSBSoftMuteMode(int8_t v = 0);
void doCutoffFilter(int8_t v);
void doCPUSpeed(int8_t v = 0);
#if USE_RDS
void doRDSErrorLevel(int8_t v);
#endif
void doBFOCalibration(int8_t v);
void doUnitsSwitch(int8_t v = 0);
void doScanSwitch(int8_t v = 0);
void doCWSwitch(int8_t v = 0);

const SettingsItem g_Settings[] PROGMEM =
{
    //Page 1
    { "ATT", SettingType::ZeroAuto,     doAttenuation     },  //Attenuation
    { "SM ", SettingType::Num,          doSoftMute        },  //Soft Mute
    { "SVC", SettingType::Switch,       doSSBAVC          },  //SSB AVC Switch
    { "Syn", SettingType::Switch,       doSync            },  //SSB Sync
    { "DeE", SettingType::Switch,       doDeEmp           },  //FM DeEmphasis (0 - 50, 1 - 75)
    { "AVC", SettingType::Num,          doAvc             },  //Automatic Volume Control
    //Page 2
    { "Scr", SettingType::Num,          doBrightness      },  //Screen Brightness
    { "SW ", SettingType::Switch,       doSWUnits         },  //SW Units
    { "SSM", SettingType::Switch,       doSSBSoftMuteMode },  //SSB Soft Mute Mode
    { "COF", SettingType::SwitchAuto,   doCutoffFilter    },  //SSB Cutoff Filter
    { "CPU", SettingType::Switch,       doCPUSpeed        },  //CPU Frequency
#if USE_RDS
    { "RDS", SettingType::Num,          doRDSErrorLevel   },  //RDS ErrorLevel
#endif
    //Page 3
    { "BFO", SettingType::Num,          doBFOCalibration  },  //BFO Offset calibration
    { "Uni", SettingType::Switch,       doUnitsSwitch     },  //Show/Hide frequency units
    { "Sca", SettingType::Switch,       doScanSwitch      },  //AM Encoder scan switch
    { "CW ", SettingType::Switch,       doCWSwitch        },  //CW is LSB or USB
};

enum SettingsIndex
{
    ATT,
    SoftMute,
    SVC,
    Sync,
    DeEmp,
    AutoVolControl,
    Brightness,
    SWUnits,
    SSM,
    CutoffFilter,
    CPUSpeed,
#if USE_RDS
    RDSError,
#endif
    BFO,
    UnitsSwitch,
    ScanSwitch,
    CWSwitch,
    SETTINGS_MAX
};

//Setting values (default values here, same order as g_Settings)
int8_t g_settingParam[SettingsIndex::SETTINGS_MAX] =
{
    0,  //ATT
    0,  //SoftMute
    1,  //SVC
    0,  //Sync
    1,  //DeEmp
    46, //AutoVolControl
    80, //Brightness
    0,  //SWUnits
    1,  //SSM
    0,  //CutoffFilter
    0,  //CPUSpeed
#if USE_RDS
    1,  //RDSError
#endif
    0,  //BFO
    1,  //UnitsSwitch
    1,  //ScanSwitch
    0,  //CWSwitch
};

const uint8_t g_SettingsMaxPages = 3;
int8_t g_SettingSelected = 0;
int8_t g_SettingsPage = 1;
bool g_SettingEditing = false;

//For managing BW
struct Bandwidth
{
    uint8_t idx;      //Internal SI473X index
    char desc[5];
};

inline uint8_t getBwIdx(const Bandwidth* table, uint8_t i)
{
    return pgm_read_byte(&table[i].idx);
}

int8_t g_bwIndexSSB = 4;
const Bandwidth g_bandwidthSSB[] PROGMEM =
{
    { 4, "0.5k" },
    { 5, "1.0k" },
    { 0, "1.2k" },
    { 1, "2.2k" },
    { 2, "3.0k" },
    { 3, "4.0k" }
};
const uint8_t g_bwSSBMaxIdx = 5;

int8_t g_bwIndexAM = 4;
const uint8_t g_maxFilterAM = 6;
const Bandwidth g_bandwidthAM[] PROGMEM =
{
    { 4, "1.0k" }, // 0
    { 5, "1.8k" }, // 1
    { 3, "2.0k" }, // 2
    { 6, "2.5k" }, // 3
    { 2, "3.0k" }, // 4 - Default
    { 1, "4.0k" }, // 5
    { 0, "6.0k" }  // 6
};

int8_t g_bwIndexFM = 0;
const char g_bandwidthFM[][5] PROGMEM =
{
    "AUTO",
    "110k",
    " 84k",
    " 60k",
    " 40k"
};

int g_tabStep[] =
{
    // AM steps in KHz
    1,
    5,
    9,
    10,
    // Large AM steps in KHz
    50,
    100,
    1000,
    // SSB steps in Hz
    10,
    25,
    50,
    100,
    500
};
uint8_t g_amTotalSteps = 7;
uint8_t g_amTotalStepsSSB = 4; //Prevent large AM steps appear in SSB mode
uint8_t g_ssbTotalSteps = 5;
volatile int8_t g_stepIndex = 3;

int8_t g_tabStepFM[] =
{
    5,  // 50 KHz
    10, // 100 KHz
    100 // 1 MHz
};
int8_t g_FMStepIndex = 1;
const int8_t g_lastStepFM = (sizeof(g_tabStepFM) / sizeof(int8_t)) - 1;

//Band table structures
enum BandType : uint8_t
{
    LW_BAND_TYPE,
    MW_BAND_TYPE,
    SW_BAND_TYPE,
    FM_BAND_TYPE
};

struct Band
{
    uint16_t minimumFreq;
    uint16_t maximumFreq;
    uint16_t currentFreq;
    int8_t currentStepIdx;
    int8_t bandwidthIdx;     // Bandwidth table index (internal table in Si473x controller)
};

#if USE_RDS
enum RDSActiveInfo : uint8_t
{
    StationName,
    StationInfo,
    ProgramInfo
};
uint8_t g_rdsActiveInfo = RDSActiveInfo::StationName;
char g_rdsPrevLen = 0;
char* g_RDSCells[3];
#endif

const char g_emptyLine[] PROGMEM = "                ";

const char bandTags[][3] PROGMEM =
{
    "LW",
    "MW",
    "SW",
    "  ",    //It looks better
};

Band g_bandList[] =
{
    /* LW */ { LW_LIMIT_LOW, 520, 300, 0, 4 },
    /* MW */ { 520, 1710, 1476, 3, 4 },
    /* SW */ { SW_LIMIT_LOW, SW_LIMIT_HIGH, SW_LIMIT_LOW, 0, 4 },
    /* FM */ { 6400, 10800, 8400, 1, 0 },
};

//SW sub-bands: broadcast and amateur (up to 10m, IARU region 2), sorted by frequency.
//Ranges are in KHz and half-open: [low, high). Edit here to adjust them.
struct SWSubBand
{
    uint16_t low;
    uint16_t high;
};

#define HAM 0x8000 //Flag in "low": amateur band, SSB is selected automatically there

const SWSubBand g_swSubBands[] PROGMEM =
{
    {  HAM | 1800,  2000 }, //  0: 160m HAM
    {  2300,  2495 }, //  1: 120m
    {  3200,  3400 }, //  2:  90m
    {  HAM | 3500,  3900 }, //  3:  80m HAM
    {  3900,  4000 }, //  4:  75m
    {  4750,  5060 }, //  5:  60m
    {  5900,  6200 }, //  6:  49m
    {  HAM | 7000,  7300 }, //  7:  40m HAM
    {  7300,  7450 }, //  8:  41m
    {  9400,  9900 }, //  9:  31m
    { HAM | 10100, 10150 }, // 10:  30m HAM
    { 11600, 12100 }, // 11:  25m
    { 13570, 13870 }, // 12:  22m
    { HAM | 14000, 14350 }, // 13:  20m HAM
    { 15100, 15800 }, // 14:  19m
    { 17480, 17900 }, // 15:  16m
    { HAM | 18068, 18168 }, // 16:  17m HAM
    { 18900, 19020 }, // 17:  15m
    { HAM | 21000, 21450 }, // 18:  15m HAM
    { 21450, 21850 }, // 19:  13m
    { HAM | 24890, 24990 }, // 20:  12m HAM
    { 25670, 26100 }, // 21:  11m
    { HAM | 28000, 29700 }, // 22:  10m HAM
};
const uint8_t g_SWSubBandCount = sizeof(g_swSubBands) / sizeof(SWSubBand);

#define SSB_LSB_LIMIT 10000 //Amateur bands below this frequency use LSB, above - USB

//Last frequency used in each sub-band (0 - not set)
uint16_t g_swLastFreq[g_SWSubBandCount];
const uint8_t g_lastBand = (sizeof(g_bandList) / sizeof(Band)) - 1;
int8_t g_bandIndex = 1;

// Modulation
enum Modulations : uint8_t
{
    AM,
    LSB,
    USB,
    CW,
    FM
};
volatile uint8_t g_currentMode = FM;
const char g_bandModeDesc[][4] PROGMEM =
{ 
    "AM ",
    "LSB",
    "USB",
    "CW ",
    "FM "
};
volatile uint8_t g_prevMode = FM;
uint8_t g_seekDirection = 1;

//Special logic for fast and responsive frequency surfing
uint32_t g_lastFreqChange = 0;
bool g_processFreqChange = 0;
uint8_t g_volume = DEFAULT_VOLUME;

Rotary g_encoder = Rotary(ENCODER_PIN_A, ENCODER_PIN_B);
SI4735 g_si4735;