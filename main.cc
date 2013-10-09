#define CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

#include <process.h>
#include "hge_include.h"
#include "resource_manage.h"

int frame = 0, direction = 0;
int mouse_frame = 0, mouse_direction = 0;

bool FrameFunc()
{
	return false;
}

bool RenderFunc()
{
	HGE* pHge = hgeCreate( HGE_VERSION);
	pHge->Gfx_BeginScene();
	pHge->Gfx_Clear( 0xFFFFFFFF);
  
  Texture mouse = g_resource_manage->GetTex( "cursor.was");
  Texture tex = g_resource_manage->GetTex( "1.was");
  float x,y;
  pHge->Input_GetMousePos( &x, &y);
  tex.Render( Coordinate(400, 400), frame, direction);
  mouse.Render( Coordinate( x, y), mouse_frame, mouse_direction);
  ++frame;
  ++mouse_frame;
  if( frame >= tex.GetFrameCount()-1){
    frame = 0, ++direction;
    if( direction >= tex.GetDirectionCount()-1) {
      direction = 0;
    }
  }
  if( mouse_frame >= mouse.GetFrameCount()-1){
    mouse_frame = 0, ++mouse_direction;
    if( mouse_direction >= mouse.GetDirectionCount()-1) {
      mouse_direction = 0;
    }
  }
  
	pHge->Gfx_EndScene();
	return false;
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

	if( pHge->System_Initiate())
	{
		pHge->System_Start();
	}
	pHge->Release();


  return 0;
}
