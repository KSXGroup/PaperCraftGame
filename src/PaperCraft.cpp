#include "PaperCraft.h"
using namespace Game;
SignalRouter::SignalRouter(){
	std::cin >> max;
	PC = new PlayerCraft(this);
	EM = new Enemy(this);
	BLT = new Bullet(this);
	PRP = new Prop(this);
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
	PC  -> init( PointD(SCR_W / 2 - PC -> PlayerPicW / 2,SCR_H - PC -> PlayerPicH / 2) , 10);
	EM  -> init(max / 2, max);
	BLT -> init(20 * max);
	PRP -> init(max);
}

void SignalRouter::drawAll(){
	PC  -> drawPlayer();
	EM  -> drawAll();
	BLT -> drawAll();
	PRP -> drawAll();
	setPenColor((Color){200, 200, 200, 255});
	drawRect(Rect{674,0,350,768},1);
}

void SignalRouter::dealAll(){
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
				EM -> enemies[i].BPB -> BumpDetect(PC -> BPB);
				if(PC -> BPB -> ifBumped() == BumpState::BUMPED){
					(PC -> life)--;
					PC -> BPB -> state = BumpState::NOTBUMPED;
					PC -> ProtectedTime += 240;
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
					PC -> ProtectedTime += 240;
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

void SignalRouter::reset(){
	//TODO
}

void BumpBox::init(const int type, const PointD &p, const int &PicW, const int &PicH){
	switch(type){
		case ObjId::PLAYER:
			size = 1;
			BumpUnit[0].unitPos = p;
			BumpUnit[0].r = 5;
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
	state =BumpState::NOTBUMPED;
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

void PlayerCraft::drawAttachments(){
	//TODO
}

void PlayerCraft::checkAndDeal(){
	if(ProtectedTime > 0){
		++ProtectedCount;
		if(ProtectedCount == ProtectedTime){
			ProtectedTime = 0;
			ProtectedCount = 0;
			state = PlayerState::LIVE;
		}
	}
	if(state == PlayerState::DIE){
		//TODO
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
		//TODO
	}
}
void EnemyCraft::shoot(const int strategy, const SignalRouter *SR){
	if(strategy == BulletState::HALFROUNDSHOOT){
		if(last_shoot < 150){
			last_shoot++;
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-sqrt(3), 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-1 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-1 , sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(-1 , 2 + sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(0 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(1 , 2 + sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(1 , sqrt(3)), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(1 , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(sqrt(3) , 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			last_shoot = 0;
		}
	}
	if(strategy == BulletState::STRAIGHTSHOOT){
		if(last_shoot < 20){
			last_shoot++;
			return;
		}
		else{
			SR -> BLT -> allocateNewBullet(PointD(pos.x, pos.y + 20), PointD(0, 1), speed + BulletState::NORMALSPEED, ObjId::ENEMY);
			last_shoot = 0;
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
		to_allocate = rand() % (max_enemy - current_enemy) + 2;
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
		std::cout << current_enemy << std::endl;
}

void Enemy::moveAll(){
	int count = 0;
	for(int i = 0;i < max_enemy; ++i){
		if(enemies[i].state == EnemyState::LIVE){
			enemies[i].move();
			++count;
			/*if(count == current_enemy){
				break;
			}*/
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
			if(enemies[i].life <= 0){
				enemies[i].reset();
				current_enemy--;
				tmp = rand() / 10000000;
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
				//TODO
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
					enemies[i].shoot(BulletState::HALFROUNDSHOOT, SR);
				}
				else{
					enemies[i].shoot(BulletState::STRAIGHTSHOOT, SR);
				}
			}
			else if(enemies[i].status == EnemyState::BIG){
				//TODO
			}
			else if(enemies[i].status == EnemyState::BOSS){
				//TODO
			}
			if(count == current_enemy){
				break;
			}
		}
	}
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
					SR -> PC -> addEnergy(10);
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
			std::cout << "wreck" << std::endl;
			props[i] = PaperProp(wreck);
			props[i].prop_type = t;
			props[i].state = PropState::ON;
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




























