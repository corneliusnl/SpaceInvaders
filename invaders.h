#define 	ARRAYWIDTH	300
#define 	ANI_TIMER	6
#define 	MOV_TIMER	3

// ##################################
// #<<<<<<< Bonus Definitions >>>>>>#
// ##################################
#define BNS_FRAME1	0
#define BNS_FRAME2	1
#define BNS_AMMO1 	2
#define BNS_AMMO2		3

// ##################################
// #<<<<<< Sheild Definitions >>>>>>#
// ##################################
#define SHL_FRAME1
#define SHL_FRAME2
#define SHL_FRAME3
#define SHL_FRAME4

// ##################################
// #<<<<<<< Music Definitions >>>>>>#
// ##################################
#define	MUS_MAIN		0
#define 	MUS_ALIEN	1
#define	MUS_DEATH	2

// ##################################
// #<<<<<< Alien Definitions >>>>>>>#
// ##################################
#define 	ALI_DEATH1	0
#define	ALI_DEATH2	1
#define 	ALI_FRAME1	2
#define 	ALI_FRAME2	3
#define 	ALI_FRAME3	4

#define 	ALI_RIGHT	0
#define 	ALI_LEFT		1

// ##################################
// #<<<<<<< Key Definitions >>>>>>>>#
// ##################################
#define 	KEY_RIGHT	0
#define	KEY_LEFT		1

// ##################################
// #<<<<<<< Misc Definitions >>>>>>>#
// ##################################
#define	COCKPIT		0
#define 	JEREMIAH		1
#define 	LEPRELOGO	2

// ##################################
// #<<<<<<< Alien Definitions >>>>>>#
// ##################################
#define DEATH1		0
#define DEATH2		1
#define FRAME1		2
#define FRAME2		3
#define FRAME3		4

// ##################################
// #<< Screen Layout Definitions >>>#
// ##################################
#define STAT_GAME			0
#define STAT_VICTORY		1
#define STAT_PAUSED		2

#define PAU_DEMO			0
#define PAU_STORY			1
#define PAU_LEPRESOFT	2
#define PAU_CREATOR		3

// ##################################
// #<<<<<<< Timer Definitions >>>>>>#
// ##################################
#define SPACE_TIMER	0

// ##################################
// #<<<<<<< Struct Prototypes >>>>>>#
// ##################################
struct HeroStat
{
	int 	ShipX;
   int 	AmmoX;
   int 	AmmoY;
	int 	AmmoFrame;
   bool	Hit;

} HeroStat;

struct Explode
{
	int ExplodeX;
   int ExplodeY;
   int Frame;

} Explode;

struct Game
{
	int Status;
   int Divide;
   int Lives;
   int Score;
   int Level;

} Game;

struct Pause
{
	int	PrevMode;
   int	Status;
   bool	Paused;

} Pause;

struct Sheilds
{
	int 	HitCount;

} Sheilds[2];

struct Aliens
{
	bool	Successful;
   int 	CurFrame;
   int	Counter;
	int 	dir;
	// Hit Status
   int 	HitLocX;
   int	HitLocY;
   int	HitFrame;
   int 	HitAlien;
	// Ammo Status
   int 	AmmoX;
   int	AmmoY;
   int	AmmoFrm;
   // Array Status
	int	ArrayX;
   int	ArrayY;
	int	XOffset;
   int	YOffset;
	int 	WidthOffset;
   int	HeightOffset;
   int	NumAliens;
   int	Momentum;

} Aliens;

struct BonusAli
{
	int	XPos;
   int	AmmoX;
   int	AmmoY;
	int	AmmoFrm;
   int	Frame;
   int	Counter;
   bool	Destroyed;

} BonusAli;

struct Music
{
	int	Current;
	bool	Capiable;
   bool	Playing;

} Music;
