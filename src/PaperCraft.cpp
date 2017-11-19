#include "PaperCraft.h"
using namespace Game;
SignalRouter::SignalRouter(){
	std::cin >> max;
	PC = new PlayerCraft(this);
	EM = new Enemy(this);
	BLT = new Bullet(this);
	PRP = new Prop(this);
	UI = new UserInteract(this);
}

SignalRouter::~SignalRouter(){
	state = GameState::END;
	delete PC;
	delete EM;
	delete BLT;
	delete PRP;
	delete UI;
}

void SignalRouter::init(){
	srand(time(NULL));
	PC  -> init( PointD(SCR_W / 2 - PC -> PlayerPicW / 2,SCR_H - PC -> PlayerPicH / 2) , 10);
	EM  -> init(max / 2, max);
	BLT -> init(20 * max);
	PRP -> init(max);
	UI  -> init();
}

void SignalRouter::drawAll(){
	if(state == GameState::INIT){
		UI -> drawWelcome();
	}
	if(state == GameState::RUN){
		PC  -> drawPlayer();
		EM  -> drawAll();
		BLT -> drawAll();
		PRP -> drawAll();
		setPenColor((Color){200, 200, 200, 255});
		drawRect(Rect{674,0,350,768},1);
		UI -> drawHint();
	}
	if(state == GameState::BOSS){
		PC  -> drawPlayer();
		EM  -> drawAll();
		BLT -> drawAll();
		setPenColor((Color){200, 200, 200, 255});
		drawRect(Rect{674,0,350,768},1);
		UI -> drawHint();
	}
	if(state == GameState::WIN || state == GameState::LOSE ){
		PC  -> drawPlayer();
		BLT -> drawAll();
		setPenColor((Color){200, 200, 200, 255});
		drawRect(Rect{674,0,350,768},1);
		UI -> drawHint();
		UI -> drawEnd(state);
	}
}

void SignalRouter::dealAll(){
	if(PC -> score >= 50 && state == GameState::RUN){
		state = GameState::BOSS;
		EM -> allocateBoss();
	}
	if(state == GameState::INIT){
		UI -> checkAndDeal();
	}
	if(state == GameState::RUN){
		EM  -> allocateNewEnemy();
		PRP -> randomAllocate();
		EM  -> shootAll();
		PC  -> shoot();
		dealBump();
		PC  -> checkAndDeal();
		EM  -> checkAndDeal();
		BLT -> checkAndDeal();
		PRP -> checkAndDeal();
	}
	if(state == GameState::BOSS){
		EM  -> shootAll();
		PC  -> shoot();
		dealBump();
		PC  -> checkAndDeal();
		EM  -> checkAndDeal();
		BLT -> checkAndDeal();
	}
	if(state == GameState::WIN ||state == GameState::LOSE ){
		PC  -> shoot();
		PC  -> checkAndDeal();
		BLT -> checkAndDeal();
	}
}

void SignalRouter::moveAll(){
	if(state == GameState::RUN){
		PC  -> move();
		EM  -> moveAll();
		BLT -> moveAll();
		PRP -> moveAll();
	}
	if(state == GameState::BOSS){
		PC  -> move();
		EM  -> moveAll();
		BLT -> moveAll();
	}
	if(state == GameState::WIN ||state == GameState::LOSE ){
		PC  -> move();
		BLT -> moveAll();
	}
	
}

