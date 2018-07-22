// ##################################
// #<<<<<<< Include Files >>>>>>>>>>#
// ##################################
#include <Windows.h>
#include "WinApp.h"
#include "MultiMedia.h"
#include "Invaders.h"
#include "Colors.h"
#include "Invaders.rh"
#include "MemDC.h"
#include "Types.h"
#include "LoadPoly.h"
#include "World.h"
#include "Matrix.h"

// ##################################
// #<<<<<<<< Window Stuff >>>>>>>>>>#
// ##################################
WINAPP			WinApp;
HMENU          hMenuInit;
HDC 				ScreenDC;
MCISEQ			*MidiDrv;

// ##################################
// #<<<<<<<<<<< Procedures >>>>>>>>>#
// ##################################
int _export FAR PASCAL AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
int _export FAR PASCAL SettingsDlgProc(HWND, UINT, WPARAM, LPARAM);
int _export FAR PASCAL ScoresDlgProc(HWND, UINT, WPARAM, LPARAM);
int _export FAR PASCAL NewHighDlgProc(HWND, UINT, WPARAM, LPARAM);

// ##################################
// #<<<<<<<<< Sprite Stuff >>>>>>>>>#
// ##################################
HPALETTE			hPalette;
MEM_DC			*SprDrv;

DIB			BackGround;
DIB			HeroShip;
DIB			HeroAmmo[2];
LPDIB			Misc;
LPDIB			GreenA;
LPDIB			YellowA;
LPDIB			PurpleA;
LPDIB			Sheild;
LPDIB			Explosion;
LPDIB			AlienAmmo;
LPDIB			Bonus;

// ##################################
// #<<<<<<<< 3-D Variables >>>>>>>>>#
// ##################################
WORLD		*World;
OBJECT	*ShipModel;
MATRIX	*Mat;
long 		Trans = 1500;
float 	XAngle = 3.14, YAngle = 0, ZAngle = 0/*.3*/;

// ##################################
// #<<<<<<<<<< Functions >>>>>>>>>>>#
// ##################################
void 	LoadBitmaps();
void 	FreeBitmaps();
int	AppIdle(HWND hWnd);

void 	DisplayStory();
void	DisplayInfo();
void 	DisplayCreator();
void	StartGame(HWND hWnd, int Level);
void	DrawAliens(DIB *AlienDibs, int Row);
void 	ArrayStatus();
void	DecoyStatus();
void	ExplodeAlien();
void	ExplodeObject();
void	AliensAmmo(HWND hWnd);
void	MoveAliens(HWND hWnd);
void	HandleHeroAmmo();
void 	AnimateAliens();
void	DisplayBonus();
void	BonusAmmo(HWND hWnd);
void	ReadInitFile(HWND hWnd);
void	LoadScores();
void	AddHigh(char *Name, int Score);

// ##################################
// #<<<<<< Program Variables >>>>>>>#
// ##################################
int	NewTime, OldTime;
int	KeyBuff[2];
int	Invaders[6][3];
int	JoyCenter;
char	Names[10][50];
int	Scores[10];
char	*JukeBox[] =
{
	"MUSIC\\FifthSym.Mid",
   "MUSIC\\AlienRay.Mid",
   "MUSIC\\Ending.Mid",
};

