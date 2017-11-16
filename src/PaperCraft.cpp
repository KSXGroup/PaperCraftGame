#include "PaperCraft.h"
using namespace Game;
SignalRouter::SignalRouter(){
	cin >> max;
	PC = new PaperCraft(this);
	EM = new Enemy(this);
	BLT = new Bullet(this);
	PRP = new Propt(this);
}

SignalRouter::~SignalRouter(){
	state = GameState::END;
	delete PC;
	delete EM;
	delete BLT;
	delete PRP;
}

void SignalRouter::init(){
	srand(time(NULL));
	PC  -> init(SCR_W / 2 - PC -> PlayerPicW / 2,SCR_H - PC -> PlayerPicH / 2, 5);
	EM  -> init(max / 2, max);
	BLT -> init(50 * max);
	PRP -> init(max);
}

void SignalRouter::drawAll(){
	PC  -> drawPlayer();
	EM  -> drawAll();
	BLT -> drawAll();
	PRP -> drawAll();
}

void SignalRouter::dealAll(){
	PC  -> checkAndDeal();
	EM  -> checkAndDeal();
	BLT -> checkAndDeal();
	PRP -> checkAndDeal();
}

void SignalRouter::moveAll(){
	PC  -> move();
	EM  -> moveAll();
	BLT -> moveAll();
	PRP -> moveAll();
}

void SignalRouter::dealBump(){
	if(PC -> state == PlayerState::LIVE){
	//PLAYER WITH ENEMY
		for(int i = 0; i < EM -> max_enemy; ++i){
			if(EM -> enemies[i].state == EnemyState::LIVE){
				EM -> enemies[i].BPB -> BumpDect(PC -> BPB);
				if(PC -> BPB -> ifBumped() == BumpState::BUMPED){
					(PC -> life)--;
					PC -> BPB -> state = BumpState::NOTBUMPED;
					(EM -> enemies[i].life)--;
					EM -> enemies[i].BPB -> state = BumpState::NOTBUMPED;
					break;
				}
			}
		}
	//PLAYER WITH BULLET
		for(int i = 0; i < BLT -> max_bullet; ++i){
			if(BLT -> bullets[i].owner == ObjId::ENEMY && BLT -> bullets[i].state == BulletState::SHOOTED){
				BLT -> bullets[i].BPB -> BumpDetect(PC -> BumpBox);
				if(PC -> BPB -> ifBumped() == BumpState::BUMPED){
					(PC -> life)--;
					PC -> BPB -> state = BumpState::NOTBUMPED;
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
						if(EM -> enemies[i].BPB -> ifBumped() == BumpState::Bumped){
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

void SignalRouter::reset(){
	//TODO
}

void BumpBox::init(const int type, const PointD &p, const int PicW = 10, const int PicH = 10){
	switch(type){
		case ObjId::PLAYER:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = PlayerState::BUMPRANGE;
			break;
		case ObjId::ENEMY:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = EnemyState::BUMPRANGE;
			break;
		case ObjId::NORMALBULLET:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = NORMALBUMPRANGE;
			break;
	}
}

void BumpBox::move(const PointD &v, const double sp){
	PointD tmp = v;
	double vlen = tmp.length();
	if(vlen > speed){
		tmp = tmp / vlen * speed;
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
}


void BumpBox::BumpDetect(BumpBox *target){
	double dx = 0, dy = 0, dis = 0;
	for(int i = 0;i < size;++i){
		for(int j = 0; j < target -> size; ++j){
			dx = BumpUnit[i].unitPos.x - target -> BumpUnit[j].unitPos.x;
			dy = BumpUnit[i].unitPos.y - target -> BumpUnit[j].unitPos.y;
			dis = sqrt(dx * dx + dy * dy);
			if(dis < (BumpUnit[i].r + target -> BumpUnit[j].r)){
				state = BumpState::BUMPED;
				target -> state = BumpState::BUMPED;
				break;
			}
		}
	}
}

inline void BumpBox::ifBumped(){
	return state;
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
	if(vlen > speed){
		pos = pos + velocity / vlen * speed;
	}
	else{
		pos = pos + velocity;
	}
	BPB -> move(velocity, speed);
}

PaperCraft::~PaperCraft(){
	cleanup(PlayerPic);
	cleanup(Protector);
}

void PaperCraft::init(const PointD &p, const double _speed){
	pos = p;
	speed = _speed;
	last_shoot = 50;
	PlayerPic = loadImage(PLAYERPIC);
	Protector = loadImage(ROUNDPIC);
	getImageSize(PlayerPic, PlayerPicW, PlayerPicH);
	getImageSize(Protector, ProtectorW, ProtectorH);
	BPB -> init(ObjId::PLAYER, pos, PlayerPicW, PlayerPicH);
}

void PaperCraft::move(){
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

void PaperCraft::shoot(){
	if(SR -> keyboard[KEY_SPACE] && last_shoot >= 50){
		SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y - 20), PointD(0, -1), speed + BulletState::NORMALSPEED, ObjId::PLAYER);
	}
}

void PaperCraft::drawPlayer(){
	if(state != PlayerState::DIE){
		drawImage(pos.x - PlayerPicW / 2, pos.y - PlayerPicH / 2);
		if(state == PlayerState::PROTECTED){
			drawImage(pos.x - ProtectorW / 2, pos.y - ProtectorH / 2);
		}
	}
}

void PaperCraft::drawAttachments(){
	//TODO
}

void PaperCraft::checkAndDeal(){
	//TODO
}

void EnemyCraft::init(const PointD &p, const double _speed, const int type){
	pos = p;
	speed = _speed;
	if(type = EnemyState::NORMAL){
		life = 1;
		last_shoot = 200;
		status = type;
		BPB -> init(ObjId::ENEMY, p, speed);
	}
	else if(type == EnemyState::BIG){
		//TODO
	}
	else if(type == EnemyState::BOSS){
		//TODO
	}
}
void EnemyCraft::shoot(const int strategy, const SignalRouter *SR){
	if(strategy == BulletState::HALFROUNDSHOOT){
		if(last_shoot < 100){
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-sqrt(3), 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-1 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-1 , 2 + sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(0 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(1 , 2 + sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(1 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(sqrt(3) , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
		}
	}
	if(strategy == BulletState::STRAGIHTSHOOT){
		if(last_shoot < 50){
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(0, 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
		}
	}
}

void EnemCraft::drawEnemy(Image *img){
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

void Enemy::~Enmey(){
	delete [] enemies;
}

void Enemy::init(const int start_amount, const int max_amount){
	current_enemy = start_amount;
	max_enemy = max_amount;
	EnemyPicNormal = loadImage(ENEMYPICNORMAL);
	enemies = new EnemyCraft(max_amount + 10);
	for(int i = 0; i < start_amount; ++i){
		enemies[i].init(PointD(rand() % (SCR_W - 40) + 20, 20), rand() % EnemyState::NORMALSPEED + 2, EnemyState::NORMAL);
		enemies[i].state = EnemyState::LIVE;
	}
	for(int i = start_amount;i < max_amount; ++i){
		enemies[i].init(PointD(), 0, EnemyState::NORMAL);
	}
}

void Enemy::allocateNewEnemy(){
	if(last_allocate < 200 || current_enemy  == max_enemy){
		return;
	}
	else{
		
	}
}



























































