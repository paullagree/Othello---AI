#ifndef _REVERSI_CPLAYER_H_
#define _REVERSI_CPLAYER_H_

#include "cdeadline.h"
#include "cmove.h"
#include "cboard.h"
#include <vector>

namespace reversi {

class CPlayer
{
public:
    ///constructor
    
    ///Shouldn't do much. Any expensive initialization should be in 
    ///Initialize
    CPlayer();

    ///perform initialization of the player
    
    ///\param pFirst true if we will move first, false otherwise
    ///\param pDue time before which we must have returned. To check how
    ///much time we have left you can use \p pDue.TimeUntil(), which returns
    ///the remaining time in milliseconds.
    ///That's the only safe way to use the pDue parameter.
    void Initialize(bool pFirst,const CDeadline &pDue);

    ///perform a move

    ///\param pBoard the current state of the board
    ///\param pDue time before which we must have returned. To check how
    ///much time we have left you can use \p pDue.TimeUntil(), which returns
    ///the remaining time in milliseconds.
    ///That's the only safe way to use the pDue parameter.
    ///\return the move we make
    CMove Play(const CBoard &pBoard,const CDeadline &pDue);
};

/*namespace reversi*/ }

#endif