// ##################################
// #<<<<<<< Window Main Proc >>>>>>>#
// ##################################
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR cmdString, int cmdShow)
{
	HWND        MainWnd;
	MSG         Msg;
	WNDCLASS    WndClass;

	WinApp.hInstance = hInstance;
	WinApp.hPrevInst = hPrevInst;
	WinApp.cmdShow   = cmdShow;

	if(!WinApp.hPrevInst)
	{
		WndClass.style          = CS_HREDRAW | CS_VREDRAW;
		WndClass.lpfnWndProc    = WndProc;
		WndClass.cbClsExtra     = 0;
		WndClass.cbWndExtra     = 0;
		WndClass.hInstance      = hInstance;
		WndClass.hIcon          = LoadIcon(WinApp.hInstance, MAKEINTRESOURCE(IDI_ICON1));
		WndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
		WndClass.hbrBackground  = (HBRUSH)(GetStockObject(LTGRAY_BRUSH));
		WndClass.lpszMenuName   = NULL;
		WndClass.lpszClassName  = "MAINWND";

		RegisterClass(&WndClass);
	}

	hMenuInit = LoadMenu(WinApp.hInstance, MAKEINTRESOURCE(IDM_MENU1));
	MainWnd   = CreateWindow("MAINWND",
   				"Space Invaders: The Year We Make Contact v2.0",
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, CW_USEDEFAULT,
					508, 547, NULL, hMenuInit, hInstance, NULL);

	ShowWindow(MainWnd, SW_NORMAL);
	UpdateWindow(MainWnd);

	for(;;)
	{
		if(PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
		{
			if(Msg.message == WM_QUIT)
				break;
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else
			if(AppIdle(MainWnd))
         	WaitMessage();						// Speed Bump
	}
	return Msg.wParam;
};

// ##################################
// #<<<<<<< Window Procedure >>>>>>>#
// ##################################
LRESULT CALLBACK _export WndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT	WinRect;
   int 	JoyX;

	switch(message)
	{
		case WM_CREATE:
         ScreenDC 	= GetDC(hWindow);
			SprDrv 		= new MEM_DC(500, 500);
			MidiDrv		= new MCISEQ();
			Mat 			= new MATRIX();
			World			= new WORLD();
  			ShipModel	= World->LoadObject("Ship.3-d");

			ReadInitFile(hWindow);
			LoadScores();
         LoadBitmaps();

         Game.Status = STAT_PAUSED;
			SprDrv->DibBlt(BackGround, 0, 0);
         NewTime = OldTime = timeGetTime();
			return 0;

		case WM_PAINT:
			BeginPaint(hWindow, &ps);
			GetClientRect(hWindow, &WinRect);

			// Select Palette
			SprDrv->BitmapPalette(ScreenDC, BackGround);

			// Blt Sprite DC to Screen
			SprDrv->StretchToScreen(ScreenDC, 0, 0,
         			WinRect.right, WinRect.bottom,
         			0, 0, SprDrv->GetWidth(), SprDrv->GetHeight());

//			SprDrv->PrepareFade(ScreenDC, 0, 0);
//         SprDrv->FadeIn(ScreenDC, 500);
// 			SprDrv->EndFade(ScreenDC, 0, 0);

			EndPaint(hWindow, &ps);
			return 0;

		case WM_ACTIVATE:
	      if(LOWORD(wParam) == WA_ACTIVE)
	      	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, NULL, SPIF_UPDATEINIFILE);
			else
				SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, NULL, SPIF_UPDATEINIFILE);
         return 0;

		case MM_MCINOTIFY:
			switch(wParam)
			{
				case MCI_NOTIFY_SUCCESSFUL:
	         	MidiDrv->CloseMIDI(hWindow);
	         	MidiDrv->OpenMIDI(hWindow, JukeBox[Music.Current]);
					MidiDrv->PlayMIDI(hWindow);
					break;
	      }
			return 0;

      case MM_JOY1MOVE:
	   case MM_JOY2MOVE:
			JoyX = LOWORD(lParam);
			if(JoyX > JoyCenter + 1500)
         {
				KeyBuff[KEY_RIGHT] 	= 1;
				KeyBuff[KEY_LEFT] 	= 0;
         }
         else if(JoyX < JoyCenter - 1500)
         {
				KeyBuff[KEY_LEFT] 	= 1;
				KeyBuff[KEY_RIGHT] 	= 0;
         }
			return 0;

      case MM_JOY1BUTTONDOWN:
      case MM_JOY2BUTTONDOWN:
        	if(HeroStat.AmmoY == 999)
			{
				HeroStat.AmmoX 		= HeroStat.ShipX + 22;
				HeroStat.AmmoY 		= 350;
				HeroStat.AmmoFrame 	= 1;
				sndPlaySound("Photon.Wav", SND_ASYNC);
			}
         return 0;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_RIGHT:
					KeyBuff[KEY_RIGHT] 	= 1;
					KeyBuff[KEY_LEFT] 	= 0;
					break;

				case VK_UP:
            	Trans += 100;
               return 0;

            case VK_DOWN:
            	Trans -= 100;
               return 0;

				case VK_LEFT:
					KeyBuff[KEY_LEFT] 	= 1;
					KeyBuff[KEY_RIGHT] 	= 0;
					break;

				case VK_SPACE:
            case VK_CONTROL:
            	if(HeroStat.AmmoY == 999)
               {
						HeroStat.AmmoX 		= HeroStat.ShipX + 22;
	               HeroStat.AmmoY 		= 350;
						HeroStat.AmmoFrame 	= 1;
						sndPlaySound("Photon.Wav", SND_ASYNC);
               }
               break;
			}
         return 0;

      case WM_KEYUP:
      	switch(wParam)
         {
         	case VK_RIGHT:
//            	KeyBuff[KEY_RIGHT] 	= 0;
               break;

            case VK_LEFT:
//            	KeyBuff[KEY_LEFT] 	= 0;
               break;

            case VK_SPACE:
            case VK_CONTROL:
               break;
         }
         return 0;

		case WM_COMMAND:
			switch(wParam)
			{
         	case CM_NEW:
				   Game.Score			= 0;
				   Game.Lives			= 3;
					Game.Divide 		= 1;
            	StartGame(hWindow, 1);
               break;

/*            case CM_PAUSE:
            	Pause.Status	 	= PAU_DEMO;
					Pause.Paused		= TRUE;
               break;*/

            case CM_ABOUT:
            	DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWindow, (DLGPROC)AboutDlgProc);
					break;

            case CM_HIGHSCORES:
            	DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_SCORES), hWindow, (DLGPROC)ScoresDlgProc);
					break;

            case CM_SETTINGS:
            	DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_SETTINGS), hWindow, (DLGPROC)SettingsDlgProc);
					break;

				case CM_EXIT:
					PostQuitMessage(0);
					break;

				case CM_STORY:
					if(Pause.Status != PAU_STORY)
               {
//           		SprDrv->PrepareFade(ScreenDC, 0, 0);
//						SprDrv->FadeOut(ScreenDC, 500);
//						SprDrv->EndFade(ScreenDC, 0, 0);

                  Pause.PrevMode	= Game.Status;
                  Pause.Status	= PAU_STORY;
						Pause.Paused 	= TRUE;
					}
               else if(Pause.Status == PAU_STORY)
               {
                  Pause.Paused	= FALSE;
               }
               break;

				case CM_LEPRESOFT:
					if(Pause.Status != PAU_LEPRESOFT)
               {
//						SprDrv->PrepareFade(ScreenDC, 0, 0);
//						SprDrv->FadeOut(ScreenDC, 500);
//						SprDrv->EndFade(ScreenDC, 0, 0);

                  Pause.PrevMode	= Game.Status;
                  Pause.Status	= PAU_LEPRESOFT;
						Pause.Paused	= TRUE;;
               }
					else if(Pause.Status == PAU_LEPRESOFT)
					{
                  Pause.Paused	= FALSE;
               }
               break;

				case CM_CREATOR:
					if(Pause.Status != PAU_CREATOR)
               {
//						SprDrv->PrepareFade(ScreenDC, 0, 0);
//						SprDrv->FadeOut(ScreenDC, 500);
//						SprDrv->EndFade(ScreenDC, 0, 0);

                  Pause.PrevMode	= Game.Status;
                  Pause.Status	= PAU_CREATOR;
						Pause.Paused	= TRUE;  
					}
               else if(Pause.Status == PAU_CREATOR)
               {
						Pause.Paused	= FALSE;
               }
               break;
			}
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			FreeBitmaps();
			delete MidiDrv;
         delete SprDrv;
			delete Mat;
         delete World;
			return 0;

		default:
			return(DefWindowProc(hWindow, message, wParam, lParam));
	}
};

// ##################################
// #<<<<<<<<< Load Bitmaps >>>>>>>>>#
// ##################################
void LoadBitmaps()
{
	// Load Bitmap Elements
   Misc			= SprDrv->LoadSprites("Misc.jcm");
	GreenA		= SprDrv->LoadSprites("Green.jcm");
   YellowA		= SprDrv->LoadSprites("Yellow.jcm");
   PurpleA		= SprDrv->LoadSprites("Purple.jcm");
   Sheild		= SprDrv->LoadSprites("Sheild.jcm");
   Explosion	= SprDrv->LoadSprites("Explode.jcm");
	AlienAmmo	= SprDrv->LoadSprites("AlienAmmo.jcm");
	Bonus			= SprDrv->LoadSprites("Bonus.jcm");
	BackGround 	= SprDrv->LoadSprite("Scenes.jcm",	"Grass Land");
   HeroShip		= SprDrv->LoadSprite("Heros.jcm", 	"Hero Ship");
  	HeroAmmo[0]	= SprDrv->LoadSprite("Heros.jcm", 	"Photon 1");
   HeroAmmo[1]	= SprDrv->LoadSprite("Heros.jcm", 	"Photon 2");

	// Select Palcette Into Screen DC
	SprDrv->BitmapPalette(ScreenDC, BackGround);
};

