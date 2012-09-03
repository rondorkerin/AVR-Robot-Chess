/**
 * \file chessengine.c
 *
 * \brief Implementation of MicroMax chess module
 * 
 * Created: 8/24/2012 9:43:35 PM
 *  Author: nul4x3r0000
 */ 

#include "chessengine.h"
#include "chessengine_p.h"

/**
 *  \brief Cause the AI to think a move for the current position
 *  and execute move
 *  
 *  \param outGameResult enum for result of the move (stalemate checkmate)
 *  \param outTookPieceFlag bit specifying whether a piece was taken
 *  \param outMove algebraic format move the AI made
 */ 
void ai_move(int* outGameResult, int* outTookPieceFlag, char* outMove)
{
	*outGameResult = *outTookPieceFlag = 0;
	
	// count the number of pieces for checking if a piece has been taken
	int prevPieceCount = get_piece_count();
	
	// not sure what these variables even are but they were set at the beginning of sendcommand.
	Z = 0;
	T = 0x3F;
	
	/* think up & do move, measure time used  */
	/* it is the responsibility of the engine */
	/* to control its search time based on    */
	/* MovesLeft, TimeLeft, MaxMoves, TimeInc */
	/* Next 'MovesLeft' moves have to be done */
	/* within TimeLeft+(MovesLeft-1)*TimeInc  */
	/* If MovesLeft<0 all remaining moves of  */
	/* the game have to be done in this time. */
	/* If MaxMoves=1 any leftover time is lost */
	PromPiece = 'q';
	N = 0;
	K = I;
	if (minimax(Side, -I, I, Q, O, 8, 3) == I) {
		
		// switch sides
		Side ^= 24;
	
		// send move result to output variable
		outMove[0] = 'a' + (K & 7);
		outMove[1] =  '8' - (K >> 4);
		outMove[2] = 'a' + (L & 7);
		outMove[3] = '8' - (L >> 4);
		
		*outTookPieceFlag = get_piece_count() == prevPieceCount ? 0 : 1;
		*outGameResult = game_status_result();
	} 
	return;
} 


/**
 *  \brief Executes a move specified in algebraic format ex: a2a4
 *  
 *  \param outGameResult enum for result of the move (stalemate checkmate)
 *  \param outTookPieceFlag bit specifying whether a piece was taken
 *  \param outMove algebraic format move the AI made
 */ 


char player_move(char* move, int* outGameResult, int* outTookPieceFlag, int* outIllegalMoveFlag)
{
	*outGameResult = *outTookPieceFlag = *outIllegalMoveFlag = 0;
	
	// count the number of pieces for checking if a piece has been taken
	int prevPieceCount = get_piece_count();
	
	// not sure what these variables even are but they were set at the beginning of sendcommand.
	Z = 0;
	T = 0x3F;
	
	Side = WHITE; // hardcode player to white side
	/* move not recognized, assume input move */
	m = move[0] < 'a' | move[0] > 'h' | move[1] < '1' | move[1] > '8' |
		move[2] < 'a' | move[2] > 'h' | move[3] < '1' | move[3] > '8';	
	K = move[0] - 16 * move[1] + 799;
	L = move[2] - 16 * move[3] + 799;
	
	/* doesn't have move syntax */
	if (*outIllegalMoveFlag = m)
	{
		return;
	}
	
	if (minimax(Side, -I, I, Q, O, 8, 3) != I) {
		/* did have move syntax, but illegal move */
		*outIllegalMoveFlag = 1;
		return;
	} 
	
	// switch sides
	Side ^= 24;
	
	// set output flags
	*outTookPieceFlag = get_piece_count() == prevPieceCount ? 0 : 1;
	*outGameResult = game_status_result();
}


/** 
 * \brief Start the engine
 */
void init_engine()
{
    int j;

    K = 8;
    W(K--) {
        L = 8;
        W(L--) b[16 * L + K + 8] = (K - 4) * (K - 4) + (L - 3.5) * (L - 3.5);   /* center-pts table   */
    }                           /*(in unused half b[]) */
}