void SignalRouter::dealBump(){
	if(PC -> state == PlayerState::LIVE){
	//PLAYER WITH ENEMY
		for(int i = 0; i < EM -> max_enemy; ++i){
			if(EM -> enemies[i].state == EnemyState::LIVE){
				EM -> enemies[i].BPB -> BumpDetect(PC -> BPB);
				if(PC -> BPB -> ifBumped() == BumpState::BUMPED){
					(PC -> life)--;
					PC -> BPB -> state = BumpState::NOTBUMPED;
					PC -> ProtectedTime = 240;
					PC -> state = PlayerState::PROTECTED;
					(EM -> enemies[i].life)--;
					EM -> enemies[i].BPB -> state = BumpState::NOTBUMPED;
					break;
				}
			}
		}
	//PLAYER WITH BULLET
		for(int i = 0; i < BLT -> max_bullet; ++i){
			if(BLT -> bullets[i].owner == ObjId::ENEMY && BLT -> bullets[i].state == BulletState::SHOOTED){
				BLT -> bullets[i].BPB -> BumpDetect(PC -> BPB);
				if(PC -> BPB -> ifBumped() == BumpState::BUMPED){
					(PC -> life)--;
					PC -> BPB -> state = BumpState::NOTBUMPED;
					PC -> ProtectedTime = 240;
					PC -> state = PlayerState::PROTECTED;
					break;
				}
			}
		}
	}
	
	//ENEMY WITH BULLET
	for(int i = 0; i < EM -> max_enemy; ++i){
		if(EM -> enemies[i].state == EnemyState::LIVE){
				for(int j = 0; j < BLT -> max_bullet; ++j){
					if(BLT -> bullets[j].owner != ObjId::ENEMY && BLT -> bullets[j].state == BulletState::SHOOTED){
						EM -> enemies[i].BPB -> BumpDetect(BLT -> bullets[j].BPB);
						if(EM -> enemies[i].BPB -> ifBumped() == BumpState::BUMPED){
							(EM -> enemies[i].life)--;
							EM -> enemies[i].BPB -> state = BumpState::NOTBUMPED;
						}
					}
				}
		}
	}
	//PLAYER WITH PROP
	for(int i = 0; i < PRP -> max_prop; ++i){
		if(PRP -> props[i].state == PropState::ON && PC -> state != PlayerState::DIE){
			PC -> BPB -> BumpDetect(PRP -> props[i].BPB);
			PC -> BPB -> state = BumpState::NOTBUMPED;
		}
	}

}

void SignalRouter::bombReset(){
	EM -> reset();
	PC -> ProtectedTime += 960;
	PC -> state = PlayerState::PROTECTED;
}

void SignalRouter::reset(){
	//TODO
}


UserInteract::~UserInteract(){
	cleanup(ScoreTitle);
	cleanup(BombSam);
	cleanup(CraftSam);
	cleanup(NumberPic);
	cleanup(Multiply);
	cleanup(Lose);
	cleanup(Win);
	cleanup(YouWin);
	cleanup(YouLose);
	cleanup(Final);
	cleanup(Logo);
	cleanup(Start1);
	cleanup(Start2);
}

void UserInteract::init(){
	ScoreTitle = loadImage(SCORETITLE);
	BombSam = loadImage(BOMBSAM);
	CraftSam = loadImage(CRAFTSAM);
	NumberPic = loadImage(NUMBERS);
	Multiply = loadImage(MULTIPLY);
	Lose = loadImage(LOSE);
	Win = loadImage(WIN);
	YouWin = loadImage(YOUWIN);
	YouLose = loadImage(YOULOSE);
	Final = loadImage(FINAL);
	Logo = loadImage(LOGO);
	Start1 = loadImage(START1);
	Start2 = loadImage(START2);
	getImageSize(ScoreTitle, ScoreTitleW, ScoreTitleH);
	getImageSize(CraftSam, CraftSamW, CraftSamH);
	getImageSize(BombSam, BombSamW, BombSamH);
	getImageSize(NumberPic, NumberPicW ,NumberPicH);
	getImageSize(Multiply, MultiplyW, MultiplyH);
	getImageSize(Lose, LoseW, LoseH);
	getImageSize(Win, WinW, WinH);
	getImageSize(YouWin, YouWinW, YouWinH);
	getImageSize(YouLose, YouLoseW, YouLoseH);
	getImageSize(Final, FinalW, FinalH);
	getImageSize(Logo, LogoW, LogoH);
	getImageSize(Start1, Start1W, Start1H);
	getImageSize(Start2, Start2W, Start2H);
}

void UserInteract::drawWelcome(){
	drawImage(Logo, SCREEN_WIDTH / 2 - LogoW / 2, SCREEN_HEIGHT / 3.5 - LogoH / 2);
	drawImage(CraftSam, SCREEN_WIDTH / 2 - CraftSamW / 2, SCREEN_HEIGHT / 3.5 - CraftSamH / 2);
	if(!f){
		drawImage(Start1, SCREEN_WIDTH / 2 - Start1W / 2, SCREEN_HEIGHT / 2 - Start1H / 2);
	}
	else{
		drawImage(Start2, SCREEN_WIDTH / 2 - Start2W / 2, SCREEN_HEIGHT / 2 - Start2H / 2);
	}
}