// ##################################
// #<<<<<<<<< Free Bitmaps >>>>>>>>>#
// ##################################
void FreeBitmaps()
{
	SprDrv->FreeDibs(Misc, 0, 2);
   SprDrv->FreeDibs(Explosion, 0, 7);
   SprDrv->FreeDibs(Sheild, 0, 3);
	SprDrv->FreeDibs(PurpleA, 0, 5);
	SprDrv->FreeDibs(GreenA, 0, 5);
   SprDrv->FreeDibs(YellowA, 0, 5);
	SprDrv->FreeDibs(HeroAmmo, 0, 1);
   SprDrv->FreeDibs(AlienAmmo, 0, 3);
   SprDrv->FreeDibs(Bonus, 0, 3);
	SprDrv->FreeDib(BackGround);
   SprDrv->FreeDib(HeroShip);
};

// ##################################
// #<<<<<<<<< Update Screen >>>>>>>>#
// ##################################
int AppIdle(HWND hWnd)
{
	if((NewTime = timeGetTime()) - OldTime > 4)
   {
		RECT	WinRect;
		GetClientRect(hWnd, &WinRect);

      // ############################
		// #<<<< Display Game Mode >>>#
      // ############################
		if((Game.Status == STAT_GAME) && (Pause.Paused == FALSE))
      {
			// Blt Background Bitmap
			SprDrv->DibBlt(BackGround, 0, 0);

	      // ############################
			// #<<<< Keyboard Control >>>>#
	      // ############################
         // Increment Ship X
         if((HeroStat.ShipX <= 450) && (KeyBuff[KEY_RIGHT]))
         	HeroStat.ShipX += 4;

			// Decrement Ship X
			if((HeroStat.ShipX >= 0) && (KeyBuff[KEY_LEFT]))
         	HeroStat.ShipX -= 4;

	      // ############################
			// #<<<<< Game Progress >>>>>>#
	      // ############################
         if(Aliens.NumAliens == 0)
         {
				Game.Level++;
         	StartGame(hWnd, Game.Level);
         }
			if((Game.Score / Game.Divide) >= 3000)
         {
				sndPlaySound("Free.Wav", SND_ASYNC);
				Game.Divide++;
            Game.Lives++;
            Game.Score += 100;
         }

	      // ############################
			// #<<<<<< Update Hero >>>>>>>#
	      // ############################
         HandleHeroAmmo();
         if(Game.Lives < 0)
         {
				Game.Status = STAT_PAUSED;

				// Play The Pipes :-)
		      if(Music.Capiable == TRUE)
		      {
		      	if(Music.Playing == TRUE)
		         {
						MidiDrv->StopMIDI(hWnd);
   		         MidiDrv->CloseMIDI(hWnd);
		         }

					Music.Current = MUS_DEATH;
					MidiDrv->OpenMIDI(hWnd, JukeBox[Music.Current]);
					MidiDrv->PlayMIDI(hWnd);
			      Music.Playing = TRUE;
            }

   			// New High Score??
				if(Game.Score >= Scores[9])
	           	DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_NEWHIGH), hWnd, (DLGPROC)NewHighDlgProc);
         }

	      // ############################
			// #<<< Update Bonus Alien >>>#
	      // ############################
         DisplayBonus();
			BonusAmmo(hWnd);
         
	      // ############################
			// #<<<< Update Aliens >>>>#
	      // ############################
			AnimateAliens();
         MoveAliens(hWnd);
			ExplodeAlien();
         DrawAliens(PurpleA, 0);
         DrawAliens(GreenA, 1);
         DrawAliens(YellowA, 2);
         ArrayStatus();
         AliensAmmo(hWnd);

	      // ############################
			// #<<<<<<< Game Screen >>>>>>#
	      // ############################
			SprDrv->TransBlt(Misc[COCKPIT], 0, 420, 0);
			if(HeroStat.Hit == FALSE)
				SprDrv->TransBlt(HeroShip, HeroStat.ShipX, 350, 0);

	      // ############################
			// #<<<<<<<<< Sheilds >>>>>>>>#
	      // ############################
         DecoyStatus();
      	if(Sheilds[0].HitCount < 4)
	  			SprDrv->TransBlt(Sheild[Sheilds[0].HitCount], 50, 300, 0);
   		if(Sheilds[1].HitCount < 4)
				SprDrv->TransBlt(Sheild[Sheilds[1].HitCount], 400, 300, 0);
         ExplodeObject();

	      // ############################
			// #<< Cockpit Information >>>#
	      // ############################
			SprDrv->SetFont(15, 21, Blue);
        	SprDrv->DisplayText(Game.Score, 	250, 480);
			SprDrv->DisplayText(Game.Level, 	450, 480);
			SprDrv->DisplayText(Game.Lives,  155, 477);
        	SprDrv->DeleteFont();

		  	// Stretch to Screen
			SprDrv->StretchToScreen(ScreenDC, 0, 0,
				WinRect.right, WinRect.bottom,
				0, 0, SprDrv->GetWidth(), SprDrv->GetHeight());
		}

      // ############################
		// #<<< Rotating Hero Ship >>>#
      // ############################
		if((Game.Status == STAT_PAUSED) || (Pause.Paused == TRUE))
      {
			// Display BackGround
			SprDrv->DibBlt(BackGround, 0, 0);

			YAngle += 0.03;

  			Mat->Identity(Mat->MainMatrix.ElemPtr);
			Mat->Scale(1, 1, 1);
			Mat->Rotate(XAngle, YAngle, ZAngle);
			Mat->Translate(0, 0, Trans);
			Mat->Transform(ShipModel);
			World->Project(SprDrv, ShipModel, 100);
			World->DrawObject(SprDrv, ShipModel);

			// Display Information
         if(Pause.Paused == TRUE)
         {
	   		switch(Pause.Status)
	         {
	         	case PAU_STORY:
			      	DisplayStory();
						break;
					case PAU_LEPRESOFT:
				     	DisplayInfo();
	               break;
					case PAU_CREATOR:
			      	DisplayCreator();
	               break;
				}
         }

		  	// Stretch to Screen
			SprDrv->StretchToScreen(ScreenDC, 0, 0,
				WinRect.right, WinRect.bottom,
				0, 0, SprDrv->GetWidth(), SprDrv->GetHeight());
      }

      // ############################
		// #<<<< Alien Celebration >>>#
      // ############################
      if(Game.Status == STAT_VICTORY)
      {
			// Blt Background Bitmap
			SprDrv->DibBlt(BackGround, 0, 0);

			// Continue Moving Aliens
			AnimateAliens();
      	MoveAliens(hWnd);
         DrawAliens(PurpleA, 0);
         DrawAliens(GreenA, 1);
         DrawAliens(YellowA, 2);
         
		  	// Stretch to Screen
			SprDrv->StretchToScreen(ScreenDC, 0, 0,
				WinRect.right, WinRect.bottom,
				0, 0, SprDrv->GetWidth(), SprDrv->GetHeight());
      }
		OldTime = NewTime;
   }
	return 0;
};

