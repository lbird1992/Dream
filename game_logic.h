#ifndef _DREAM_GAMELOGIC_
#define _DREAM_GAMELOGIC_

#include <stdio.h>
#include <stdint.h>
#include "hge_include.h"

const int kMaxMapCount = 100;//游戏中地图单体的最大数量
const double kWalkSpeed = 0.27;//行走速度

enum HeroImage{
  HI_NONE = 0,

  HI_FEIYANNV = 1,
  HI_XIAOYAOSHEG = 2,
  HI_YINGNVXIA = 3,
  HI_JIANXIAKE = 4,
  HI_WUTIANJI = 5,
  HI_SHENTIANBING = 6,
  HI_XUANCAIE = 7,
  HI_LONGTAIZI = 8,
  HI_GUJINGLING = 9,
  HI_HUTOUGUANI = 10,
  HI_HUMEIREN = 11,
  HI_JUMOWANG = 12,

  HI_BAIJINGJING = 20,
  HI_BAIWUCHANG = 21,
  HI_BANGPAISHIYE = 22,
  HI_BUYUREN = 23,
  HI_CAIFENG = 24,
  HI_CHANGSHOUCUNCUNZHANG = 25,
  HI_CHENGFUREN = 26,
  HI_CHENGYAOJIN = 27,
  HI_CHULIANYI = 28,
  HI_CHUNSANSHINIANG = 29,
  HI_CUIHUA = 30,
  HI_DADAWANG = 31,
  HI_BUGAOLAN = 32,
  HI_CAIFENGTAI = 33,
  HI_DAZAOLU = 34,
  HI_JIGUANREN = 35,
  HI_BAIXAING = 36,
  HI_DAOTONG = 37,
  HI_DIZANGWANG = 38,
  HI_DIANXIAOER = 39,
  HI_DONGGAILONGWANG = 40,
  HI_ERBAO = 41,
  HI_ERDAWANG = 42,
  HI_FANGDUWEI = 43,
  HI_GUDONGSHANGREN = 44,
  HI_GUANYINJIEJIE = 45,
  HI_GUANJIA = 46,
  HI_HEIWUCHANG = 47,
  HI_HONGNIANG = 48,
  HI_HOUYIXIAN = 49,
  HI_HUANONG = 50,
  HI_HUOSHANG = 51,
  HI_JIANDINGSHANG = 52,
  HI_JIANGJUN = 53,
  HI_JIANGHUJIANSHANG = 54,
  HI_JIANGDAQUAN = 55,
  HI_JIAOFU = 56,
  HI_DEDAOGAOSENG = 57,
  HI_JIUDIANLAOBAN = 58,
  HI_KAOGUAN = 59,
  HI_LANHU = 60,  
  HI_LEIHEIZI = 61,
  HI_LIJING = 62,
  HI_LISHIMIN = 63,
  HI_LIANER = 64,
  HI_LUOBAIWANG = 65,
  HI_LUODAOREN = 66,
  HI_MUJIANG = 67,
  HI_NEZHA = 68,
  HI_NANJIXIANWENG = 69,
  HI_NANYOUXIGUANLIYUAN = 70,
  HI_NIUER = 71,
  HI_NNIUMOWANG = 72,
  HI_NVYOUXIGUANLIYUAN = 73,
  HI_PANGUAN = 74,
  HI_PANGSHANGREN = 75,
  HI_POPO = 76,
  HI_PUTIZUSHI = 77,
  HI_QIONGHAN = 78,
  HI_RANSESHI = 79,
  HI_SANDAWANG = 80,
  HI_SHIZHONGXIAN = 81,
  HI_CHAOJIWUYI = 82,
  HI_SHUSHENG = 83,
  HI_SHUIYUNXIAN = 84,
  HI_SUNPOPO = 85,
  HI_SUNXIAOSAN = 86,
  HI_TAIBAIJINGXING = 87,
  HI_TAISHANGLAOJUN = 88,
  HI_TANGSENG = 89,
  HI_TIANGONGCHUANSONGREN = 90,
  HI_TIEJIANG = 91,
  HI_TIEJIANGTUDI = 92,
  HI_TUDIGONGGONG = 93,
  HI_WANGFUREN = 94,
  HI_WANGMUNIANGNIANG = 95,
  HI_XIAOBAILONG = 96,
  HI_YAYI = 97,
  HI_YANRUYU = 98,
  HI_YANGJIAN = 99,
  HI_YAODIANLAOBAN = 100,
  HI_YONGREN = 101,
  HI_YUDI = 102,
  HI_YUELAO = 103,
  HI_YUNYOUSENG = 104,
  HI_ZHANGLAOCAI = 105,
  HI_ZHAOYINIANG = 106,
  HI_XIAOHESHANG = 107,
  HI_ZHENTAZHISHEN = 108,
  HI_ZHENYUANDAXIAN = 109,
  HI_ZHIZUNBAO = 110,
  HI_ZHONGKUI = 111,
  HI_CHANGE = 112,
  HI_YIZHANLAOBAN = 113,
  HI_QIAOFU = 114,
  HI_BIAOTOU = 115
};