/**
 * \brief Start a new game 
 */ 
void init_game(int gameType)
{
    int i, j;

    for (i = 0; i < 128; i++)
        b[i & ~M] = 0;
    K = 8;
    W(K--) {
        b[K] = (b[K + 112] = o[K + 24] + 8) + 8;
        b[K + 16] = 18;
        b[K + 96] = 9;          /* initial board setup */
    }                           /*(in unused half b[]) */
    Side = WHITE;

    Q = 0;
    O = S;
    R = 0;
	Computer = BLACK;
	
}

/**
 *  \brief Recursive Minimax Search
 */ 
short minimax(unsigned char k,short q,short l,short e,unsigned char E,unsigned char z,unsigned char n)    /* E=e.p. sqr.z=prev.dest, n=depth; return score */
{                       
 short m,v,i,P,V,s;
 unsigned char t,p,u,x,y,X,Y,H,B,j,d,h,F,G,C;
 signed char r;

 if (++Z>30) {                                     /* stack underrun check */
  --Z;return e;                                    
 }
 
 q-=q<e;l-=l<=e;                               /* adj. window: delay bonus */
 d=Y=0;                                        /* start iter. from scratch */
 X=myrand()&~M;                                /* start at random field    */
 W(d++<n||d<3||              /*** min depth = 2   iterative deepening loop */
   z==8&K==I&&(N<T&d<98||                      /* root: deepen upto time   */
   (K=X,L=Y&S-9,d=3)))                         /* time's up: go do best    */
 {x=B=X;                                       /* start scan at prev. best */
  h=Y&S;                                       /* request try noncastl. 1st*/
  P=d>2&&l+I?minimax(24-k,-l,1-l,-e,S,S,d-3):I;      /* search null move         */
  m=-P<l|R>35?d-2?-I:e:-P;  /*** prune if > beta  unconsidered:static eval */
  ++N;                                         /* node count (for timing)  */
  do{u=b[x];                                   /* scan board looking for   */
   if(u&k)                                     /*  own piece (inefficient!)*/
   {r=p=u&7;                                   /* p = piece type (set r>0) */
    j=o[p+16];                                 /* first step vector f.piece*/
    W(r=p>2&r<0?-r:-o[++j])                    /* loop over directions o[] */
    {A:                                        /* resume normal after best */
     y=x;F=G=S;                                /* (x,y)=move, (F,G)=castl.R*/
     do{                                       /* y traverses ray, or:     */
      H=y=h?Y^h:y+r;                           /* sneak in prev. best move */
      if(y&M)break;                            /* board edge hit           */
      m=E-S&b[E]&&y-E<2&E-y<2?I:m;             /* bad castling  */
      if(p<3&y==E)H^=16;                       /* shift capt.sqr. H if e.p.*/
      t=b[H];if(t&k|p<3&!(y-x&7)-!t)break;     /* capt. own, bad pawn mode */
      i=37*w[t&7]+(t&192);                     /* value of capt. piece t   */
      if(i<0)m=I,d=98;                         /* K capture                */
      if(m>=l&d>1)goto C;                      /* abort on fail high       */
      v=d-1?e:i-p;                             /*** MVV/LVA scoring if d=1**/
      if(d-!t>1)                               /*** all captures if d=2  ***/
      {v=p<6?b[x+8]-b[y+8]:0;                  /* center positional pts.   */
       b[G]=b[H]=b[x]=0;b[y]=u|32;             /* do move, set non-virgin  */
       if(!(G&M))b[F]=k+6,v+=50;               /* castling: put R & score  */
       v-=p-4|R>30?0:20;                       /*** freeze K in mid-game ***/
       if(p<3)                                 /* pawns:                   */
       {v-=9*((x-2&M||b[x-2]-u)+               /* structure, undefended    */
              (x+2&M||b[x+2]-u)-1              /*        squares plus bias */
             +(b[x^16]==k+36))                 /*** cling to magnetic K ***/
             -(R>>2);                          /* end-game Pawn-push bonus */
        i+=V=y+r+1&S?647-p:2*(u&y+16&32);      /* promotion / passer bonus */
        b[y]+=V;                               /* upgrade P or convert to Q*/
       }
       v+=e+i;V=m>q?m:q;                       /*** new eval & alpha    ****/
       C=d-1-(d>5&p>2&!t&!h);                  /* nw depth, reduce non-cpt.*/
       C=R>30|P-I|d<3||t&&p-4?C:d;             /* extend 1 ply if in-check */
       do
        s=C>2|v>V?-minimax(24-k,-l,-V,-v,/*** futility, recursive eval. of reply */
                                    F,y,C):v;
       W(s>q&++C<d);v=s;                       /* no fail:re-srch unreduced*/
       if(z&8&&K-I)                            /* move pending: check legal*/
       {if(v+I&&x==K&y==L)                     /*   if move found          */
        {Q=-e-i;O=F;
         R+=i>>7;                              /*** total captd material ***/
		 //if((b[y]&7)!=p && PromPiece == 'n') UnderProm = y;
         //if((b[y]&7)!=p) {printf("tellics kibitz promotion\n");fflush(stdout);};
               --Z;return l;                  /*   & not in check, signal */
				
        }
        v=m;                                   /* (prevent fail-lows on    */
       }                                       /*   K-capt. replies)       */
       b[G]=k+6;b[F]=b[y]=0;b[x]=u;b[H]=t;     /* undo move,G can be dummy */
      }                                        /*          if non-castling */
      if(v>m)                                  /* new best, update max,best*/
       m=v,X=x,Y=y|S&F;                        /* mark non-double with S   */
      if(h){h=0;goto A;}                       /* redo after doing old best*/
      if(x+r-y|u&32|                           /* not 1st step,moved before*/
         p>2&(p-4|j-7||                        /* no P & no lateral K move,*/
         b[G=x+3^r>>1&7]-k-6                   /* no virgin R in corner G, */
         ||b[G^1]|b[G^2])                      /* no 2 empty sq. next to R */
        )t+=p<5;                               /* fake capt. for nonsliding*/
      else F=y;                                /* enable e.p.              */
     }W(!t);                                   /* if not capt. continue ray*/
  }}}W((x=x+9&~M)-B);                          /* next sqr. of board, wrap */
C:m=m+I|P==I?m:0;        /*** check test thru NM  best loses K: (stale)mate*/	   
 }                                             /*    encoded in X S,8 bits */
if(z==S+1)K=X,L=Y&~M;
 --Z;return m+=m<e;                            /* delayed-loss bonus       */
}

