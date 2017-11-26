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
const std::string ENEMYPICBOSS = "boss.png";
const std::string BULLETPIC = "bullet2.png";
const std::string PROPLIFE = "life.png";
const std::string PROPENER = "energy.png";
const std::string PROPBOMB = "bomb.png";
const std::string SCORETITLE = "score.png";
const std::string BOMBSAM = "bombsam1.png";
const std::string CRAFTSAM = "craftsam.png";
const std::string NUMBERS = "numbers.png";
const std::string MULTIPLY = "multiply.png";
const std::string LOSE = "lose.png";
const std::string WIN = "win.png";
const std::string YOUWIN = "youwin.png";
const std::string YOULOSE = "youlose.png";
const std::string FINAL = "final.png";
const std::string LOGO = "logo.png";
const std::string START1 = "start1.png";
const std::string START2 = "start.png";
const int SCR_W = Game::SCREEN_WIDTH - 350;
const int SCR_H = Game::SCREEN_HEIGHT;

class PlayerCraft;
class Bullet;
class Enemy;
class SignalRouter;
class Prop;
class UserInteract;

struct ObjId{
	static const int PLAYER = 0;
	static const int ENEMY = 1;
	static const int BOSS = 10086;
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
	static const int BOSS = 4;
	static const int WIN = 5;
	static const int LOSE = 6;
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
	static const int REVERSEHALFROUNDSHOOT = 9;
	static const int HALFROUNDSHOOT = 10;
	static const int STRAIGHTSHOOT = 11;
	static const int TRACESHOOT = 12;
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
		friend class UserInteract;
		SignalRouter();
		virtual ~SignalRouter();
		void init();
		void drawAll();
		//void readKbd(const int &key); TO BE DELETED;
		void dealAll();
		void moveAll();
		void dealBump();//TODO
		void reset();
		void bombReset();
		//PointD windVelocity = PointD();
		std::map<int, bool> keyboard;
		double mX = 0;
		double mY = 0;
		bool MC = false;
		int state = GameState::INIT;
	private:
		int max = 10;
		PlayerCraft *PC = nullptr;
		Enemy *EM = nullptr;
		Bullet *BLT = nullptr;
		Prop *PRP = nullptr;
		UserInteract *UI = nullptr;
};

class UserInteract{
	public:
		UserInteract(SignalRouter *sig):SR(sig){}
		virtual ~UserInteract();
		void init();
		void drawHint(); 
		void drawWelcome(); //TODO
		void drawEnd(int res); 
		void checkAndDeal();
	private:
		SignalRouter *SR;
		int scro = 0;
		bool f = 0;
		Image *ScoreTitle = nullptr;
		int ScoreTitleH = 0, ScoreTitleW = 0;
		Image *BombSam = nullptr;
		int BombSamH = 0, BombSamW = 0;
		Image *CraftSam = nullptr;
		int CraftSamH = 0, CraftSamW = 0;
		Image *NumberPic = nullptr;
		int NumberPicH = 0, NumberPicW = 0;
		Image *Multiply = nullptr;
		int MultiplyH = 0, MultiplyW = 0;
		Image *Lose = nullptr;
		int LoseH = 0, LoseW = 0;
		Image *Win = nullptr;
		int WinH = 0, WinW = 0;
		Image *YouWin = nullptr;
		int YouWinW = 0, YouWinH = 0;
		Image *YouLose = nullptr;
		int YouLoseW = 0, YouLoseH = 0;
		Image *Final = nullptr;
		int FinalW = 0, FinalH = 0;
		Image *Logo = nullptr;
		int LogoW = 0, LogoH = 0;
		Image *Start1 = nullptr;
		int Start1W = 0, Start1H = 0;
		Image *Start2 = nullptr;
		int Start2W = 0, Start2H = 0;
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
		inline PointD getPos() const;
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
		friend class UserInteract;
		PlayerCraft(SignalRouter *sig) : PaperObj(){
			SR = sig;
		}
		virtual ~PlayerCraft();//remember to clean up and delete BPB!
		void init(const PointD &p, const double _speed);
		void move() override;
		void shoot();
		void drawPlayer();
		//void drawAttachments(); //TODO
		void checkAndDeal();
		inline PointD getPos();
		inline void addScore(const int sc){
			score += sc;
		}
		inline void addLife(const int lf){
			life += lf;
			if(life > 99){
				life = 99;
			}
		}
		inline void addEnergy(const int en){
			energy += en;
			if(energy > 100){
				energy = 100;
			}
		}
		inline void addBomb(const int bo){
			bomb += bo;
			if(bomb > 99){
				bomb = 99;
			}
		}
		//void kbdRecv(const int &key);
		int state = PlayerState::LIVE;
	private:
		Image *PlayerPic = nullptr;
		Image *Protector = nullptr;
		SignalRouter *SR = nullptr;
		bool ifMove = true;
		int last_bomb = 0;
		int PlayerPicH = 0;
		int PlayerPicW = 0;
		int ProtectorH = 0;
		int ProtectorW = 0;
		int last_shoot = 10;
		int ProtectedTime = 240;
		int ProtectedCount = 0;
		int PicAlpha = 255;
		int life = 99;
		int energy = 100;
		int bomb = 2;
		int score = 0;

};

class EnemyCraft : public PaperObj{
	public:
		friend class SignalRouter;
		friend class Enemy;
		friend class Prop;
		EnemyCraft() : PaperObj(){}
		virtual ~EnemyCraft() = default;//remember to delete and clean!
		void init(const PointD &p, const double _speed , const int type);
		void shoot(const int strategy, const SignalRouter *SR,const PointD &centre, const PointD &drct);
		void drawEnemy(Image *img);
		void reset();
		int state = EnemyState::INIT;
		int status = EnemyState::NORMAL;
	private:
		int last_shoot = 0;
		int shoot_type = 0;
		int PicAlpha = 255;
		int life = 1;


};

class Enemy{
	public:
		friend class SignalRouter;
		Enemy(SignalRouter *sig):SR(sig){}
		void init(const int start_amount = 5,const int max_amount = 10);//LOAD PIC AND RANDOMIZE START POSITION
		void allocateNewEnemy();
		void allocateBoss();
		void moveAll();
		void velocityChangeAll(const PointD &v);
		void checkAndDeal();
		void drawAll();
		void shootAll();
		void reset();
		virtual ~Enemy();//remember to delete and clean!
	private:
		int current_enemy = 0;
		int max_enemy = 0;
		int last_allocate = 0;
		Image *EnemyPicNormal = nullptr;
		Image *EnemyPicBig = nullptr;//TODO
		Image *EnemyPicBoss = nullptr;//TODO
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
		/*PaperProp(const EnemyCraft &wreck) : PaperObj(){
			std::cerr << "startcopy" << std::endl;
			velocity = wreck.velocity;
			pos.x = wreck.pos.x;
			pos.y = wreck.pos.y;
			speed = wreck.speed;
			BPB -> init(ObjId::PROP, wreck.pos);
			std::cerr << "endcopy" << std::endl;
		}*/
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
		void allocateNewProp(const EnemyCraft &wreck, const int t);
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
