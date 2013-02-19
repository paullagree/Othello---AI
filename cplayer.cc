#include "cplayer.h"
#include "node.h"
#include "pruning.h"
#include <cstdlib>

namespace reversi
{

	CPlayer::CPlayer()
	{
	}

	void CPlayer::Initialize(bool pFirst,const CDeadline &pDue)
	{
		srand(CDeadline::Now());
		//You can use this function to perform initialization tasks for your program. It is
		//perfectly ok to leave it as is
	}

	CMove CPlayer::Play(const CBoard &pBoard,const CDeadline &pDue)
	{
		//Use the commented version if your system supports ANSI color (linux does)
		//pBoard.PrintNoColor();
		pBoard.Print();

		std::vector<CMove> lMoves;
		//this gets a list of our (OWN) possible moves
		pBoard.FindPossibleMoves(lMoves,OWN);
		///if the list of moves is empty, we must pass
		if(lMoves.empty()) return cPass;

		/*
		 * Here you should write your clever algorithms to get the best next move.
		 * This skeleton returns a random movement instead.
		 */


		int empty=0;
		for(int i=0;i<64;i++) {
			if(pBoard.At(i)==0)
				empty++;
		}
		Node *node=NULL;
		int optim=0;
		CBoard *cboard= new CBoard(pBoard);
		node=new Node(cboard,reversi::CMove(-1,-1));
		if(empty>55) {
			optim=pruning(node, 6, -40000, 40000, true, 0);
		}
		else if(empty>15) {
			optim=pruning(node, 6, -40000, 40000, true, 1);
		}
		else if(empty>13){
			optim=pruning(node, 9, -2000, 2000, true, 2);
		}
		else {
			optim=pruning(node, 12, -2000, 2000, true, 2);
		}
		CMove choix=node->getMeilleurFils()->getCMove();
		delete node;
		return choix;
	}

	/*namespace reversi*/ }
