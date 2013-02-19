#ifndef _REVERSI_PRUNING_H_
#define _REVERSI_PRUNING_H_

#include "cmove.h"
#include "node.h"
#include "cboard.h"
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <algorithm>


int pruning(Node *node, int depth, int alpha, int beta, bool maxPlayer, int moment)
{
	uint8_t who = node->isTerminal((maxPlayer)?1:2);
	if(depth==0 || who == 0) {
		return node->eval(moment);
	}

	int meilleurScore=-1000;
	node->setMeilleurFils(NULL);

	if(who == 1) {
		node->generateSons(maxPlayer);
		std::vector<Node*> sons = node->getSons();
		for(int i=0;i<sons.size();i++) {
			int score=pruning(sons[i], depth-1, alpha, beta, !maxPlayer, moment);
			alpha=std::max(alpha, score);
			if(score>meilleurScore) {
				meilleurScore=score;
				node->setMeilleurFils(sons[i]);
			}
			if(beta<=alpha) {
				break;
			}
		}
		return alpha;
	}

	else {
		node->generateSons(maxPlayer);
		std::vector<Node*> sons = node->getSons();
		for(int i=0;i<sons.size();i++) {
			beta=std::min(beta, pruning(sons[i], depth-1, alpha, beta, !maxPlayer, moment));
			if(beta<=alpha) {
				break;
			}
		}
		return beta;
	}
}		

/*namespace reversi*/ //}

#endif