// ##################################
// #<<<<<<<<< Start New Game >>>>>>>#
// ##################################
void StartGame(HWND hWnd, int Level)
{
	int NumBackPic;

	// Reset Aliens
	memset(&Invaders, 1, sizeof(int) * 18);

	// Hero Status
	HeroStat.ShipX 		= 350;
   HeroStat.AmmoX 		= 250;
   HeroStat.AmmoY 		= 999;
   HeroStat.Hit			= FALSE;

	// Game Status
	Game.Status			= STAT_GAME;
	Game.Level 			= Level;

   // Bonus Status
   BonusAli.XPos			= -50;
	BonusAli.AmmoY			= 999;
	BonusAli.AmmoFrm		= BNS_AMMO1;
   BonusAli.Frame			= BNS_FRAME1;
   BonusAli.Counter 		= 0;
   BonusAli.Destroyed	= FALSE;

	// Alien Status
   Aliens.Successful = TRUE;
   Aliens.ArrayX		= 0;
   Aliens.ArrayY		= 0;
   Aliens.Counter		= 0;
   Aliens.CurFrame	= ALI_FRAME1;
   Aliens.dir			= ALI_RIGHT;
	Aliens.HitAlien	= 999;
	Aliens.AmmoX		= 999;
   Aliens.AmmoY 		= 999;
	Aliens.XOffset			= 0;
   Aliens.YOffset 		= 0;
   Aliens.WidthOffset 	= 0;
   Aliens.HeightOffset 	= 150;
   Aliens.NumAliens		= 18;
   Aliens.AmmoFrm			= 0;
   Aliens.Momentum 		= 10;

   // Explosion Status
   Explode.Frame 	= 999;

   // Sheild Status
   Sheilds[0].HitCount	= 0;
   Sheilds[1].HitCount	= 0;

	// Music Status
	Music.Current		= MUS_MAIN;
   Music.Capiable 	= FALSE;

	// Load Back Drop
	NumBackPic = Game.Level % 5;
   switch(NumBackPic)
   {
   	case 0:
     		BackGround 	= SprDrv->LoadSprite("Scenes.jcm",	"Grass Land");
         break;
      case 1:
     		BackGround 	= SprDrv->LoadSprite("Scenes.jcm",	"Desert Sands");
         break;
      case 2:
      	BackGround 	= SprDrv->LoadSprite("Scenes.jcm",	"Water World");
         break;
      case 3:
      	BackGround 	= SprDrv->LoadSprite("Scenes.jcm",	"Midnight Moon");
         break;
      case 4:
      	BackGround 	= SprDrv->LoadSprite("Scenes.jcm",	"Earth Watch");
         break;
	}
   // Is Computer Capiable of Playing Midi's?
	if(midiOutGetNumDevs() > 0)
   {
		Music.Capiable = TRUE;
		if(Music.Playing == TRUE)
      {
			MidiDrv->StopMIDI(hWnd);
		   MidiDrv->CloseMIDI(hWnd);
      }
		MidiDrv->OpenMIDI(hWnd, JukeBox[Music.Current]);
		MidiDrv->PlayMIDI(hWnd);
      Music.Playing = TRUE;
   }
};

// ##################################
// #<<<<<<<<<< Draw Aliens >>>>>>>>>#
// ##################################
void DrawAliens(DIB *AlienDib, int Row)
{
	for(int i = 0; i < 6; i++)
   {
		// Detect if Alien Hit with Ammo
   	if(( HeroStat.AmmoX 		>= Aliens.ArrayX + (i * 50)		) &&
      	( HeroStat.AmmoX + 5 <= Aliens.ArrayX + ((i+1) * 50)	) &&
			( HeroStat.AmmoY 		>= Aliens.ArrayY + (Row * 50)		) &&
			( HeroStat.AmmoY + 5 <= Aliens.ArrayY + ((Row+1) * 50)) &&
         ( Invaders[i][Row] != 0 ))
      {
      	sndPlaySound("Pop.Wav", SND_ASYNC);
			Invaders[i][Row] 		= 0;
			HeroStat.AmmoY 		= 999;
   		Aliens.HitLocX 		= Aliens.ArrayX + (i * 50);
         Aliens.HitLocY 		= Aliens.ArrayY + (Row * 50);
         Aliens.HitFrame 		= ALI_DEATH1;
         Aliens.HitAlien 		= Row;
         Aliens.Momentum		+= 2;
			Aliens.NumAliens--;

         if(Row == 0)
         	Game.Score += 30;
         if(Row == 1)
         	Game.Score += 20;
         if(Row == 2)
         	Game.Score += 10;
      }

		// Draw Alien if not killed
      if(Invaders[i][Row] != 0)
			SprDrv->TransBlt(AlienDib[Aliens.CurFrame], Aliens.ArrayX + i * 50, Aliens.ArrayY + (Row * 50), 0);
   }
};

// ##################################
// #<<<<<<<<< Explode Alien >>>>>>>>#
// ##################################
void ExplodeAlien()
{
	// Purple Alien Hit
	if((Aliens.HitAlien == 0) && (Aliens.HitFrame < 2))
   {
		SprDrv->TransBlt(PurpleA[Aliens.HitFrame], Aliens.HitLocX, Aliens.HitLocY, 0);
   }
	// Green Alien Hit
	else if((Aliens.HitAlien == 1) && (Aliens.HitFrame < 2))
   {
		SprDrv->TransBlt(GreenA[Aliens.HitFrame], Aliens.HitLocX, Aliens.HitLocY, 0);
   }
	// Yellow Alien Hit
	else if((Aliens.HitAlien == 2) && (Aliens.HitFrame < 2))
   {
		SprDrv->TransBlt(YellowA[Aliens.HitFrame], Aliens.HitLocX, Aliens.HitLocY, 0);
   }
	Aliens.HitFrame++;
};

