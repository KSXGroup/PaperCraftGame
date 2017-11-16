#include "PaperCraft.h"
SignalRouter(){
	cin >> max;
	PC = new PaperCraft(this);
	EM = new Enemy(this);
	BLT = new Bullet(this);
	PRP = new Propt(this);
}

~SignalRouter(){
	
}