void UserInteract::drawHint(){
	int score = SR -> PC -> score;
	int life = SR -> PC -> life;
	int energy = SR -> PC -> energy;
	int bomb = SR -> PC -> bomb;
	int p = 1;
	Rect clip;
	drawImage(ScoreTitle, 800 - ScoreTitleW / 2, 80 - ScoreTitleH / 2);
	//std::cout << "ADD POINT:" <<score << std::endl;
	for(int i = 1; i <= 6; ++i){
		clip = Rect{(score % (10 * p)) / p * NumberPicW / 10,0, NumberPicW / 10, NumberPicH };
		drawImage(NumberPic, 784 + 6 * NumberPicW / 10 - (i + 1) * NumberPicW / 10 , 150 - NumberPicH / 2, 1, 1, 0, NULL, FLIP_NONE, &clip);
		p *= 10;
	}
	p = 1;
	drawImage(CraftSam, 770 - CraftSamW / 2, 270 - CraftSamH / 2);
	drawImage(Multiply, 770 + 90 - MultiplyW / 2, 270 - MultiplyH / 2);
	for(int i = 1; i <= 2; ++i){
		clip = Rect{(life % (10 * p)) / p * NumberPicW / 10,0, NumberPicW / 10, NumberPicH };
		drawImage(NumberPic, 800 + 6 * NumberPicW / 10 - (i + 1) * NumberPicW / 10 , 270 - NumberPicH / 2, 1,1, 0, NULL, FLIP_NONE, &clip);
		p *= 10;
	}
	p = 1;
	drawImage(BombSam, 770 - BombSamW / 2, 380 - BombSamH / 2);
	drawImage(Multiply, 770 + 90 - MultiplyW / 2, 380 - MultiplyH / 2);
	for(int i = 1; i <= 2; ++i){
		clip = Rect{(bomb % (10 * p)) / p * NumberPicW / 10,0, NumberPicW / 10, NumberPicH };
		drawImage(NumberPic, 800 + 6 * NumberPicW / 10 - (i + 1) * NumberPicW / 10 , 380 - NumberPicH / 2, 1,1, 0, NULL, FLIP_NONE, &clip);
		p *= 10;
	}
}

void UserInteract::drawEnd(const int res){
	Rect clip;
	int p = 1;
	setPenColor(255,255,255,220);
	drawRect(Rect{0,0,1027,768}, 1);
	if(res == GameState::WIN){
		if(scro  < SR -> PC -> score + SR -> PC -> life * 100 + SR -> PC -> bomb * 100){
			scro += 100;
			if(scro > SR -> PC -> score + SR -> PC -> life * 100 + SR -> PC -> bomb * 100){
				scro = SR -> PC -> score + SR -> PC -> life * 100 + SR -> PC -> bomb * 100;
			}
		}
		drawImage(Win, SCREEN_WIDTH / 2 - WinW / 2, SCREEN_HEIGHT / 4 - WinH / 2);
		drawImage(YouWin, SCREEN_WIDTH / 2 - YouWinW / 2, SCREEN_HEIGHT / 2  - YouWinH / 2);
		drawImage(Final, SCREEN_WIDTH / 4  - FinalW / 2, SCREEN_HEIGHT / 1.4 - FinalH / 2);
		for(int i = 1; i <= 6; ++i){
		clip = Rect{(scro % (10 * p)) / p * NumberPicW / 10,0, NumberPicW / 10, NumberPicH };
		drawImage(NumberPic, SCREEN_WIDTH / 1.7 + 6 * NumberPicW / 10 - (i + 1) * NumberPicW / 10 , SCREEN_HEIGHT / 1.39 - NumberPicH / 2, 1, 1, 0, NULL, FLIP_NONE, &clip);
		p *= 10;
	}
	}
	else{
		drawImage(Lose, SCREEN_WIDTH /2 - LoseW / 2, SCREEN_HEIGHT / 4 -LoseH / 2);
		drawImage(YouLose, SCREEN_WIDTH / 2 - YouLoseW / 2, SCREEN_HEIGHT / 2 - YouLoseH / 2);
	}
}

void UserInteract::checkAndDeal(){
	if(SR -> state == GameState::INIT){
		//std::cout << "CKD" << std::endl;
		if(SR -> mX >= (SCREEN_WIDTH / 2 - Start1W / 2) && SR -> mX <= (SCREEN_WIDTH / 2 + Start1W / 2) && SR -> mY >= (SCREEN_HEIGHT / 2 - Start1H / 2) && SR -> mY <= (SCREEN_HEIGHT / 2 + Start1H / 2) ){
			std::cerr << SR -> mX <<","<<SR -> mY << std::endl;
			f = 1;
			if(SR -> MC == 1){
				SR -> state = GameState::RUN;
			}
		}
		else{
			f = 0;
		}
	}
}