// ##################################
// #<<<<<< Alien Array Status >>>>>>#
// ##################################
void ArrayStatus()
{
	// ############################
	// #<<<<<<<< Left Side >>>>>>>#
	// ############################
	if((Invaders[0][0] == 0) && (Invaders[0][1] == 0) && (Invaders[0][2] == 0))
	{
		Aliens.XOffset = 50;
   }
	if((Invaders[1][0] == 0) && (Invaders[1][1] == 0)&&
   	(Invaders[1][2] == 0) && (Aliens.XOffset == 50))
	{
		Aliens.XOffset = 100;
   }
	if((Invaders[2][0] == 0) && (Invaders[2][1] == 0) &&
   	(Invaders[2][2] == 0) && (Aliens.XOffset == 100))
   {
		Aliens.XOffset = 150;
   }
	if((Invaders[3][0] == 0) && (Invaders[3][1] == 0) &&
   	(Invaders[3][2] == 0) && (Aliens.XOffset == 150))
   {
		Aliens.XOffset = 200;
   }
	if((Invaders[4][0] == 0) && (Invaders[4][1] == 0) &&
   	(Invaders[4][2] == 0) && (Aliens.XOffset == 200))
   {
		Aliens.XOffset = 250;
   }

	// ############################
	// #<<<<<<< Right Side >>>>>>>#
	// ############################
	if((Invaders[5][0] == 0) && (Invaders[5][1] == 0) && (Invaders[5][2] == 0))
	{
		Aliens.WidthOffset = 50;
	}
	if((Invaders[4][0] == 0) && (Invaders[4][1] == 0)&&
   	(Invaders[4][2] == 0) && (Aliens.WidthOffset == 50))
	{
		Aliens.WidthOffset = 100;
   }
	if((Invaders[3][0] == 0) && (Invaders[3][1] == 0) &&
   	(Invaders[3][2] == 0) && (Aliens.WidthOffset == 100))
   {
		Aliens.WidthOffset = 150;
   }
	if((Invaders[2][0] == 0) && (Invaders[2][1] == 0) &&
   	(Invaders[2][2] == 0) && (Aliens.WidthOffset == 150))
   {
		Aliens.WidthOffset = 200;
   }
	if((Invaders[1][0] == 0) && (Invaders[1][1] == 0) &&
   	(Invaders[1][2] == 0) && (Aliens.WidthOffset == 200))
   {
		Aliens.WidthOffset = 250;
   }

	// ############################
	// #<<<<<<<<< Bottom >>>>>>>>>#
	// ############################
   if((Invaders[0][2] == 0) && (Invaders[1][2] == 0) &&
   	(Invaders[2][2] == 0) && (Invaders[3][2] == 0) &&
      (Invaders[4][2] == 0) && (Invaders[5][2] == 0) &&
      (Aliens.HeightOffset == 150))
   {
   	Aliens.HeightOffset -= 50;
	}
   if((Invaders[0][1] == 0) && (Invaders[1][1] == 0) &&
   	(Invaders[2][1] == 0) && (Invaders[3][1] == 0) &&
      (Invaders[4][1] == 0) && (Invaders[5][1] == 0) &&
      (Aliens.HeightOffset == 100))
   {
   	Aliens.HeightOffset -= 50;
	}
};

// ##################################
// #<<<<<<<<< Explode Alien >>>>>>>>#
// ##################################
void DecoyStatus()
{
	// ##############################
	// #<<<<<<<<< Hero Ammo >>>>>>>>#
	// ##############################
	// Left Decoy
	if((HeroStat.AmmoX >= 50) && (HeroStat.AmmoX <= 100) &&
   	(HeroStat.AmmoY >= 300) && (HeroStat.AmmoY <= 350))
   {
		Sheilds[0].HitCount++;

		if(Sheilds[0].HitCount <= 4)
		{
	      HeroStat.AmmoY	= 999;
	      sndPlaySound("Splat.wav", SND_ASYNC);
      }
	}

	// Right Decoy
  	if((HeroStat.AmmoX >= 400) && (HeroStat.AmmoX <= 450) &&
   	(HeroStat.AmmoY >= 300) && (HeroStat.AmmoY <= 350))
   {
      Sheilds[1].HitCount++;

   	if(Sheilds[1].HitCount <= 4)
		{
	      HeroStat.AmmoY	= 999;
	      sndPlaySound("Splat.wav", SND_ASYNC);
      }
	}

	// ##############################
	// #<<<<<<<< Alien Ammo >>>>>>>>#
	// ##############################
	// Left Decoy
	if((Aliens.AmmoX >= 50) && (Aliens.AmmoX <= 100) &&
   	(Aliens.AmmoY >= 300) && (Aliens.AmmoY <= 350))
   {
		Sheilds[0].HitCount++;

		if(Sheilds[0].HitCount <= 4)
		{
	      Aliens.AmmoY	= 999;
	      sndPlaySound("Splat.wav", SND_ASYNC);
		}
	}

	// Right Decoy
  	if((Aliens.AmmoX >= 400) && (Aliens.AmmoX <= 450) &&
   	(Aliens.AmmoY >= 300) && (Aliens.AmmoY <= 350))
   {
      Sheilds[1].HitCount++;

   	if(Sheilds[1].HitCount <= 4)
		{
	      Aliens.AmmoY	= 999;
	      sndPlaySound("Splat.wav", SND_ASYNC);
      }
	}

	// ##############################
	// #<<<<<<<< Bonus Ammo >>>>>>>>#
	// ##############################
	if((BonusAli.AmmoX >= 50) && (BonusAli.AmmoX <= 100) &&
   	(BonusAli.AmmoY >= 300) && (BonusAli.AmmoY <= 350))
   {
		Sheilds[0].HitCount++;

		if(Sheilds[0].HitCount <= 4)
		{
	      BonusAli.AmmoY	= 999;
	      sndPlaySound("Splat.wav", SND_ASYNC);
      }
	}
	// Right Decoy
  	if((BonusAli.AmmoX >= 400) && (BonusAli.AmmoX <= 450) &&
   	(BonusAli.AmmoY >= 300) && (BonusAli.AmmoY <= 350))
   {
      Sheilds[1].HitCount++;

   	if(Sheilds[1].HitCount <= 4)
		{
	      BonusAli.AmmoY	= 999;
	      sndPlaySound("Splat.wav", SND_ASYNC);
      }
	}

	// Left Sheild Destroyed
	if(Sheilds[0].HitCount == 4)
	{
		Sheilds[0].HitCount++;
   	Explode.ExplodeX 		= 50;
  	   Explode.ExplodeY 		= 300;
      Explode.Frame 			= 0;
	}
	// Right Sheild Destroyed
	if(Sheilds[1].HitCount == 4)
	{
		Sheilds[1].HitCount++;
   	Explode.ExplodeX 		= 400;
  	   Explode.ExplodeY 		= 300;
      Explode.Frame 			= 0;
	}
};

