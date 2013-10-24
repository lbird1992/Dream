#include "game_logic.h"
#include "game_manage.h"

const int MAX = 32;
void GameLogic::LoadMapIni() {
  int i;
  FILE* fp = fopen( "data\\map.ini","r");
  while( fscanf( fp, "%d", &i) != EOF)
    fscanf(fp, "%d %d %d", &map_data_[i].map_image, &map_data_[i].map_x_max, &map_data_[i].map_y_max);
  fclose(fp);
}

void GameLogic::Walk() {
  float x_length = (map_coordinate_to_go_ - map_coordinate_).GetX();
  float y_length = (map_coordinate_to_go_ - map_coordinate_).GetY();
  if( x_length * x_length + y_length * y_length < kWalkSpeed * kWalkSpeed) {
    //偏移量不足一帧移动
    map_coordinate_ = map_coordinate_to_go_;
    g_game_manage->GetMap()->GetPlayerGUI()->EndAnimation();
    return;
  }
  double x_speed, y_speed;
  if( x_length == 0) {
    x_speed = 0;
    y_speed = kWalkSpeed;
  }
  else {
    x_speed = kWalkSpeed * cos( atan( abs(y_length)/abs(x_length)));
    y_speed = kWalkSpeed * sin( atan( abs(y_length)/abs(x_length)));
  }
  if( x_length < 0)
    x_speed *= -1;
  if( y_length < 0)
    y_speed *= -1;
  if( g_game_manage->GetMap()->GetCell( 
                          map_coordinate_ + Coordinate(static_cast<float>(x_speed), 
                                                      static_cast<float>(y_speed))) == 1)
  {
    map_coordinate_to_go_ = map_coordinate_;
    g_game_manage->GetMap()->GetPlayerGUI()->EndAnimation();
    return;
  }
  map_coordinate_.Plus( Coordinate(static_cast<float>(x_speed), static_cast<float>(y_speed)));
  
  if( x_speed > 0 && y_speed > 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 0, true);
  else if( x_speed < 0 && y_speed > 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 1, true);
  else if( x_speed < 0 && y_speed < 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 2, true);
  else if( x_speed > 0 && y_speed < 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 3, true);
  else if( x_speed == 0 && y_speed > 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 4, true);
  else if( x_speed < 0 && y_speed == 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 5, true);
  else if( x_speed == 0 && y_speed < 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 6, true);
  else if( x_speed > 0 && y_speed == 0)
    g_game_manage->GetMap()->GetPlayerGUI()->AddAnimation( 2, 7, true);
}


int Player::NewPlayer(int ID,char* name,int image){
  memset(this,0,sizeof(Player));
  char name[256];
	
	
	Player hero;
	
	hero.ID_ = ID;
	strcpy( hero.name, name);
	hero.image_ = image;

	if( image_/4 == 0)
	{//人族
		hero.tizhi_ = hero.moli_ = hero.liliang_ = hero.naili_ = hero.minjie_ = 10;
		hero.hp_ = hero.hp2_ = hero.hp3_ = 150;
		hero.mp_ = hero.mp2_ = 110;
		hero.shanghai_ = 41;
		hero.mingzhong_ = 50;
		hero.fangyu_ = 15;
		hero.duobi_ = 10;
		hero.sudu_ = 10;
		hero.lingli_ = 16;
	}
	else if( image_/4 == 1)
	{//天族
		hero.tizhi_ = 12, hero.moli_ = 5, hero.liliang_ = 11, hero.naili_ = 12, hero.minjie_ = 10;
		hero.hp_ = hero.hp2_ = hero.hp3_ = 154;
		hero.mp_ = hero.mp2_ = 97.5;
		hero.shanghai_ = 46.6;
		hero.mingzhong_ = 48.7;
		hero.fangyu_ = 19.2;
		hero.duobi_ = 10;
		hero.sudu_ = 10.5;
		hero.lingli_ = 13.9;
	}
	else
	{//魔族
		hero.tizhi_ = 12, hero.moli_ = 11, hero.liliang_ = 11, hero.naili_ = 8, hero.minjie_ = 8;
		hero.hp_ = hero.hp2_ = hero.hp3_ = 172;
		hero.mp_ = hero.mp2_ = 107.5;
		hero.shanghai_ = 42.8;
		hero.mingzhong_ = 52.3;
		hero.fangyu_ = 10.4;
		hero.duobi_ = 8;
		hero.sudu_ = 8.7;
		hero.lingli_ = 17.3;
	}
	hero.mapID_ = 1;
	hero.mapX_ = hero.mapAimX_ = 30;
	hero.mapY_ = hero.mapAimY_ = 30;
	hero.tili2_ = hero.huoli2_ = 100;
	hero.xp_ = 0;
	
	return 0;
}

