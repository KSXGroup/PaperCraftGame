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
const std::string ENEMYPICNORMAL = "enemy1.png";
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
	//static const double BUMPRANEG = 5;
};

struct EnemyState{
	static const int NORMALSPEED = 5;
	static const int BULLETSPEED = 5;
	static const int INIT = -1;
	static const int LIVE = 1;
	static const int DIE = 0;
	//static const int PROP = 2;
	static const int NORMAL = 10;
	static const int BIG = 11;
	static const int BOSS = 12;
	//static const double BUMPRANGE = 10.3;
};

struct PropState{
	static const int ON = 10;
	static const int OFF = -10;
	static const int ADD_LIFE = 0;
	static const int ADD_ENER = 1;
	static const int ADD_BOMB = 2;
	//static const double BUMPRANGE = 10;
};
struct BulletState{
	static const int BULLETSPEED = 2;
	static const int SHOOTED = 1;
	static const int UNSHOOTED = 0;
	static const int NORMALSPEED = 2;
	//static const double NORMALBUMPRNAGE = 5;
	static const int HALFROUNDSHOOT = 10;
	static const int STRAIGHTSHOOT = 11;
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
		void drawWelcome(); //TODO
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
		void move(const PointD &v, const double sp);
		//void posSet(const PointD &p);
		void BumpDetect(BumpBox *target);
		inline int ifBumped(){
			return state;
		}
	private:
		int state = BumpState::NOTBUMPED;
		int size = 0;
		//Image *dot = Game::loadImage("dot.png");
		CirRange *BumpUnit;
};
//TODO
class PaperObj{
	public:
		PaperObj();
		virtual ~PaperObj();
		inline void velocityChange(const PointD &v);
		inline void velocitySet(const PointD &v);
		inline void posChange(const PointD &p);
		inline void speedSet(const double s);
		virtual void move();
		BumpBox *BPB = nullptr;
		double speed;
	protected:
		PointD velocity = PointD();
		PointD pos = PointD();
};

class PlayerCraft : public PaperObj{
	public:
		friend class SignalRouter;
		PlayerCraft(SignalRouter *sig) : PaperObj(){
			SR = sig;
		}
		virtual ~PlayerCraft();//remember to clean up and delete BPB!
		void init(const PointD &p, const double _speed);
		void move() override;
		void shoot();
		void drawPlayer();
		void drawAttachments(); //TODO
		void checkAndDeal();
		inline void addScore(const int sc){
			score += sc;
		}
		inline void addLife(const int lf){
			life += lf;
		}
		inline void addEnergy(const int en){
			energy += en;
		}
		inline void addBomb(const int bo){
			bomb += bo;
		}
		//void kbdRecv(const int &key);
		int state = PlayerState::LIVE;
	private:
		Image *PlayerPic = nullptr;
		Image *Protector = nullptr;
		SignalRouter *SR = nullptr;
		bool ifMove = true;
		int PlayerPicH = 0;
		int PlayerPicW = 0;
		int ProtectorH = 0;
		int ProtectorW = 0;
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
		EnemyCraft() : PaperObj(){}
		virtual ~EnemyCraft() = default;//remember to delete and clean!
		void init(const PointD &p, const double _speed , const int type);
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
		Image *EnemyPicNormal = nullptr;
		Image *EnemyPicBig = nullptr;//TODO
		Image *EnmeyPicBoss = nullptr;//TODO
		EnemyCraft *enemies = nullptr; 
		SignalRouter *SR = nullptr;
};

class PaperBullet : public PaperObj{
	public:
		friend class Bullet;
		friend class SignalRouter;
		PaperBullet() : PaperObj(){}
		virtual ~PaperBullet() = default;
		void init(const PointD &p,const PointD &v, const double sp);
		void reset();
		void drawBullet(Image *img);
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
		void init(const int mb,std::string bulletPicPath = BULLETPIC);//dont forget to load
		void checkAndDeal();
		void moveAll();
		void drawAll();
		int allocateNewBullet(const PointD &p, const PointD &v, const double sp, const int ow);
		void reset();
	private:
		SignalRouter *SR = nullptr;
		PaperBullet *bullets = nullptr;
		int BulletW = 0;
		int BulletH = 0;
		int max_bullet = 0;
		int current_bullet = 0;
		Image *BulletPic[5];//TODO
};

class PaperProp : public PaperObj{
	public:
		friend class Prop;
		friend class SignalRouter;
		PaperProp(const EnemyCraft &wreck) : PaperObj(){
			velocity = wreck.velocity;
			pos.x = wreck.pos.x;
			pos.y = wreck.pos.y;
			speed = wreck.speed;
		}
		PaperProp():PaperObj(){}
		virtual ~PaperProp() = default;
		void init(const int type, const PointD &p,const PointD &v, const double sp);
		void reset();
		void drawProp(Image *img);
	private:
		int state = PropState::OFF;
		int prop_type = -1;
};

class Prop{
	public:
		friend class SignalRouter;
		Prop(SignalRouter *sig){
			SR = sig;
		}
		virtual ~Prop();
		void init(const int max_em);
		void moveAll();
		void drawAll();
		void checkAndDeal();
		void randomAllocate();
		void allocateNewProp(const int type, const PointD &p, const PointD &v,const double sp);
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