// ############################
// #<<<<<< Move Aliens >>>>>>>#
// ############################
void MoveAliens(HWND hWnd)
{
	if((Aliens.Counter == MOV_TIMER) || (Aliens.Counter == MOV_TIMER + 3))
	{
 		// Prevent Aliens From Being Clipped Off Screen
		if((Aliens.ArrayX + ARRAYWIDTH - Aliens.WidthOffset >= 500) &&
        	(Aliens.dir == ALI_RIGHT))
		{
			Aliens.dir	= ALI_LEFT;
		}
		else if((Aliens.ArrayX + Aliens.XOffset <= 0) && (Aliens.dir == ALI_LEFT))
		{
			Aliens.ArrayY += 10;
			Aliens.dir 	= ALI_RIGHT;
		}

		// Move Aliens Horizontally
		if(Aliens.dir == ALI_RIGHT)
        	Aliens.ArrayX += Aliens.Momentum + Game.Level;
		else if(Aliens.dir == ALI_LEFT)
        	Aliens.ArrayX -= Aliens.Momentum + Game.Level;
/*		else if((Aliens.dir == ALI_RIGHT) && (Aliens.NumAliens < 9))
         Aliens.ArrayX += 17 + Game.Level;
		else if((Aliens.dir == ALI_LEFT) && (Aliens.NumAliens < 9))
     		Aliens.ArrayX -= 17 + Game.Level;*/
	}

	// Check if Hero Surrounded
   if((Aliens.ArrayY + Aliens.HeightOffset >= 300) &&
   	(Game.Status != STAT_VICTORY))
   {
   	Game.Status = STAT_VICTORY;
      if(Music.Capiable == TRUE)
      {
      	if(Music.Playing == TRUE)
         {
				MidiDrv->StopMIDI(hWnd);
            MidiDrv->CloseMIDI(hWnd);
         }
			Music.Current = MUS_ALIEN;
			MidiDrv->OpenMIDI(hWnd, JukeBox[Music.Current]);
			MidiDrv->PlayMIDI(hWnd);
	      Music.Playing = TRUE;
      }
   }
};

// ############################
// #<<<<<< Hero's Ammo >>>>>>>#
// ############################
void HandleHeroAmmo()
{
	// Reset Ammo Position
	if(HeroStat.AmmoY < 0)
		HeroStat.AmmoY	= 999;

	if(HeroStat.AmmoY < 999)
	{
		HeroStat.AmmoY -= 10;

		// Animate Ammo
		if(HeroStat.AmmoFrame == 1)
        	HeroStat.AmmoFrame = 0;
		else
        	HeroStat.AmmoFrame = 1;

		// Display Ammo
		SprDrv->TransBlt(HeroAmmo[HeroStat.AmmoFrame], HeroStat.AmmoX, HeroStat.AmmoY, 0);
	}
};

// ############################
// #<<<<<< Animate Aliens >>>>#
// ############################
void AnimateAliens()
{
	if(Aliens.Counter == ANI_TIMER)
	{
		Aliens.Counter = 0;

		// Animate Aliens
		if(Aliens.CurFrame == ALI_FRAME3)
        	Aliens.CurFrame = ALI_FRAME1;
		else
			Aliens.CurFrame++;
	}
	Aliens.Counter++;
};

// ##################################
// #<<<<<< Handle Alien Ammo >>>>>>>#
// ##################################
void AliensAmmo(HWND hWnd)
{
	int RandNum;
   int Height = 150;

	randomize();
   if(Aliens.AmmoY >= 450)
   {
		for(int i = 0; i < 6; i++)
      {
			RandNum = rand() % 6;
			if(Invaders[RandNum][2] == 0)
				Height -= 50;
         if((Invaders[RandNum][1] == 0) && (Invaders[RandNum][2] == 0))
         	Height -= 50;
         if((Invaders[RandNum][0] == 0) && (Invaders[RandNum][1] == 0))
         	Height -= 50;

         if(Height > 0)
         {
				Aliens.AmmoX = Aliens.ArrayX + RandNum * 50 + 20;
		      Aliens.AmmoY = Aliens.ArrayY + Height;
				break;
         }
      }
   }
   if((Aliens.AmmoX > HeroStat.ShipX) && (Aliens.AmmoX < HeroStat.ShipX + 50) &&
   	(Aliens.AmmoY > 350) && (Aliens.AmmoY < 400))
   {
		HeroStat.Hit			= TRUE;
   	Explode.ExplodeX 		= HeroStat.ShipX;
  	   Explode.ExplodeY 		= 350;
      Explode.Frame 			= 0;
      Aliens.AmmoY			= 999;
   	Game.Lives 				-= 1;
   }
   if((HeroStat.Hit == TRUE) && (Explode.Frame == 5))
		StartGame(hWnd, Game.Level);

	Aliens.AmmoY += 12;
	SprDrv->TransBlt(AlienAmmo[Aliens.AmmoFrm], Aliens.AmmoX, Aliens.AmmoY, 0);
};

// ##################################
// #<<<<<< Explode An Object >>>>>>>#
// ##################################
void ExplodeObject()
{
	if(Explode.Frame == 0)
   	sndPlaySound("Explode.Wav", SND_ASYNC);

   if(Explode.Frame < 8)
	{
		SprDrv->TransBlt(Explosion[Explode.Frame], Explode.ExplodeX, Explode.ExplodeY, 0);
		Explode.Frame++;
   }
};