void Player::ChangePoint(int playerID, int tizhi, int moli, int liliang, int naili, int minjie){
 /* mpHeroLock.Lock();
	Hero hero = mpHero[ playerID];
	mpHeroLock.Unlock();*/
  Player hero;
	if( tizhi + moli + liliang + naili + minjie > hero.qianli_)
		return;
	if( hero.image_/4 == 0)//人族
	{
		hero.hp3_ += tizhi*5;
		hero.mp2_ += moli*3;
		hero.shanghai_ += liliang*0.67;
		hero.mingzhong_ += liliang*2.01;
		hero.fangyu_ += naili*1.5;
	}
	else if( hero.image_/4 == 1)//天族
	{
		hero.hp3_ += tizhi*4.5;
		hero.mp2_ += moli*3.5;
		hero.shanghai_ += liliang*0.57;
		hero.mingzhong_ += liliang*1.71;
		hero.fangyu_ += naili*1.6;
	}
	else//魔族
	{
		hero.hp3_ += tizhi*6;
		hero.mp2_ += moli * 2.5;
		hero.shanghai_ += liliang*0.77;
		hero.mingzhong_ += liliang*2.31;
		hero.fangyu_ += naili*1.4;
	}
	hero.lingli_ += tizhi*0.3 + moli * 0.7 + naili*0.2 + liliang*0.4;
	hero.sudu_ += tizhi*0.1 + naili*0.1 + liliang*0.1 + minjie * 0.7;
	hero.duobi_ += minjie;
	if( tizhi != 0)
		hero.hp_ = hero.hp2_ = hero.hp3_;
	if( moli != 0)
		hero.mp_ = hero.mp2_;
	//强身冥想等属性影响未添加

	hero.qianli_ -= tizhi + moli + liliang + naili + minjie;
	hero.tizhi_ += tizhi;
	hero.moli_ += moli;
	hero.liliang_ += liliang;
	hero.naili_ += naili;
	hero.minjie_ += minjie;
	//mpHeroLock.Lock();
	//mpHero[ playerID] = hero;
	//mpHeroLock.Unlock();
}

void Player::LoadPlayer(FILE* fc) const{
  int i = 0,j,k;
  Player temp;
  for(i=0;i<MAX;i++){
     fread( &temp.ID_, sizeof(ID_), 1, fc);
     fread( &temp.image_, sizeof(temp.image_), 1, fc);
     fread( &temp.weaponImage_, sizeof(temp.weaponImage_), 1, fc);
     fread(temp.name,sizeof(char), 20, fc);
     fread( &temp.renqi_, sizeof(temp.renqi_), 1, fc);
     fread( &temp.menpai_, sizeof(temp.menpai_), 1, fc);
     fread( &temp.menpaigongxian_, sizeof(temp.menpaigongxian_), 1, fc);
     fread( &temp.money_, sizeof(temp.money_), 1, fc);
     fread( &temp.bank_, sizeof(temp.bank_), 1, fc);
     fread( &temp.hp_, sizeof(temp.hp_), 1, fc);
     fread( &temp.hp2_, sizeof(temp.hp2_), 1, fc);
     fread( &temp.hp3_, sizeof(temp.hp3_), 1,fc);
     fread( &temp.mp_, sizeof(temp.mp_), 1, fc);
     fread( &temp.mp2_, sizeof(temp.mp2_), 1, fc);
     fread( &temp.sp_, sizeof(temp.sp_), 1,fc);
     fread( &temp.huoli_, sizeof(temp.huoli_), 1, fc);
     fread( &temp.huoli2_, sizeof(temp.huoli2_), 1, fc);
     fread( &temp.tili_, sizeof(temp.tili_), 1, fc);
     fread( &temp.tili2_, sizeof(temp.tili2_), 1, fc);

     fread( &temp.mingzhong_, sizeof(temp.mingzhong_), 1, fc);
     fread( &temp.shanghai_, sizeof(temp.shanghai_), 1, fc);
     fread( &temp.fangyu_, sizeof(temp.fangyu_), 1, fc);

     fread( &temp.sudu_, sizeof(temp.sudu_), 1, fc);
     fread( &temp.duobi_, sizeof(temp.duobi_), 1, fc);
     fread( &temp.lingli_, sizeof(temp.lingli_), 1, fc);
     fread( &temp.tizhi_, sizeof(temp.tizhi_), 1, fc);
     fread( &temp.moli_, sizeof(temp.moli_), 1, fc);
     fread( &temp.liliang_, sizeof(temp.liliang_), 1, fc);
     fread( &temp.naili_, sizeof(temp.naili_), 1, fc);
     fread( &temp.minjie_, sizeof(temp.minjie_), 1, fc);
     fread( &temp.qianli_, sizeof(temp.qianli_), 1, fc);
     fread( &temp.xp_, sizeof(temp.xp_), 1, fc);
	}
  fclose(fc);
}


