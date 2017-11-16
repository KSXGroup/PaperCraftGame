#include "PaperCraft.h"
SignalRouter(){
	cin >> max;
	PC = new PaperCraft(this);
	EM = new Enemy(this);
	BLT = new Bullet(this);
	PRP = new Propt(this);
}

~SignalRouter(){
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
			}
		}
	//PLAYER WITH BULLET
		for(int i = 0; i < BLT -> max_bullet; ++i){
			if(BLT -> bullets[i].owner == ObjId::ENEMY && BLT -> bullets[i].state == BulletState::SHOOTED){
				BLT -> bullets[i].BPB -> BumpDetect(PC -> BumpBox);
			}
		}
	}
	//ENEMY WITH BULLET
	//PLAYER WITH PROP
}