void BumpBox::init(const int type, const PointD &p, const int &PicW, const int &PicH){
	switch(type){
		case ObjId::PLAYER:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = 8;
			break;
		case ObjId::ENEMY:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = 10.3;
			break;
		case ObjId::NORMALBULLET:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = 5;
			break;
		case ObjId::PROP:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = 10;
			break;
		case ObjId::BOSS:
			size = 5;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = 45;
			BumpUnit[1].unitPos = p - PointD(90, -10);
			BumpUnit[1].r = 10;
			BumpUnit[2].unitPos = p - PointD(50,0);
			BumpUnit[2].r = 5;
			BumpUnit[3].unitPos = p + PointD(70, 15);
			BumpUnit[3].r = 10;
			BumpUnit[4].unitPos = p + PointD(50, -45);
			BumpUnit[2].r = 5;
			break;
	}
}


void BumpBox::move(const PointD &v, const double sp){
	//drawImage(dot, BumpUnit[0].unitPos.x - 7.5, BumpUnit[0].unitPos.y - 7.5);
	PointD tmp = v;
	double vlen = tmp.length();
	if(vlen > sp){
		tmp = tmp / vlen * sp;
	}
	for(int i = 0;i < size; ++i){
		BumpUnit[i].unitPos = BumpUnit[i].unitPos + tmp;
	}
}

void BumpBox::reset(){
	for(int i = 0;i < size; ++i){
		BumpUnit[i].unitPos = PointD();
		BumpUnit[i].r = -1;
	}
	state = BumpState::NOTBUMPED;
}


void BumpBox::BumpDetect(BumpBox *target){
	double dx = 0, dy = 0, dis = 0;
	for(int i = 0;i < size;++i){
		for(int j = 0; j < target -> size; ++j){
			dx = BumpUnit[i].unitPos.x - target -> BumpUnit[j].unitPos.x;
			dy = BumpUnit[i].unitPos.y - target -> BumpUnit[j].unitPos.y;
			dis = sqrt(dx * dx + dy * dy);
			if(dis <= (BumpUnit[i].r + target -> BumpUnit[j].r)){
				state = BumpState::BUMPED;
				target -> state = BumpState::BUMPED;
				break;
			}
		}
	}
}


PaperObj::PaperObj(){
	BPB = new BumpBox;
}

PaperObj::~PaperObj(){
	delete BPB;
}

inline void PaperObj::velocityChange(const PointD &v){
	velocity = velocity + v;
}
inline void PaperObj::velocitySet(const PointD &v){
	velocity = v;
}

inline void PaperObj::posChange(const PointD &p){
	pos = pos + p;
	BPB -> posChange(p);
}

inline void PaperObj::speedSet(const double s){
	speed = s;
}

void PaperObj::move(){
	double vlen = velocity.length();
	velocity = velocity / vlen * speed;
	pos = pos + velocity;
	BPB -> move(velocity, speed);
}

PlayerCraft::~PlayerCraft(){
	cleanup(PlayerPic);
	cleanup(Protector);
}

inline PointD PlayerCraft::getPos(){
	return pos;
}

void PlayerCraft::init(const PointD &p, const double _speed){
	pos = p;
	speed = _speed;
	last_shoot = 50;
	PlayerPic = loadImage(PLAYERPIC);
	Protector = loadImage(ROUNDPIC);
	getImageSize(PlayerPic, PlayerPicW, PlayerPicH);
	getImageSize(Protector, ProtectorW, ProtectorH);
	BPB -> init(ObjId::PLAYER, pos, PlayerPicW, PlayerPicH);
	state = PlayerState::PROTECTED;
}

void PlayerCraft::move(){
	ifMove = false;
	if(SR -> keyboard[KEY_UP]){
		if(pos.y <= PlayerPicH / 2){
			return;
		}
		velocity = velocity + PointD(0,-1)*speed;
		ifMove = true;
	}
	if(SR -> keyboard[KEY_DOWN]){
		if(pos.y >= SCR_H - PlayerPicH / 2){
			return;
		}
		velocity = velocity + PointD(0,+1)*speed;
		ifMove = true;
	}
	if(SR -> keyboard[KEY_LEFT]){
		if(pos.x <= PlayerPicW / 2){
			return;
		}
		velocity = velocity + PointD(-1,0)*speed;
		ifMove = true;
	}
	if(SR -> keyboard[KEY_RIGHT]){
		if(pos.x >= SCR_W - PlayerPicW / 2){
			return;
		}
		velocity = velocity + PointD(+1,0)*speed;
		ifMove = true;
	}

	//Limit player's speed
	double len = velocity.length();
	if( len > speed ){
		velocity = velocity / len *speed;
	}
	if(!ifMove){
		velocity = PointD();
	}
	//Calculate new position
	pos = pos + velocity;
	BPB -> move(velocity, speed);
}