// ##################################
// #<<<<<< Display Bonus Alien >>>>>#
// ##################################
void DisplayBonus()
{
	randomize();
   
	if((Aliens.ArrayY >= 50) && (BonusAli.Destroyed == FALSE))
   {
		BonusAli.Counter++;
      BonusAli.AmmoY += 10;
		BonusAli.AmmoFrm++;
      
      if(BonusAli.Counter >= 40 + (rand() % 20))
      {
			BonusAli.XPos += rand() % 11;
             
			BonusAli.Frame++;

			// Animate Bonus Alien Ship
		   if(BonusAli.Frame > BNS_FRAME2)
		   	BonusAli.Frame = BNS_FRAME1;

			// Check if Off Screen
		   if(BonusAli.XPos >= 550)
         {
		   	BonusAli.XPos		=- 50;
				BonusAli.Counter	= 0;
			}

			// Detect Hero's Ammo
         if((HeroStat.AmmoX 		>= BonusAli.XPos) &&
         	(HeroStat.AmmoX + 5	<= BonusAli.XPos + 50) &&
            (HeroStat.AmmoY 		>= 0) &&
            (HeroStat.AmmoY + 5	<= 50) &&
            (BonusAli.Destroyed == FALSE))
         {
				HeroStat.AmmoY			= 999;
            Game.Score				+= 250;
		   	Explode.ExplodeX 		= BonusAli.XPos;
		  	   Explode.ExplodeY 		= 0;
		      Explode.Frame 			= 0;
				BonusAli.Destroyed 	= TRUE;
         }

			// Display Bonus
			if(BonusAli.Destroyed == FALSE)
				SprDrv->TransBlt(Bonus[BonusAli.Frame], BonusAli.XPos, 0, 0);
		}

		// Animate Ammo
		if(BonusAli.AmmoFrm > BNS_AMMO2)
      	BonusAli.AmmoFrm = BNS_AMMO1;
         
		// Unsuccessful Ammo
		if(BonusAli.AmmoY >= 450)
		{
			BonusAli.AmmoY = 50;
			BonusAli.AmmoX = BonusAli.XPos + 10;
      }

		// Display Bonus Ammo
		SprDrv->TransBlt(Bonus[BonusAli.AmmoFrm], BonusAli.AmmoX, BonusAli.AmmoY, 0);
   }
};

// ##################################
// #<<<<<<< Bonus Alien Ammo >>>>>>>#
// ##################################
void BonusAmmo(HWND hWnd)
{
   if((BonusAli.AmmoX > HeroStat.ShipX) && (BonusAli.AmmoX < HeroStat.ShipX + 50) &&
   	(BonusAli.AmmoY > 350) && (BonusAli.AmmoY < 400))
   {
		HeroStat.Hit			= TRUE;
   	Explode.ExplodeX 		= HeroStat.ShipX;
  	   Explode.ExplodeY 		= 350;
      Explode.Frame 			= 0;
      BonusAli.AmmoY			= 999;
   	Game.Lives 				-= 1;
   }
   if((HeroStat.Hit == TRUE) && (Explode.Frame == 5))
		StartGame(hWnd, Game.Level);
};

// ##################################
// #<<<<<<< Load High Scores >>>>>>>#
// ##################################
void LoadScores()
{
	char Buff[50];
   char Key[6];
	char *P;
      
   for(int i = 0; i < 10; i++)
   {
		wsprintf(Key, "High%d", i+1);
		GetPrivateProfileString("Scores", Key, "Unknown", Buff, sizeof(Buff), "Invaders.ini");

      // Get Score
		P = strtok(Buff, ",");		     
      Scores[i] = atoi(Buff);

		// Get Name
		P = strtok(NULL, ",");	
      wsprintf(Names[i], "%s", P);  
   }
};

// ##################################
// #<<<<<<<< Add High Scores >>>>>>>#
// ##################################
void AddHigh(char *Name, int Score)
{
	char	Buff[250];
	char	Label[10];
	int	Entry;
      
	for(int i = 0; i < 10; i++)
   {
		// Find Entry To Modify
		if(Score >= Scores[i])
      {
			// Save Entry
			Entry = i;
         
			for(i = 9; i > Entry; i--)
			{
				Scores[i] = Scores[i - 1];
				wsprintf(Names[i], "%s", Names[i - 1]);
			}

			// Set Entries New Settings
			Scores[Entry]	= Score;
         wsprintf(Names[Entry], "%s", Name);

			// Write Scores
			for(i = 0; i < 10; i++)
         {
				wsprintf(Label, "High%d", i+1);
            wsprintf(Buff, "%d, %s", Scores[i], Names[i]);
            WritePrivateProfileString("Scores", Label, Buff, "Invaders.ini");
         }
         break;
      }
   }
};

// ##################################
// #<<<<<<< Load User Options >>>>>>#
// ##################################
void ReadInitFile(HWND hWnd)
{
	JOYINFO	JoyInfo;
   JOYCAPS	JoyCaps;
   char		Buff[50];
   
	// If Joystick Port Exists
	if(joyGetNumDevs() > 0)
   {
	  	GetPrivateProfileString("Settings", "JoyStick", "NULL", Buff, sizeof(Buff), "Invaders.ini");

		if(strstr(Buff, "JOY1"))
		{
			joySetCapture(hWnd, JOYSTICKID1, NULL, TRUE);
			joyGetDevCaps(JOYSTICKID1, &JoyCaps, sizeof(JOYCAPS));
      }
      else if(strstr(Buff, "JOY2"))
      {
			joySetCapture(hWnd, JOYSTICKID2, NULL, TRUE);
			joyGetDevCaps(JOYSTICKID2, &JoyCaps, sizeof(JOYCAPS));      
      }
		// Set Joystick Center
		JoyCenter = JoyCaps.wXmax / 2;
	}

};

// ##################################
// #<<<<< Display Game's Story >>>>>#
// ##################################
void DisplayStory()
{
	char *Story[14];
	Story[0] = "   All the years of wondering what was beyond the";
	Story[1] = "black void of darkness, has come to an end... With";
	Story[2] = "the discovery of another lifeform, an unfriendly";
	Story[3] = "non-carbon based lifeform looking for a new place";
	Story[4] = "to live. Your mission is top secret, and there's";
	Story[5] = "no one to help you once you embark. The";
	Story[6] = "government has sent you secretly, seeing no other";
	Story[7] = "possible solution and unaware to the people that";
	Story[8] = "argued for well being of this new found lifeform.";
	Story[9] = "   Who's right? Who's to say, all you do know";
	Story[10]= "is that right now its, Carbon Vs. Non-Carbon!";
	Story[11]= "";
	Story[12]= "                 Jeremiah McCarthy";
	Story[13]= "            <jmccarth@terra.nlnet.nf.ca>";

	// Set Font Settings
	SprDrv->SetFont(10, 15, Green);

	// Display Text to Sprite DC
	for(int i = 0; i < 14; i++)
		SprDrv->DisplayText(Story[i], 5, 120 + i * 15);

   // Delete Font Settings
	SprDrv->DeleteFont();
};

