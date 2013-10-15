#define CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

#include <process.h>
#include "hge_include.h"
#include "resource_manage.h"
#include "gui_manage.h"
#include "gui_dialog.h"
#include "gui_animation.h"
#include "gui_button.h"

int frame = 0, direction = 0;
int mouse_frame = 0, mouse_direction = 0;

bool FrameFunc()
{
  g_gui_manage->Control();
	return false;
}

bool RenderFunc()
{
	HGE* pHge = hgeCreate( HGE_VERSION);
	pHge->Gfx_BeginScene();
	pHge->Gfx_Clear( 0xFFFFFFFF);
  
  g_gui_manage->Render();
  Texture tex = g_resource_manage->GetTex( "cursor.was");
  tex.Render( g_gui_manage->GetMousePosition(), 0, 0);
  
	pHge->Gfx_EndScene();
	return false;
}

void ClickOnButton() {
  MessageBox( NULL, "µã»÷ÁË°´Å¥", NULL, MB_OK);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
 // _crtBreakAlloc = 236;

	HGE* pHge = hgeCreate( HGE_VERSION);
  pHge->System_SetState( HGE_LOGFILE, "log.txt");
	pHge->System_SetState( HGE_FRAMEFUNC, FrameFunc);
	pHge->System_SetState( HGE_RENDERFUNC, RenderFunc);
	pHge->System_SetState( HGE_WINDOWED, true);
	pHge->System_SetState( HGE_TITLE, "Dream Game");
	pHge->System_SetState( HGE_SCREENWIDTH, 640);
	pHge->System_SetState( HGE_SCREENHEIGHT, 480);
	pHge->System_SetState( HGE_USESOUND, true);
	pHge->System_SetState( HGE_DONTSUSPEND, true);
	pHge->System_SetState( HGE_SHOWSPLASH, false);//unuse the logo of hge
	pHge->System_SetState( HGE_ZBUFFER, false);
	pHge->System_SetState( HGE_FPS, 30);
  g_resource_manage = new ResourceManage();
  g_gui_manage = new GUIManage();
  g_gui_manage->Init();

  

	if( pHge->System_Initiate())
	{
    GUIDialog* dialog = new GUIDialog( "state_human.was", Coordinate( 50, 50));
    GUIButton* button = new GUIButton( "btnNext.was", Coordinate( 30, 30), dialog, ClickOnButton);
    dialog->AddSon( button);
    GUIAnimation* animation = new GUIAnimation( "1.was", Coordinate( 50, 40), dialog, LOOP);
    dialog->AddSon( animation);
    g_gui_manage->AddAnimation( animation);
    g_gui_manage->GetRoot()->AddSon( dialog);
    g_gui_manage->GetRoot()->SetOnDraw();
    dialog->SetOnDraw();
		pHge->System_Start();
	}
	pHge->Release();


  return 0;
}