void Player::Save(FILE* fa) const{
  int i;
  Player temp;
  
  if(fa != NULL){
    for(i = 0;i<MAX;i++){
     fwrite( &temp.ID_, sizeof(ID_), 1, fa);
     fwrite( &temp.image_, sizeof(temp.image_), 1, fa);
     fwrite( &temp.weaponImage_, sizeof(temp.weaponImage_), 1, fa);
     fwrite(temp.name,sizeof(char), 20, fa);
     fwrite( &temp.renqi_, sizeof(temp.renqi_), 1, fa);
     fwrite( &temp.menpai_, sizeof(temp.menpai_), 1, fa);
     fwrite( &temp.menpaigongxian_, sizeof(temp.menpaigongxian_), 1, fa);
     fwrite( &temp.money_, sizeof(temp.money_), 1, fa);
     fwrite( &temp.bank_, sizeof(temp.bank_), 1, fa);
     fwrite( &temp.hp_, sizeof(temp.hp_), 1, fa);
     fwrite( &temp.hp2_, sizeof(temp.hp2_), 1, fa);
     fwrite( &temp.hp3_, sizeof(temp.hp3_), 1,fa);
     fwrite( &temp.mp_, sizeof(temp.mp_), 1, fa);
     fwrite( &temp.mp2_, sizeof(temp.mp2_), 1, fa);
     fwrite( &temp.sp_, sizeof(temp.sp_), 1,fa);
     fwrite( &temp.huoli_, sizeof(temp.huoli_), 1, fa);
     fwrite( &temp.huoli2_, sizeof(temp.huoli2_), 1, fa);
     fwrite( &temp.tili_, sizeof(temp.tili_), 1, fa);
     fwrite( &temp.tili2_, sizeof(temp.tili2_), 1, fa);

     fwrite( &temp.mingzhong_, sizeof(temp.mingzhong_), 1, fa);
     fwrite( &temp.shanghai_, sizeof(temp.shanghai_), 1, fa);
     fwrite( &temp.fangyu_, sizeof(temp.fangyu_), 1, fa);

     fwrite( &temp.sudu_, sizeof(temp.sudu_), 1, fa);
     fwrite( &temp.duobi_, sizeof(temp.duobi_), 1, fa);
     fwrite( &temp.lingli_, sizeof(temp.lingli_), 1, fa);
     fwrite( &temp.tizhi_, sizeof(temp.tizhi_), 1, fa);
     fwrite( &temp.moli_, sizeof(temp.moli_), 1, fa);
     fwrite( &temp.liliang_, sizeof(temp.liliang_), 1, fa);
     fwrite( &temp.naili_, sizeof(temp.naili_), 1, fa);
     fwrite( &temp.minjie_, sizeof(temp.minjie_), 1, fa);
     fwrite( &temp.qianli_, sizeof(temp.qianli_), 1, fa);
     fwrite( &temp.xp_, sizeof(temp.xp_), 1, fa);
    }
  }
  fclose(fa);
  
}