void PlayerCraft::shoot(){
	if(SR -> keyboard[KEY_SPACE] && last_shoot >= 5){
		SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y - 20), PointD(0, -1), speed + BulletState::NORMALSPEED, ObjId::PLAYER);
		last_shoot = 0;
		return;
	}
	last_shoot++;
}

void PlayerCraft::drawPlayer(){
	if(state != PlayerState::DIE){
		drawImage(PlayerPic, pos.x - PlayerPicW / 2, pos.y - PlayerPicH / 2);
		if(state == PlayerState::PROTECTED){
			drawImage(Protector,pos.x - ProtectorW / 2, pos.y - ProtectorH / 2, 1, 1, ProtectedCount % 181);
		}
	}
}

//void PlayerCraft::drawAttachments(){
	//TODO
//}

void PlayerCraft::checkAndDeal(){
	if(ProtectedTime > 0){
		++ProtectedCount;
		if(ProtectedCount == ProtectedTime){
			ProtectedTime = 0;
			ProtectedCount = 0;
			state = PlayerState::LIVE;
		}
	}
	if(life <= 0){
		SR -> state = GameState::LOSE;
	}
	if(SR -> keyboard['b'] && bomb > 0 && SR -> state != GameState::BOSS){
		SR -> bombReset();
		bomb--;
	}
}