// ##################################
// #<<<<< Display Creator Info >>>>>#
// ##################################
void DisplayCreator()
{
	char *Creator[10];
	SprDrv->TransBlt(Misc[JEREMIAH], 180, 55, 255);
	Creator[0] = "   Space Invaders: The Year We Make Contact";
	Creator[1] = "";
	Creator[2] = "   Was made by Jeremiah McCarthy, at fourteen.";
	Creator[3] = "If you have any suggesstions, problems; which";
	Creator[4] = "I am not responsible for in anyway.  Please";
	Creator[5] = "E-Mail me, I'd love to hear from you. Now kick";
	Creator[6] = "some silicon based Butt!";
   Creator[7] = "";
	Creator[8] = "                Jeremiah McCarthy";
	Creator[9] = "            <jmccarth@terra.nlnet.nf.ca>";

	SprDrv->SetFont(10, 15, Yellow);
	for(int i = 0; i < 10; i++)
		SprDrv->DisplayText(Creator[i], 15, 170 + i * 15);
	SprDrv->DeleteFont();
};

// ##################################
// #<<<<<< Display Information >>>>>#
// ##################################
void DisplayInfo()
{
	char *Info[10];

	SprDrv->TransBlt(Misc[JEREMIAH], 15, 185, 255);
	SprDrv->TransBlt(Misc[LEPRELOGO], 415, 175, 0);

	Info[0] = "Space Invaders: The Year We Make Contact v2.0";
	Info[1] = "";
	Info[2] = "      Copyright(C) 1995 Leprechaun Software";
	Info[3] = "                ALL RIGHTS RESERVED";
	Info[4] = "";
	Info[5] = "              Register Space Invaders 2.0";
	Info[6] = "                    Game: $15 Canadian";
	Info[7] = "";
	Info[8] = "                   Jeremiah McCarthy";
	Info[9] = "              <jmccarth@terra.nlnet.nf.ca>";

	SprDrv->SetFont(10, 15, DarkMagenta);
	for(int i = 0; i < 4; i++)
		SprDrv->DisplayText(Info[i], 20, 80 + i * 15);
	SprDrv->DeleteFont();

	SprDrv->SetFont(10, 15, Magenta);
	for(int i = 4; i < 10; i++)
		SprDrv->DisplayText(Info[i], 20, 230 + i * 15);
	SprDrv->DeleteFont();
};

// ##################################
// #<<<<<<< About Dlg Proc >>>>>>>>>#
// ##################################
int _export FAR PASCAL AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
   	case WM_COMMAND:
      	switch(wParam)
         {
         	case IDC_OK:
            	EndDialog(hDlg, TRUE);
               break;
         }
         return 0;
	}
	return 0;
};

// ##################################
// #<<<<<< Settings Dlg Proc >>>>>>>#
// ##################################
int _export FAR PASCAL SettingsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	JOYINFO	JoyInfo;
   JOYCAPS	JoyCaps;

	switch(message)
	{
		case WM_INITDIALOG:
         if(joyGetNumDevs() > 0)
			{
				if(joyGetPos(JOYSTICKID1, &JoyInfo) != JOYERR_NOERROR)
					EnableWindow(GetDlgItem(hDlg, IDC_JOYSTICK1), FALSE);
				if(joyGetPos(JOYSTICKID2, &JoyInfo) != JOYERR_NOERROR)
					EnableWindow(GetDlgItem(hDlg, IDC_JOYSTICK2), FALSE);
         }
         else
         {
				EnableWindow(GetDlgItem(hDlg, IDC_JOYSTICK1), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_JOYSTICK2), FALSE);
         }
      	return 0;

   	case WM_COMMAND:
      	switch(wParam)
         {
				case IDC_CHGRES:
					WinExec("Control Desk.cpl", SW_SHOW);
               break;

            case IDC_CONFIGJOY:
            	WinExec("joy.cpl", SW_NORMAL);
					SendMessage(hDlg, WM_INITDIALOG, 0, 0L);
               break;

				case IDCANCEL:
            	EndDialog(hDlg, TRUE);
               break;

         	case IDOK:
					if(IsDlgButtonChecked(hDlg, IDC_JOYSTICK1))
               {
               	WritePrivateProfileString("Settings", "JoyStick", "JOY1", "Invaders.ini");
						joySetCapture(GetParent(hDlg), JOYSTICKID1, NULL, TRUE);
						joyGetDevCaps(JOYSTICKID1, &JoyCaps, sizeof(JOYCAPS));
               }
					if(IsDlgButtonChecked(hDlg, IDC_JOYSTICK2))
               {
               	WritePrivateProfileString("Settings", "JoyStick", "JOY2", "Invaders.ini");
						joySetCapture(GetParent(hDlg), JOYSTICKID2, NULL, TRUE);
						joyGetDevCaps(JOYSTICKID2, &JoyCaps, sizeof(JOYCAPS));
               }
					JoyCenter = JoyCaps.wXmax / 2;
            	EndDialog(hDlg, TRUE);
               break;
         }
         return 0;
	}
	return 0;
};

// ##################################
// #<<<<<<< Scores Dlg Proc >>>>>>>>#
// ##################################
int _export FAR PASCAL ScoresDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char Buff[50];
	int i;

	switch(message)
	{
		case WM_INITDIALOG:
			LoadScores();
			for(i = 0; i < 10; i++)
         {
				itoa(Scores[i], Buff, 10); 
	         SendDlgItemMessage(hDlg, IDC_NAMES, LB_ADDSTRING, NULL, (LPARAM)Names[i]);
	         SendDlgItemMessage(hDlg, IDC_SCORES, LB_ADDSTRING, NULL, (LPARAM)Buff);
         }
			return 0;

		case WM_COMMAND:
      	switch(wParam)
         {
				case IDOK:
		      	EndDialog(hDlg, TRUE);
		         return 0;
      	}
         return 0;
               
		case WM_CLOSE:
      	EndDialog(hDlg, TRUE);
         return 0;

	}
	return 0;
}

// ##################################
// #<<<< New High Score Dlg Proc >>>#
// ##################################
int _export FAR PASCAL NewHighDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char Buff[250];

	switch(message)
	{
		case WM_INITDIALOG:
			wsprintf(Buff, "High Score: %d", Game.Score);
			SetDlgItemText(hDlg, IDC_SCORE, Buff);
			return 0;

		case WM_COMMAND:
      	switch(wParam)
         {
				case IDOK:
		      	EndDialog(hDlg, TRUE);
		         return 0;
      	}
         return 0;
               
		case WM_DESTROY:
			GetDlgItemText(hDlg, IDC_NAME, Buff, sizeof(Buff));
      	AddHigh(Buff, Game.Score);
        	DialogBox(WinApp.hInstance, MAKEINTRESOURCE(IDD_SCORES), GetParent(hDlg), (DLGPROC)ScoresDlgProc);
         return 0;
         
		case WM_CLOSE:
      	EndDialog(hDlg, TRUE);
         return 0;

	}
	return 0;
}
