#ifndef _REVERSI_NODE_H_
#define _REVERSI_NODE_H_

#include "cmove.h"
#include "cboard.h"
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>

//namespace reversi {

int valueBoard3[8][8] = { {20,-3,11,8,8,11,-3,20} , {-3,-7,-4,1,1,-4,-7,-3} , {11,-4,2,2,2,2,-4,11} , {8,1,2,-3,-3,2,1,8} , {8,1,2,-3,-3,2,1,8} , {11,-4,2,2,2,2,-4,11} , {-3,-7,-4,1,1,-4,-7,-3} , {20,-3,11,8,8,11,-3,20} };
int valueBoard2[8][8] = { {0,0,1,2,2,1,0,0} , {0,0,3,3,3,3,0,0} , {0,3,20,20,20,20,3,0} , {0,3,20,60,60,20,3,0} , {0,3,20,60,60,20,3,0} , {0,3,20,20,20,20,3,0} , {0,0,3,3,3,3,0,0} ,  {0,0,1,2,2,1,0,0} };
int valueBoard[8][8] = { {500,-150,30,10,10,30,-150,500} , {-150,-250,0,0,0,0,-250,-150} , {30,0,1,2,2,1,0,30} , {10,0,2,16,16,2,1,10} , {10,0,2,16,16,2,1,10}  , {30,0,1,2,2,1,0,30} , {-150,-250,0,0,0,0,-250,-150} , {500,-150,30,10,10,30,-150,500} };

class Node 
{
	private:
		reversi::CBoard *cboard;
		std::vector<Node*> sons;
		Node* meilleurFils;
		reversi::CMove cmove;

	public:
		Node(reversi::CBoard *cboard, reversi::CMove cmove) {
			this->cboard=cboard;
			this->cmove=cmove;
		}

		~Node()
		{
			for(int i=0; i<sons.size();i++) {
				delete sons[i];
			}
			delete cboard;
		}

		reversi::CBoard* getCBoard() {
			return cboard;
		}

		reversi::CMove getCMove() {
			return cmove;
		}

		std::vector<Node*> getSons() {
			return sons;
		}

		Node* getMeilleurFils() {
			return meilleurFils;
		}

		void setMeilleurFils(Node* meilleurFils) {
			this->meilleurFils=meilleurFils;
		}

		uint8_t isTerminal(uint8_t who) {
			uint8_t other = (who == 1)? 2:1;
			std::vector<reversi::CMove> pMoves;
			this->cboard->FindPossibleMoves(pMoves,who);
			if(pMoves.empty()) {
				this->cboard->FindPossibleMoves(pMoves,other);
				if(pMoves.empty()) {
					return 0;
				}
				return other;
			}
			return who;
		}

		int eval(int moment) {
			//ToDo		
			int value=0;

			std::vector<reversi::CMove> pMoves;
			this->cboard->FindPossibleMoves(pMoves,1);
			int moves=pMoves.size();

			if(moment==0) {
				for(int i=0;i<8;i++) {
					for(int j=0;j<8;j++) {
						if(this->cboard->At(i,j)==1)
							value+=valueBoard2[i][j];
					}
				}
				value = moves+value;
				return value;
			}

			if(moment==1) {
				for(int i=0;i<8;i++) {
					for(int j=0;j<8;j++) {
						if(this->cboard->At(i,j)==1)
							value+=valueBoard[i][j];
					}
				}
				value = moves+value;
				return value;
			}

			else {
				for(int i=0;i<8;i++) {
					for(int j=0;j<8;j++) {
						if(this->cboard->At(i,j)==1)
							value++;
					}
				}
				return value;
			}
		}

		void generateSons(bool maxPlayer) {
			int pWho=2;

			if(maxPlayer) {
				pWho=1;
			}
			std::vector<reversi::CMove> pMoves;
			this->cboard->FindPossibleMoves(pMoves,pWho);
			for(int i=0;i<pMoves.size();i++) {
				Node* pointeur=new Node(new reversi::CBoard(*this->cboard,pMoves[i]),pMoves[i]);
				sons.push_back(pointeur);
			}
		}

};

/*namespace reversi*/ //}

#endif
