#include "PaperCraft.h"
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

void PaperCraft::init(const PointD &p, const double _speed){
	pos = p;
	speed = _speed;
	last_shoot = 50;
}

void PaperCraft::move(){
}

















