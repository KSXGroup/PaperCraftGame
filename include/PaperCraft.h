#ifndef _PAPERCRAFT_H_
#define _PAPERCRAFT_H_
#include "SDL2_header.h"
#include "pointd.h"
#include <map>
#include <ctime>
#include <cstdlib>
#include <cmath>

const std::string ROUNDPIC = "round.png";
const std::string PLAYERPIC = "paper.png";
const std::string ENEMYPIC = "enemy1.png";
const std::string BULLETPIC = "bullet2.png";
const std::string PROPLIFE = "life.png";
const std::string PROPENER = "energy.png";
const std::string PROPBOMB = "bomb.png";
const std::string SCORETITLE = "score.png";
const std::string NUMBERS = "numbers1.png";
const int SCR_W = Game::SCREEN_WIDTH - 350;
const int SCR_H = Game::SCREEN_HEIGHT;

class PlayerCraft;
class Bullet;
class Enemy;
class SignalRouter;
class Prop;

struct ObjId{
	static const int PLAYER = 0;
	static const int ENEMY = 1;
	static const int NORMALBULLET = 2;
	static const int BIGBULLET = 3;
	static const int LAYSER = 4;
	static const int PROP = 5;
};

struct GameState{
	static const int INIT = 0;
	static const int RUN = 1;
	static const int END = 2;
	static const int PAUSE = 3;
	static const int BOARD = 50;
};

struct BumpState{
	static const int BUMPED = 1;
	static const int NOTBUMPED = 0;
};

struct PlayerState{
	static const int SPEED = 8;
	static const int LIVE = 1;
	static const int DIE = 0;
	static const int PROTECTED = 2;
	static const double BUMPRANEG = 5;
};

struct EnemyState{
	static const int NORMALSPEED = 5;
	static const int BULLETSPEED = 5;
	static const int INIT = -1;
	static const int LIVE = 1;
	static const int DIE = 0;
	//static const int PROP = 2;
	static const int NORMAL = 3;
	static const int BIG = 4;
	static const int BOSS = 5;
	static const double BUMPRANGE = 10.3;
};

struct PropState{
	static const int ON = 10;
	static const int OFF = -10;
	static const int ADD_LIFE = 0;
	static const int ADD_ENER = 1;
	static const int ADD_BOMB = 2;
	static const double BUMPRANGE = 10;
};

struct BulletState{
	static const int BULLETSPEED = 2;
	static const int SHOOTED = 1;
	static const int UNSHOOTED = 0;
	static const double NORMALBUMPRNAGE = 5;
};

struct CirRange{
	PointD unitPos = PointD();
	double r = 0;
};


class SignalRouter{
	public:
		friend class PlayerCraft;
		friend class Enemy;
		friend class EnemyCraft;
		friend class Bullet;
		friend class Prop;
		SignalRouter();
		virtual ~SignalRouter();
		void init();
		void drawAll();
		//void readKbd(const int &key); TO BE DELETED;
		void dealAll();
		void moveAll();
		void dealBump();//TODO
		void reset();
		//void bombReset();
		//PointD windVelocity = PointD();
		std::map<int, bool> keyboard;
	private:
		int max = 10;
		int state = GameState::RUN;
		PlayerCraft *PC = nullptr;
		Enemy *EM = nullptr;
		Bullet *BLT = nullptr;
		Prop *PRP = nullptr;
};

class UserInteract{
	public:
		UserInteract(SignalRouter *sig):SR(sig){}
		virtual ~UserInteract();
		void drawHint(); //TODO
		void drawWelcome; //TODO
		void drawEnd(); //TODO
	private:
		SignalRouter *SR;
};

class BumpBox{
	public:
		friend class BumpBox;
		friend class SignalRouter;
		BumpBox(){
			BumpUnit = new CirRange[10];
		}
		virtual ~BumpBox(){
			delete [] BumpUnit;
			//cleanup(dot);
		}
		void init(const int type, const PointD &p, const int &PicW = 10, const int &PicH = 10);
		void posChange(const PointD &v){
			for(int i = 0; i < size ; ++i){
				BumpUnit[i].unitPos.x = BumpUnit[i].unitPos.x + v.x;
				BumpUnit[i].unitPos.y = BumpUnit[i].unitPos.y + v.y;
			}
		}
		void reset();
		void move(PointD v, const double sp);
		//void posSet(const PointD &p);
		void BumpDetect(BumpBox *target);
		inline int ifBumped();
	private:
		int status = BumpState::NOTBUMPED;
		int size = 0;
		//Image *dot = Game::loadImage("dot.png");
		CirRange *BumpUnit;
};
//TODO
class PaperObj{
	public:
		PaperObj() = default;
		virtual ~PaperObj();
		inline void velocityChange(const PointD &v);
		inline void velocitySet(const PointD &v);
		inline void posChange(const PointD &p);
		inline void posSet(const PointD &p);
		void speedSet(const double s);
		BumpBox *BPB = nullptr;
		double speed;
	protected:
		PointD velocity = PointD();
		PointD pos = PointD();
};