void EnemyCraft::init(const PointD &p, const double _speed, const int type){
	velocity = PointD(0, 1);
	pos = p;
	speed = _speed;
	if(type == EnemyState::NORMAL){
		life = 1;
		last_shoot = 200;
		status = type;
		BPB -> init(ObjId::ENEMY, p, speed);
	}
	else if(type == EnemyState::BIG){
		//TODO
	}
	else if(type == EnemyState::BOSS){
		life = 150;
		last_shoot = 200;
		status = type;
		BPB -> init(ObjId::BOSS, p, speed);
	}
}
void EnemyCraft::shoot(const int strategy, const SignalRouter *SR, const PointD &centre, const PointD &drct = PointD()){
	if(strategy == BulletState::HALFROUNDSHOOT){
		if(last_shoot < 150){
			last_shoot++;
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-sqrt(3), 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-1 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-1 , sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-1 , 2 + sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(0 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(1 , 2 + sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(1 , sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(1 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(sqrt(3) , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			last_shoot = 0;
		}
	}
	if(strategy == BulletState::REVERSEHALFROUNDSHOOT){
		if(last_shoot < 150){
			last_shoot++;
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-sqrt(3), -1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-1 , -1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-1 , -sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(-1 , -2 - sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(0 , -1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(1 , -2 - sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(1 , -sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(1 , -1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(sqrt(3) , -1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			last_shoot = 0;
		}
	}
	if(strategy == BulletState::STRAIGHTSHOOT){
		if(last_shoot < 20){
			last_shoot++;
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(centre.x, centre.y + 20), PointD(0, 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			last_shoot = 0;
		}
	}
	if(strategy == BulletState::TRACESHOOT){
		if(last_shoot < 50){
			last_shoot++;
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(centre,drct - centre, speed + BulletState::NORMALSPEED, ObjId::ENEMY);
		}
	}
}

void EnemyCraft::drawEnemy(Image *img){
	int w = 0, h = 0;
	getImageSize(img, w, h);
	drawImage(img, pos.x - w / 2, pos.y - h / 2, 1, 1, 180);
}

void EnemyCraft::reset(){
	life = 0;
	last_shoot = 200;
	pos = PointD();
	velocity  = PointD();
	BPB -> reset();
	state = EnemyState::INIT;
	status = -1;
}

Enemy::~Enemy(){
	delete [] enemies;
}

void Enemy::init(const int start_amount, const int max_amount){
	current_enemy = start_amount;
	max_enemy = max_amount;
	EnemyPicNormal = loadImage(ENEMYPICNORMAL);
	EnemyPicBoss = 	loadImage(ENEMYPICBOSS);
	enemies = new EnemyCraft[max_amount];
	for(int i = 0; i < start_amount; ++i){
		enemies[i].init(PointD(rand() % (SCR_W - 40) + 20, 20), rand() % EnemyState::NORMALSPEED + 2, EnemyState::NORMAL);
		enemies[i].state = EnemyState::LIVE;
	}
	for(int i = start_amount;i < max_amount; ++i){
		enemies[i].init(PointD(), 0, EnemyState::NORMAL);
	}
}

void Enemy::allocateNewEnemy(){
	int to_allocate = 0, count = 0;
	last_allocate++;
	if(last_allocate < 200 || current_enemy  == max_enemy){
		return;
	}
	else{
		srand(time(NULL));
		to_allocate = rand() % (max_enemy - current_enemy / 2) + 2;
		current_enemy += to_allocate;
		for(int i = 0; i < max_enemy; ++i){
			if(enemies[i].state == EnemyState::INIT){
				enemies[i].init(PointD(rand() % (SCR_W - 40) + 20, 20), rand() % EnemyState::NORMALSPEED + 2, EnemyState::NORMAL);
				enemies[i].state = EnemyState::LIVE;
				count++;
			}
			if(count == to_allocate){
				break;
			}
		}
		last_allocate = 0;
	}
		//std::cout << current_enemy << std::endl;
}

void Enemy::allocateBoss(){
	int count = 0;
	for(int i = 0; i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::LIVE){
			enemies[i].speedSet(20);
			count++;
			if(count == current_enemy){
				break;
			}
		}
	}
	for(int i = 0; i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::INIT){
			enemies[i].state = EnemyState::LIVE;
			enemies[i].init(PointD(SCR_W / 2, 0), 1 ,EnemyState::BOSS);
		}
	}
	
}

void Enemy::moveAll(){
	int count = 0;
	for(int i = 0;i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::LIVE){
			if(enemies[i].status != EnemyState::BOSS){
				enemies[i].move();
			}
			else{
				if(enemies[i].pos.y >= SCR_H / 4){
					continue;
				}
				else{
					enemies[i].move();
				}
			}
		}
	}
}

void Enemy::velocityChangeAll(const PointD &v){
	for(int i = 0;i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::LIVE){
			enemies[i].velocityChange(v);
		}
	}
}

void Enemy::checkAndDeal(){
	int tmp = 0;
	for(int i = 0; i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::LIVE){
			if(enemies[i].status == EnemyState::BOSS){
				std::cerr << enemies[i].life << std::endl;
			}
			if(enemies[i].life <= 0){
				tmp = rand() / 1000000;
				if(tmp < 500){
					if(tmp > 100){
						SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_ENER);
					}
					else if(tmp > 10){
						SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_LIFE);
					}
					else{
						SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_BOMB);
					}
				}
				if(enemies[i].status == EnemyState::NORMAL){
					SR -> PC -> addScore(1);
				}
				else if(enemies[i].status == EnemyState::BIG){
					SR -> PC -> addScore(5);
				}
				else if(enemies[i].status == EnemyState::BOSS){
					SR -> state = GameState::WIN;
					/*SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_ENER);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_ENER);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_ENER);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_LIFE);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_LIFE);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_LIFE);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_BOMB);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_BOMB);
					SR -> PRP -> allocateNewProp(enemies[i], PropState::ADD_BOMB);*/
					SR -> PC -> addScore(500);
				}
				enemies[i].reset();
				current_enemy--;
			}
			if(enemies[i].pos.y >= SCR_H + 20){
				enemies[i].reset();
				current_enemy--;
			}
		}
	}
}

void Enemy::drawAll(){
	int count = 0;
	for(int i = 0;i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::LIVE){
			if(enemies[i].status == EnemyState::NORMAL){
				enemies[i].drawEnemy(EnemyPicNormal);
			}
			else if(enemies[i].status == EnemyState::BIG){
				//TODO
			}
			else if(enemies[i].status == EnemyState::BOSS){
				enemies[i].drawEnemy(EnemyPicBoss);
			}
			count++;
			if(count == current_enemy){
				break;
			}
		}
	}
}

void Enemy::shootAll(){
	int count = 0;
	for(int i = 0; i < max_enemy;++i){
		if(enemies[i].state == EnemyState::LIVE){
			count++;
			if(enemies[i].status == EnemyState::NORMAL){
				if(enemies[i].speed <= 3){
					enemies[i].shoot(BulletState::HALFROUNDSHOOT, SR, enemies[i].pos);
				}
				else{
					enemies[i].shoot(BulletState::STRAIGHTSHOOT, SR, enemies[i].pos);
				}
			}
			else if(enemies[i].status == EnemyState::BIG){
				//TODO
			}
			else if(enemies[i].status == EnemyState::BOSS){
				if(enemies[i].last_shoot < 60){
					++enemies[i].last_shoot;
					return;
				}
				enemies[i].shoot(BulletState::HALFROUNDSHOOT, SR, enemies[i].pos - PointD(50, 0));
				enemies[i].last_shoot = 200;
				enemies[i].shoot(BulletState::REVERSEHALFROUNDSHOOT, SR, enemies[i].pos - PointD(50, 0));
				enemies[i].last_shoot = 200;
				enemies[i].shoot(BulletState::HALFROUNDSHOOT, SR, enemies[i].pos);
				enemies[i].last_shoot = 200;
				enemies[i].shoot(BulletState::REVERSEHALFROUNDSHOOT, SR, enemies[i].pos + PointD(50, 0));
				enemies[i].last_shoot = 200;
				enemies[i].shoot(BulletState::HALFROUNDSHOOT, SR, enemies[i].pos + PointD(50,0));
				enemies[i].last_shoot = 200;
				enemies[i].shoot(BulletState::TRACESHOOT, SR, enemies[i].pos, SR -> PC -> getPos());
				enemies[i].last_shoot = 0;
			}
			if(count == current_enemy){
				break;
			}
		}
	}
}

void Enemy::reset(){
	for(int i = 0; i < max_enemy; ++i){
		enemies[i].reset();
	}
		current_enemy = 0;
		last_allocate = 100;
}

void PaperBullet::init(const PointD &p, const PointD &v, const double sp){
	pos = p;
	velocity = v;
	speed = sp;
	owner = 0;
	BPB -> init(ObjId::NORMALBULLET, p);
}

void PaperBullet::reset(){
	//std::cout << "reset:" << pos.x << "," << pos.y << std::endl;
	pos = PointD();
	velocity = PointD();
	state = BulletState::UNSHOOTED;
	owner = 0;
	BPB -> reset();
}

void PaperBullet::drawBullet(Image *img){
	int w = 0, h = 0;
	getImageSize(img, w, h);
	drawImage(img, pos.x - w / 2, pos.y - h / 2);
}

Bullet::~Bullet(){
	delete [] bullets;
	for(int i = 0; i < 5; ++i){
		cleanup(BulletPic[i]);
	}
}

void Bullet::init(const int mb, std::string bulletPicPath){
	//varios bullets TODO here
	bullets = new PaperBullet[mb];
	BulletPic[0] = loadImage(bulletPicPath);
	max_bullet = mb;
	getImageSize(BulletPic[0], BulletW, BulletH);
}

void Bullet::checkAndDeal(){
	//std::cout << max_bullet <<":"<<current_bullet << std::endl;
	for(int i = 0; i < max_bullet; ++i){
		if(bullets[i].state == BulletState::SHOOTED){
			if(bullets[i].pos.x >= SCR_W || bullets[i].pos.x <= 0 || bullets[i].pos.y >= SCR_H || bullets[i].pos.y <= 0){
				bullets[i].reset();
				current_bullet--;
			}
			else if(bullets[i].BPB -> ifBumped() == BumpState::BUMPED){
				bullets[i].reset();
				current_bullet--;
			}
		}
	}
}

void Bullet::moveAll(){
	int count = 0;
	for(int i = 0; i < max_bullet; ++i){
		if(bullets[i].state == BulletState::SHOOTED){
			bullets[i].move();
			count++;
			if(count == current_bullet){
				break;
			}
		}
	}
}

void Bullet::drawAll(){
	int count = 0;
	for(int i = 0; i < max_bullet; ++i){
		if(bullets[i].state == BulletState::SHOOTED){
			bullets[i].drawBullet(BulletPic[0]);
			count++;
			if(count == current_bullet){
				break;
			}
		}
	}
}

int Bullet::allocateNewBullet(const PointD &p, const PointD &v, const double sp, const int ow){
	for(int i = 0; i < max_bullet; ++i){
		if(bullets[i].state == BulletState::UNSHOOTED){
			bullets[i].init(p, v, sp);
			bullets[i].owner = ow;
			bullets[i].state = BulletState::SHOOTED;
			current_bullet++;
			return 0;
		}
	}
	return 1;
}

void Bullet::reset(){
	for(int i = 0; i < max_bullet; ++i){
		bullets[i].reset();
	}
}


void PaperProp::init(const int type, const PointD &p, const PointD &v, const double sp){
	prop_type = type;
	pos = p;
	velocity = v;
	speed = sp;
	BPB -> init(ObjId::PROP, p);
}

void PaperProp::reset(){
	pos = PointD();
	velocity = PointD();
	speed = -1;
	prop_type = -1;
	state = PropState::OFF;
	BPB -> reset();
}

void PaperProp::drawProp(Image *img){
	int w = 0, h = 0;
	getImageSize(img, w, h);
	drawImage(img, pos.x - w / 2, pos.y - h / 2, 1, 1);
}

Prop::~Prop(){
	delete [] props;
}

void Prop::init(const int max_em){
	props = new PaperProp[2 * max_em];
	max_prop = max_em * 2;
	lifeProp = loadImage(PROPLIFE);
	enerProp = loadImage(PROPENER);
	bombProp = loadImage(PROPBOMB);
	getImageSize(lifeProp, PropW, PropH);
}

void Prop::moveAll(){
	int count = 0;
	for(int i = 0; i < max_prop; ++i){
		if(props[i].state == PropState::ON){
			count++;
			props[i].move();
			if(count == current_prop){
				return;
			}
		}
	}
}

void Prop::drawAll(){
	int count = 0;
	for(int i = 0; i < max_prop; ++i){
		if(props[i].state == PropState::ON){
			count++;
			if(props[i].prop_type == PropState::ADD_LIFE){
				props[i].drawProp(lifeProp);
			}
			else if(props[i].prop_type == PropState::ADD_ENER){
				props[i].drawProp(enerProp);
			}
			else if(props[i].prop_type == PropState::ADD_BOMB){
				props[i].drawProp(bombProp);
			}
			if(count == current_prop){
				return;
			}
		}
	}
}

void Prop::checkAndDeal(){
	for(int i = 0; i < max_prop ;++i){
		if(props[i].state == PropState::ON){
			if(props[i].BPB -> ifBumped() == BumpState::BUMPED){
				//std::cout << "BUMPED" <<std::endl;
				if(props[i].prop_type == PropState::ADD_LIFE){
					SR -> PC -> addLife(1);
				}
				else if(props[i].prop_type == PropState::ADD_ENER){
					SR -> PC -> addEnergy(5);
				}
				else if(props[i].prop_type == PropState::ADD_BOMB){
					SR -> PC -> addBomb(1);
				}
				props[i].reset();
			}
			else if(props[i].pos.x > SCR_W || props[i].pos.x < 0 || props[i].pos.y > SCR_H || props[i].pos.y < 0){
				props[i].reset();
			}
		}
	}
}

void Prop::allocateNewProp(const int type, const PointD &p, const PointD &v, const double sp){
	for(int i = 0; i < max_prop; ++i){
		if(props[i].state == PropState::OFF){
			props[i].init(type, p, v, sp);
			props[i].state = PropState::ON;
			return;
		}
	}
}

void Prop::allocateNewProp(const EnemyCraft &wreck, const int t){
	for(int i = 0; i < max_prop; ++i){
		if(props[i].state == PropState::OFF){
			//std::cerr << "NEW" <<wreck.pos.x << wreck.pos.y<< std::endl;
			props[i].init(t, wreck.pos, wreck.velocity, wreck.speed);
			props[i].prop_type = t;
			props[i].state = PropState::ON;
			props[i].BPB -> init(ObjId::PROP, wreck.pos);
			return;
		}
	}
}

void Prop::randomAllocate(){
	int f = rand() / 50000;
	if(f > 100){
		return;
	}
	else if(f > 50){
		allocateNewProp(PropState::ADD_ENER, PointD(rand() % (SCR_W - 40) + 20, 0), PointD(0, 1), rand() % 2 + 1);
		return;
	}
	else if(f > 10){
		allocateNewProp(PropState::ADD_LIFE, PointD(rand() % (SCR_W - 40) + 20, 0), PointD(0, 1), rand() % 2 + 1);
		return;
	}
	else{
		allocateNewProp(PropState::ADD_BOMB, PointD(rand() % (SCR_W - 40) + 20, 0), PointD(0, 1), rand() % 2 + 1);
		return;
	}
}




