class Hero{
public: 
  int ID_;
	int image_;
	int weaponImage_;
	char name[20]_;
	int mapID_;
	int mapX_;
	int mapY_;
	int mapAimX_;
	int mapAimY_;
	
	float hp_;
	float mingzhong_;
	float shanghai_;
	float fangyu_;
	float sudu_;
	float duobi_;
	float lingli_;


};

class Player:public Hero{
public:
  int ID_;
	int image_;
	int weaponImage_;
	char name[20];
	int mapID_;
	int mapX_;
	int mapY_;
	
	int renqi_;
	int menpai_;
	int menpaigongxian_;
	int money_;
	int bank_;
	float hp_;
	float hp2_;
	float hp3_;
	float mp_;
	float mp2_;
	int sp_;
	int huoli_;
	int huoli2_;
	int tili_;
	int tili2_;
	float mingzhong_;
	float shanghai_;
	float fangyu_;
	float sudu_;
	float duobi_;
	float lingli_;
	int tizhi_;
	int moli_;
	int liliang_;
	int naili_;
	int minjie_;
	int tizhi2_;//¼ÓµãÓÃ
	int moli2_;
	int liliang2_;
	int naili2_;
	int minjie2_;
	int tmptizhi_;//ÁÙÊ±
	int tmpmoli_;
	int tmpliliang_;
	int tmpnaili_;
	int tmpminjie_;
	float tmpmingzhong_;
	float tmpshanghai_;
	float tmpfangyu_;
	float tmpsudu_;
	float tmpduobi_;
	float tmplingli_;
	float tmphp_;
	float tmpmp_;
	int qianli_;
	long long xp_;
  
public:
  int NewPlayer(int ID,char* name,int image);
  void ChangePoint(int playerID, int tizhi, int moli, int liliang, int naili, int minjie);
  void Save();
  void LoadPlayer();
};


/*class Enemy : public Hero{

}*/


struct MapData {
  int map_image;
  int map_x_max;
  int map_y_max;
};

class GameLogic {
public:
  inline Coordinate GetPlayerCoordinate() const {
    return map_coordinate_;
  }
  inline void SetPlayerCoordinate( const Coordinate coordinate) {
    map_coordinate_ = coordinate;
  }
  inline int GetMapID() const {
    return map_id_;
  }
  inline void SetMapID( const int map_id) {
    map_id_ = map_id;
  }
  inline int GetPlayerDirection() const {
    return player_direction_;
  }
  inline void SetPlayerDirection( const int direction) {
    player_direction_ = direction;
  }
  void LoadMapIni();
  inline MapData GetMapData( const int map_id) const {
    return map_data_[map_id];
  }

  inline Coordinate GetMapAim() const {
    return map_coordinate_to_go_;
  }
  inline void SetMapAim( const Coordinate aim) {
    map_coordinate_to_go_ = aim;
  }

  void Walk();//人物行走，此处只更改人物坐标，画面中心坐标在GUIMAP中实现

private:
  int map_id_;
  Coordinate map_coordinate_;
  Coordinate map_coordinate_to_go_;//目标点坐标
  int player_direction_;
  MapData map_data_[kMaxMapCount];
};

extern GameLogic* g_game_logic;

#endif //_DREAM_GAMELOGIC