#ifndef _REVERSI_CMOVE_H_
#define _REVERSI_CMOVE_H_

#include <vector>
#include <string>
#include <sstream>

namespace reversi {

///encapsulates a move

///in general, you should regard this as an opaque structure that you obtain
///from FindPossibleMoves, and provide to DoMove, or to the constructor of CBoard
///that takes a move.
///
///The functions IsPass() and IsEOG() can however be useful.
///
///The rest of the interface you can probably ignore it.
class CMove
{
public:
    ///constructs a move
    CMove(int pR=-1,int pC=-1,int pWho=0)
        :   mR(pR),mC(pC),mWho(pWho)
    {}

    ///\param pString a string, which should have been previously generated
    ///by ToString(), or obtained from the server
    CMove(const std::string &pString,int pWho)
        :	mWho(pWho)
    {
        std::istringstream lStream(pString);
        
        lStream >> mR >> mC;
    }

    ///returns true if the movement is null or invalid
    bool IsPass() const			 {	 return (mR==-1); }
    ///returns true if the movement represents end of game
    bool IsEOG() const			{	return (mR==-2); }
    ///row of the cell to play
    int Row() const           {   return mR;   }
    ///column of the cell to play
    int Col() const           {   return mC;   }
    ///either CBoard::OWN or CBoard::OTHER, indicating who is to play this move
    int Who() const			 {	 return mWho;			}

    ///converts the move to a string so that it can be sent to the server
    std::string ToString() const
    {
        std::ostringstream lStream;
        lStream << mR << ' ' << mC;
        
        return lStream.str();
    }

    ///returns true if the two objects represent the same move
    bool operator==(const CMove &pRH) const
    {
        return mR==pRH.mR&&mC==pRH.mC;
    }
    
private:
    int mR,mC,mWho;
};

///a global constant to return a pass move
static const CMove cPass;
///a global constant to return an EOG move
static const CMove cEOG(-2);

/*namespace reversi*/ }

#endif