/**
 * \brief returns the result of the present move (whether someone won or lost)
 * \return value based on the GameResult enum
 */  
int game_status_result(int side)
{
    int i, j, k, cnt = 0;

    K = I;
    cnt = minimax(side, -I, I, Q, O, side + 1, 3);
    if (cnt == 0 && K == 0 && L == 0) {
        //printf("1/2-1/2 {Stalemate}\n");
        return StaleMate;
    }
    if (cnt == -I + 1) {
        if (side == WHITE)
			return BlackMates;
            //printf("0-1 {Black mates}\n");
        else
            //printf("1-0 {White mates}\n");
			return WhiteMates;
    }

    return InPlay;
}

/**
 *  \brief Checks to see if there was a piece taken in the last turn
 */ 
int get_piece_count()
{
	int j, count = 0;
	
	// loop through all the board squares and count pieces
	for (j = 0; j < 64; j++)
	{
		char piece = b[j + (j & 0x38)];        /* board squares  */	
		if (piece != EMPTY && piece != '@')
			count++;
	}
	
	return count;
}


/**
 * \brief Seed random number generator
 */ 
void mysrand(unsigned short r_)
{
	r = r_;
}


/**
 * \brief Get value from random number generator
 */ 
unsigned short myrand(void) 
{
	return r=((r<<11)+(r<<7)+r)>>1;
}