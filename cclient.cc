#include "cclient.h"

#include <sstream>

namespace reversi {

CClient::CClient(CPlayer &pPlayer)
    :   mPlayer(pPlayer)
{
}

CClient::~CClient()
{
}

void CClient::ReadInit(CDeadline &pTime,bool &pFirst)
{
    std::string lString;
    mSocket.ReadLine(lString);
    std::istringstream lS(lString);
    int64_t lTime;
    int lFirst;
    lS >> lTime >> lFirst;
    if(!lS) 
        throw std::runtime_error("wrong response from server");

    pTime=CDeadline(lTime/1000);
    pFirst=(lFirst!=0);
}

bool CClient::ReadMove(CDeadline &pTime,CMove &pMove,bool pBlock)
{
    std::string lString;
    if(!mSocket.ReadLine(lString,pBlock))
        return false;
    
    int lSpace=lString.find(' ');
    if(lSpace==std::string::npos) throw std::runtime_error("wrong response from server");

    std::istringstream lS(lString);
    int64_t lTime;
    lS >> lTime;
    if(!lS) 
        throw std::runtime_error("wrong response from server");
    
    pTime=CDeadline(lTime);
    pMove=CMove(lString.substr(lSpace+1),OTHER);
    return true;
}

void CClient::Run(const std::string &pHost,const std::string &pPort,
                  const std::string &pKey)
{
    mSocket.Init(pHost,pPort);
    if(pKey.empty())
    {
        mStandalone=true;
        mSocket.WriteLine("STANDALONE");
    }
    else
    {
        mStandalone=false;
        mSocket.WriteLine(pKey);
    }

    //receive the answer
    bool lFirst; //will be true if we play first
    CDeadline lTime; //time when initialization must be done
    ReadInit(lTime,lFirst);

    mBoard=CBoard(lFirst);
    
    if(mStandalone)
        lTime=CDeadline(CDeadline::Now()+10000);

    mPlayer.Initialize(lFirst,lTime);

    mSocket.WriteLine("INIT");
    
    while(true)
    {
        CMove lMove;

        ReadMove(lTime,lMove,true);

        if(mStandalone)
            lTime=CDeadline(CDeadline::Now()+2000);
        
        if(lMove.IsEOG())
        {
            if(mBoard.OwnCount()>mBoard.OtherCount())
                std::cout << "YOU WIN (" << mBoard.OwnCount() << "-" << mBoard.OtherCount() << ")\n";
            else if(mBoard.OwnCount()<mBoard.OtherCount())
                std::cout << "YOU LOSE (" << mBoard.OwnCount() << "-" << mBoard.OtherCount() << ")\n";
            else
                std::cout << "DRAW\n";

            return;
        }

        mBoard.DoMove(lMove);

        lMove=mPlayer.Play(mBoard,lTime);
        
        mSocket.WriteLine(lMove.ToString());
        
        if(lMove.IsEOG())
            return;
        
        mBoard.DoMove(lMove);
    }
}

/*namespace reversi*/ }