class PlayerCraft : public PaperObj{
	public:
		friend class SignalRouter;
		PlayerCraft(SignalRouter *sig) : PaperObj(-1, -1, -1){
			SR = sig;
		}
		virtual ~PlayerCraft();//remember to clean up and delete BPB!
		void init(const double init_posx,const double init_posy, const double _speed);
		void shoot();
		void move();
		inline void addScore(const int sc);
		void drawPlayer();
		void drawAttachments();
		void checkAndDeal();
		//void kbdRecv(const int &key);
		int state = PlayerState::LIVE;
	private:
		Image *PlayerPic = nullptr;
		Image *Protector = nullptr;
		Image *ScoreTitle = nullptr;
		Image *NumberPic = nullptr;
		SignalRouter *SR = nullptr;
		bool ifMove = true;
		int PlayerPicH = 0;
		int PlayerPicW = 0;
		int ProtectorH = 0;
		int ProtectorW = 0;
		int NumberW = 0;
		int NumberH = 0;
		int TitleH = 0;
		int TitleW = 0;
		int last_shoot = 10;
		int ProtectedTime = 240;
		int PicAlpha = 255;
		int life = 5;
		int energy = 100;
		int bomb = 0;
		int score = 0;

};

class EnemyCraft : public PaperObj{
	public:
		friend class SignalRouter;
		friend class Enemy;
		friend class PaperProp;
		EnemyCraft() : PaperObj(-1, -1, -1){
			BPB = new BumpBox;
		}
		virtual ~EnemyCraft();//remember to delete and clean!
		void init(const double init_posx, const double init_posy, const double _speed , const int type);
		void move();
		void shoot(const int strategy, const SignalRouter *SR);
		void drawEnemy(Image *img);
		void reset();
		int state = EnemyState::INIT;
		int status = EnemyState::NORMAL;
	private:
		int last_shoot = 0;
		int PicAlpha = 255;
		int life = 1;


};

class Enemy{
	public:
		friend class SignalRouter;
		Enemy(SignalRouter *sig):SR(sig){}
		void init(const int start_amount = 5,const int max_amount = 10);//LOAD PIC AND RANDOMIZE START POSITION
		void allocateNewEnemy();
		void moveAll();
		void velocityChangeAll(const PointD &v);
		void checkAndDeal();
		void drawAll();
		void shootAll();
		virtual ~Enemy();//remember to delete and clean!
	private:
		int current_enemy = 0;
		int max_enemy = 0;
		int last_allocate = 0;
		Image *EnemyPic = nullptr;
		EnemyCraft *enemies = nullptr; 
		SignalRouter *SR = nullptr;
};

class PaperBullet : public PaperObj{
	public:
		friend class Bullet;
		friend class SignalRouter;
		PaperBullet() : PaperObj(-1,-1,-1){}
		virtual ~PaperBullet();//remember to delete!
		void velocityChange(const PointD &v);
		void velocitySet(const PointD &v);
		void init(const PointD &p,const PointD &v, const double &sp);
		void move();
		void reset();
		void speedSet(const double s);
		void drawBullet(Image *img, const int w, const int h);
		BumpBox *BPB= nullptr;
	private:
		int state = BulletState::UNSHOOTED;
		int owner = 0;
};

class Bullet{
	public:
		friend class SignalRouter;
		Bullet(SignalRouter *sig){
			SR = sig;
		};
		virtual ~Bullet();//remember to delete and clean!
		void init(const int max_bullet,std::string bulletPicPath = BULLETPIC);//dont forget to load
		void checkAndDeal();
		void moveAll();
		void drawAll();
		int allocateNew(const PointD &p, const PointD &v, const double sp, const int ow);
		void reset();
	private:
		SignalRouter *SR = nullptr;
		PaperBullet *bullets = nullptr;
		int BulletW = 0;
		int BulletH = 0;
		int max_bullet = 0;
		int current_bullet = 0;
		Image *BulletPic[5];
};

class PaperProp : public PaperObj{
	public:
		friend class Prop;
		friend class SignalRouter;
		PaperProp(const EnemyCraft &wreck) : PaperObj(wreck.pos.x, wreck.pos.y, wreck.speed){
			velocity = wreck.velocity;
			BPB = new BumpBox;
		}
		PaperProp():PaperObj(-1, -1, -1){
			BPB = new BumpBox;
		}
		virtual ~PaperProp();
		void init(const int type, const PointD &p,const PointD &v, const double sp);
		void move();
		void reset();
		void drawProp(Image *img, const int w, const int h);
		BumpBox *BPB = nullptr;
	private:
		int state = PropState::OFF;
		int prop_type = -1;
};

class Prop{
	public:
		friend class SignalRouter;
		Prop(SignalRouter *sig){
			max_prop = max;
			props = new PaperProp[max];
			SR = sig;
		}
		virtual ~Prop();
		void init(const int max_em);
		void moveAll();
		void drawAll();
		void checkAndDeal();
		void randomAllocate();
		void allocateNewProp(const int type, const double posx, const double posy, const double sp, const PointD &v);
	private:
		SignalRouter *SR = nullptr;
		PaperProp *props = nullptr;
		Image *lifeProp = nullptr;
		Image *enerProp = nullptr;
		Image *bombProp = nullptr;
		int PropW = 0;
		int PropH = 0;
		int current_prop = 0;
		int max_prop = 0;
};

#endif
