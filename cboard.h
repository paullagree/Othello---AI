#ifndef _REVERSI_CBOARD_H_
#define _REVERSI_CBOARD_H_

#include "cmove.h"
#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>

namespace reversi {

///for internal use in move calculation
static const int sTryDelta[8][2]=
{
    {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}
};

///this enumeration is used to specify the content of a cell of the board.
///it is also used when we want to say whose turn it is to move, or which player made a move
enum ECell
{
    EMPTY=0,			///< the cell is empty
    OWN=1,			///< the cell belongs to us (our agent)
    OTHER=2,			///< the cell belongs to the other player
};

///represents a position in an 8x8 board
class CBoard
{
public:
    static const int cSize=8;		///< size of board
    
    ///Initializes the board to the starting position
    
    ///pFirst indicates if we are going to play first
    explicit CBoard(bool pFirst=true)
    {
        for(int i=0;i<cSize;i++)
        {
            for(int j=0;j<cSize;j++)
            {
                if(i>=cSize/2-1&&i<=cSize/2&&j>=cSize/2-1&&j<=cSize/2)
                    mCell[i*cSize+j]=(i==j&&!pFirst||i!=j&&pFirst)?OWN:OTHER;
                else
                    mCell[i*cSize+j]=EMPTY;
            }
        }
        mOwn=2;
        mOther=2;
    }

    ///constructs a board which is the result of applying move \p pMove to board \p pRH
    
    /// \param pRH the starting board position
    /// \param pMove the movement to perform
    ///
    /// \sa DoMove()
    CBoard(const CBoard &pRH,const CMove &pMove)
    {
        memcpy(mCell,pRH.mCell,sizeof(mCell));
        mOwn=pRH.mOwn;
        mOther=pRH.mOther;
        
        DoMove(pMove);
    }

    ///number of own pieces
    int OwnCount() const		{	return mOwn;	}
    ///number of opponent's pieces
    int OtherCount() const		{	return mOther;	}

    ///returns the content of a cell in the board.
    uint8_t At(int pPos) const
    {
        assert(pPos<cSize*cSize);
        return mCell[pPos];
    }
    
    ///operator version of the above function
    uint8_t operator()(int pPos) const
    {
        return At(pPos);
    }
    
    ///returns the content of a cell in the board.
    uint8_t At(int pR,int pC) const
    {
        assert(pR>=0&&pR<cSize&&pC>=0&&pC<cSize);
        return mCell[pR*cSize+pC];
    }

    ///operator version of the above function
    uint8_t operator()(int pR,int pC) const
    {
        return At(pR,pC);
    }

private:
    uint8_t PrivAt(int pR,int pC) const
    {
        return mCell[pR*cSize+pC];
    }
    
    uint8_t& PrivAt(int pR,int pC)
    {
        return mCell[pR*cSize+pC];
    }

public:
    /// returns a list of all valid moves for \p pWho
    
    /// \param pMoves a vector where the list of moves will be appended
    /// \param pWho OWN if it is our turn to move, OTHER if it is the other player
    void FindPossibleMoves(std::vector<CMove> &pMoves,int pWho) const
    {
        pMoves.clear();
        assert(pWho==OWN||pWho==OTHER);
        int lOther=pWho^(OWN|OTHER);

        for(int r=0;r<cSize;r++)
        {
            for(int c=0;c<cSize;c++)
            {
                if(PrivAt(r,c)!=EMPTY) continue;
                
                bool lFound=false;
                
                for(int t=0;t<8;t++)
                {
                    for(int nr=r+sTryDelta[t][0],nc=c+sTryDelta[t][1];
                        nr<cSize&&nc<cSize&&nr>=0&&nc>=0;
                        nr+=sTryDelta[t][0],nc+=sTryDelta[t][1])
                    {
                        if(PrivAt(nr,nc)==EMPTY) break;
                        if(PrivAt(nr,nc)==lOther) continue;
                        if(nr!=r+sTryDelta[t][0]||nc!=c+sTryDelta[t][1])
                            lFound=true;
                        break;
                    }
                    
                    if(lFound) break;
                }
                
                if(lFound)
                {
                    pMoves.push_back(CMove(r,c,pWho));
                }
            }
        }
    }
    
    ///transforms the board by performing a move

    ///it doesn't check that the move is valid, so you should only use
    ///it with moves returned by FindPossibleMoves 
    
    /// \param pMove the move to perform
    void DoMove(const CMove &pMove)
    {
        if(pMove.IsPass()) return;

        int r=pMove.Row(),c=pMove.Col();
        int lWho=pMove.Who();
        int lOther=lWho^(OWN|OTHER);

        int lTurned=0;
        
        for(int t=0;t<8;t++)
        {
            bool lFound=false;
        
            for(int nr=r+sTryDelta[t][0],nc=c+sTryDelta[t][1];
                nr<cSize&&nc<cSize&&nr>=0&&nc>=0;
                nr+=sTryDelta[t][0],nc+=sTryDelta[t][1])
            {
                if(PrivAt(nr,nc)==EMPTY) break;
                if(PrivAt(nr,nc)==lOther) continue;
                if(nr!=r+sTryDelta[t][0]||nc!=c+sTryDelta[t][1])
                    lFound=true;
                break;
            }
                    
            if(lFound)
            {
                for(int nr=r+sTryDelta[t][0],nc=c+sTryDelta[t][1];
                    nr<cSize&&nc<cSize&&nr>=0&&nc>=0;
                    nr+=sTryDelta[t][0],nc+=sTryDelta[t][1])
                {
                    if(PrivAt(nr,nc)==lOther)
                    {
                        lTurned++;
                        PrivAt(nr,nc)=lWho;
                    }
                    else
                        break;
                }
                
            }
        }

        PrivAt(r,c)=lWho;
        
        if(lWho==OWN)
        {
            mOwn+=lTurned+1;
            mOther-=lTurned;
        }
        else
        {
            mOther+=lTurned+1;
            mOwn-=lTurned;
        }
    }

    ///prints the board
    
    ///Useful for debug purposes. Don't call it in the final version.
    void Print() const
    {
        for(int r=0;r<cSize;r++)
        {
            for(int c=0;c<cSize;c++)
            {
                if(PrivAt(r,c)&OWN)
                {
                    std::cout << "\e[1;37;46m()\e[0m";
                }
                else if(PrivAt(r,c)&OTHER)
                {
                    std::cout << "\e[30;46m()\e[0m";
                }
                else
                {
                    std::cout << "\e[1;37;46m  \e[0m";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
    ///prints the board (no color version)
    
    ///Useful for debug purposes. Don't call it in the final version.
    void PrintNoColor() const
    {
        for(int r=0;r<cSize;r++)
        {
            for(int c=0;c<cSize;c++)
            {
                if(PrivAt(r,c)&OWN)
                {
                    std::cout << "OO";
                }
                else if(PrivAt(r,c)&OTHER)
                {
                    std::cout << "XX";
                }
                else
                {
                    std::cout << "  ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    
private:   
    uint8_t mCell[cSize*cSize];
    int mOwn,mOther;
};

/*namespace reversi*/ }

#endif
