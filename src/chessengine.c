/***************************************************************************/
/*                               micro-Max,                                */
/* A chess program smaller than 2KB (of non-blank source), by H.G. Muller  */
/* Port to Atmel ATMega and AVR GCC, by Andre Adrian                       */
/***************************************************************************/
/* version 4.8 (~1900 characters) features:                                */
/* - recursive negamax search                                              */
/* - all-capture quiescence search with MVV/LVA priority                   */
/* - (internal) iterative deepening                                        */
/* - best-move-first 'sorting'                                             */
/* - a hash table storing score and best move                              */
/* - futility pruning                                                      */
/* - king safety through magnetic frozen king                              */
/* - null-move pruning                                                     */
/* - Late-move reductions                                                  */
/* - full FIDE rules (expt minor promotion) and move-legality checking     */
/* - keep hash + rep-draw detect                                           */
/* - end-game Pawn-push bonus, new piece values, gradual promotion         */

/* Rehash sacrificed, simpler retrieval. Some characters squeezed out.     */
/* No hash-table clear, single-call move-legality checking based on K==I   */

/* fused to generic Winboard driver */

/* 26nov2008 no hash table */
/* 29nov2008 pseudo random generator  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <windows.h>

int StartKey;

#define EMPTY 0
#define WHITE 8
#define BLACK 16

#define STATE 64

/* make unique integer from engine move representation */
#define PACK_MOVE 256*K + L;

/* convert intger argument back to engine move representation */
#define UNPACK_MOVE(A) K = (A)>>8 & 255; L = (A) & 255;

/* Global variables visible to engine. Normally they */
/* would be replaced by the names under which these  */
/* are known to your engine, so that they can be     */
/* manipulated directly by the interface.            */

int Side;
int Move;
int PromPiece;
int Result;
int TimeLeft;
int MovesLeft;
int MaxDepth;
int Post;
int Fifty;
int UnderProm;

int Ticks, tlim;

int GameHistory[1024];
char HistoryBoards[1024][STATE];
int GamePtr, HistPtr;

#define W while
#define M 0x88
#define S 128
#define I 8000

long N, T;                  /* N=evaluated positions+S, T=recursion limit */
short Q,O,K,R;

char L,
w[]={0,2,2,7,-1,8,12,23},                      /* relative piece values    */
o[]={-16,-15,-17,0,1,16,0,1,16,15,17,0,14,18,31,33,0, /* step-vector lists */
     7,-1,11,6,8,3,6,                          /* 1st dir. in o[] per piece*/
     6,3,5,7,4,5,3,6},                         /* initial piece setup      */
b[129],                                /* board, center piece table, dummy */

n[]=".?inkbrq?I?NKBRQ",            /* piece symbols on printout, Pawn is i */
Z;                                                  /* Z=recursion counter */

/* 16bit pseudo random generator by Andre Adrian */
#define MYRAND_MAX 65535

unsigned short r = 1;                     /* pseudo random generator seed */

void mysrand(unsigned short r_) {
 r = r_;
}

unsigned short myrand(void) {
 return r=((r<<11)+(r<<7)+r)>>1;
}

short D(k,q,l,e,E,z,n)          /* recursive minimax search, k=moving side */
short q,l,e;                        /* (q,l)=window, e=current eval. score */
unsigned char k,E,z,n;    /* E=e.p. sqr.z=prev.dest, n=depth; return score */
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
  P=d>2&&l+I?D(24-k,-l,1-l,-e,S,S,d-3):I;      /* search null move         */
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
        s=C>2|v>V?-D(24-k,-l,-V,-v,/*** futility, recursive eval. of reply */
                                    F,y,C):v;
       W(s>q&++C<d);v=s;                       /* no fail:re-srch unreduced*/
       if(z&8&&K-I)                            /* move pending: check legal*/
       {if(v+I&&x==K&y==L)                     /*   if move found          */
        {Q=-e-i;O=F;
         R+=i>>7;                              /*** total captd material ***/
         if((b[y]&7)!=p && PromPiece == 'n') UnderProm = y;
         if((b[y]&7)!=p) {printf("tellics kibitz promotion\n");fflush(stdout);};
         Fifty = t|p<3?0:Fifty+1;
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
if(z==8&Post){
  printf("%2d ",d-2);
  printf("%6d ",m);
  printf("%8d %10d %c%c%c%c\n",(GetTickCount()-Ticks)/10,N,
     'a'+(X&7),'8'-(X>>4),'a'+(Y&7),'8'-(Y>>4&7)),fflush(stdout);}
 }                                             /*    encoded in X S,8 bits */
if(z==S+1)K=X,L=Y&~M;
 --Z;return m+=m<e;                            /* delayed-loss bonus       */
}

/* Generic main() for Winboard-compatible engine     */
/* (Inspired by TSCP)                                */
/* Author: H.G. Muller                               */

/* The engine is invoked through the following       */
/* subroutines, that can draw on the global vaiables */
/* that are maintained by the interface:             */
/* Side         side to move                         */
/* Move         move input to or output from engine  */
/* PromPiece    requested piece on promotion move    */
/* TimeLeft     ms left to next time control         */
/* MovesLeft    nr of moves to play within TimeLeft  */
/* MaxDepth     search-depth limit in ply            */
/* Post         boolean to invite engine babble      */

/* InitEngine() progran start-up initialization      */
/* InitGame()   initialization to start new game     */
/*              (sets Side, but not time control)    */
/* Think()      think up move from current position  */
/*              (leaves move in Move, can be invalid */
/*               if position is check- or stalemate) */
/* DoMove()     perform the move in Move             */
/*              (togglese Side)                      */
/* ReadMove()   convert input move to engine format  */
/* PrintMove()  print Move on standard output        */
/* Legal()      check Move for legality              */
/* ClearBoard() make board empty                     */
/* PutPiece()   put a piece on the board             */

/* define this to the codes used in your engine,     */
/* if the engine hasn't defined it already.          */

int PrintResult(int s)
{
    int i, j, k, cnt = 0;

    /* search last 50 states with this stm for third repeat */
    for (j = 2; j <= 100; j += 2) {
        for (k = 0; k < STATE; k++)
            if (HistoryBoards[HistPtr][k] !=
                HistoryBoards[HistPtr - j & 1023][k]) {
                goto differs;
            }
        /* is the same, count it */
        if (++cnt == 2) {       /* third repeat */
            printf("1/2-1/2 {Draw by repetition}\n");
            return 1;
        }
      differs:;
    }
    K = I;
    cnt = D(s, -I, I, Q, O, S + 1, 3);
    if (cnt == 0 && K == 0 && L == 0) {
        printf("1/2-1/2 {Stalemate}\n");
        return 2;
    }
    if (cnt == -I + 1) {
        if (s == WHITE)
            printf("0-1 {Black mates}\n");
        else
            printf("1-0 {White mates}\n");
        return 3;
    }
    if (Fifty >= 100) {
        printf("1/2-1/2 {Draw by fifty move rule}\n");
        return 4;
    }
    return 0;
}


InitEngine()
{
    int j;

    K = 8;
    W(K--) {
        L = 8;
        W(L--) b[16 * L + K + 8] = (K - 4) * (K - 4) + (L - 3.5) * (L - 3.5);   /* center-pts table   */
    }                           /*(in unused half b[]) */
}

InitGame()
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
    Fifty = R = 0;
    UnderProm = -1;
}

void CopyBoard(int s)
{
    int j, k, cnt = 0;

    /* copy game representation of engine to HistoryBoard */
    /* don't forget castling rights and e.p. state!       */
    for (j = 0; j < 64; j++)
        HistoryBoards[s][j] = b[j + (j & 0x38)];        /* board squares  */
    if (!(O & M))
        HistoryBoards[s][O + (O & 7) >> 1] |= 64;       /* mark ep square */
}

int main(int argc, char **argv)
{
    int Computer, MaxTime, MaxMoves, TimeInc, sec, i;
    char line[256], command[256];
    int m, nr;

    signal(SIGINT, SIG_IGN);
    mysrand(time(NULL));        /* make myrand() calls random */
    printf("tellics say     micro-Max 4.8 (m)\n");
    printf("tellics say     by H.G. Muller\n");
    InitEngine();
    InitGame();
    Computer = EMPTY;
    MaxTime = 10000;            /* 10 sec */
    MaxDepth = 30;              /* maximum depth of your search */

    for (;;) {
        fflush(stdout);
        Z = 0;
        T = 0x3F;
        if (Side == Computer) {
            /* think up & do move, measure time used  */
            /* it is the responsibility of the engine */
            /* to control its search time based on    */
            /* MovesLeft, TimeLeft, MaxMoves, TimeInc */
            /* Next 'MovesLeft' moves have to be done */
            /* within TimeLeft+(MovesLeft-1)*TimeInc  */
            /* If MovesLeft<0 all remaining moves of  */
            /* the game have to be done in this time. */
            /* If MaxMoves=1 any leftover time is lost */
            Ticks = GetTickCount();
            m = MovesLeft <= 0 ? 40 : MovesLeft;
            tlim = 0.6 * (TimeLeft + (m - 1) * TimeInc) / (m + 7);
            PromPiece = 'q';
            N = 0;
            K = I;
            if (D(Side, -I, I, Q, O, 8, 3) == I) {
                Side ^= 24;
                if (UnderProm >= 0 && UnderProm != L) {
                    printf("tellics I hate under-promotions!\n");
                    printf("resign\n");
                    Computer = EMPTY;
                    continue;
                } else
                    UnderProm = -1;
                printf("move ");
                printf("%c%c%c%c", 'a' + (K & 7), '8' - (K >> 4),
                       'a' + (L & 7), '8' - (L >> 4));
                printf("\n");
                m = GetTickCount() - Ticks;

                /* time-control accounting */
                TimeLeft -= m;
                TimeLeft += TimeInc;
                if (--MovesLeft == 0) {
                    MovesLeft = MaxMoves;
                    if (MaxMoves == 1)
                        TimeLeft = MaxTime;
                    else
                        TimeLeft += MaxTime;
                }

                GameHistory[GamePtr++] = PACK_MOVE;
                CopyBoard(HistPtr = HistPtr + 1 & 1023);
                if (PrintResult(Side))
                    Computer = EMPTY;
            } else {
                if (!PrintResult(Side))
                    printf("resign\n");
                Computer = EMPTY;
            }
            continue;
        }
        if (!fgets(line, 256, stdin))
            return;
        if (line[0] == '\n')
            continue;
        sscanf(line, "%s", command);
        if (!strcmp(command, "xboard"))
            continue;
        if (!strcmp(command, "new")) {
            /* start new game */
            InitGame();
            GamePtr = 0;
            HistPtr = 0;
            Computer = BLACK;
            TimeLeft = MaxTime;
            MovesLeft = MaxMoves;
            for (nr = 0; nr < 1024; nr++)
                for (m = 0; m < STATE; m++)
                    HistoryBoards[nr][m] = 0;
            continue;
        }
        if (!strcmp(command, "quit"))
            /* exit engine */
            return;
        if (!strcmp(command, "force")) {
            /* computer plays neither */
            Computer = EMPTY;
            continue;
        }
        if (!strcmp(command, "white")) {
            /* set white to move in current position */
            Side = WHITE;
            Computer = BLACK;
            continue;
        }
        if (!strcmp(command, "black")) {
            /* set blck to move in current position */
            Side = BLACK;
            Computer = WHITE;
            continue;
        }
        if (!strcmp(command, "st")) {
            /* move-on-the-bell mode     */
            /* indicated by MaxMoves = 1 */
            sscanf(line, "st %d", &MaxTime);
            MovesLeft = MaxMoves = 1;
            TimeLeft = MaxTime *= 1000;
            TimeInc = 0;
            continue;
        }
        if (!strcmp(command, "sd")) {
            /* set depth limit (remains in force */
            /* until next 'sd n' command)        */
            sscanf(line, "sd %d", &MaxDepth);
            MaxDepth += 2;      /* QS depth */
            continue;
        }
        if (!strcmp(command, "level")) {
            /* normal or blitz time control */
            sec = 0;
            if (sscanf(line, "level %d %d %d",
                       &MaxMoves, &MaxTime, &TimeInc) != 3 &&
                sscanf(line, "level %d %d:%d %d",
                       &MaxMoves, &MaxTime, &sec, &TimeInc) != 4)
                continue;
            MovesLeft = MaxMoves;
            TimeLeft = MaxTime = 60000 * MaxTime + 1000 * sec;
            TimeInc *= 1000;
            continue;
        }
        if (!strcmp(command, "time")) {
            /* set time left on clock */
            sscanf(line, "time %d", &TimeLeft);
            TimeLeft *= 10;     /* centi-sec to ms */
            continue;
        }
        if (!strcmp(command, "otim")) {
            /* opponent's time (not kept, so ignore) */
            continue;
        }
        if (!strcmp(command, "go")) {
            /* set computer to play current side to move */
            Computer = Side;
            MovesLeft = -(GamePtr + (Side == WHITE) >> 1);
            while (MaxMoves > 0 && MovesLeft <= 0)
                MovesLeft += MaxMoves;
            continue;
        }
        if (!strcmp(command, "hint")) {
            Ticks = GetTickCount();
            tlim = 1000;
            D(Side, -I, I, Q, O, S + 1, 6);
            if (K == 0 && L == 0)
                continue;
            printf("Hint: ");
            printf("%c%c%c%c", 'a' + (K & 7), '8' - (K >> 4),
                   'a' + (L & 7), '8' - (L >> 4));
            printf("\n");
            continue;
        }
        if (!strcmp(command, "undo") && (nr = 1) ||
            !strcmp(command, "remove") && (nr = 2)) {
            /* 'take back' moves by replaying game */
            /* from history until desired ply      */
            if (GamePtr < nr)
                continue;
            GamePtr -= nr;
            HistPtr -= nr;      /* erase history boards */
            while (nr-- > 0)
                for (m = 0; m < STATE; m++)
                    HistoryBoards[HistPtr + nr + 1 & 1023][m] = 0;
            InitGame();
            for (nr = 0; nr < GamePtr; nr++) {
                UNPACK_MOVE(GameHistory[nr]);
                D(Side, -I, I, Q, O, 8, 3);
                Side ^= 24;
            }
            continue;
        }
        if (!strcmp(command, "post")) {
            Post = 1;
            continue;
        }
        if (!strcmp(command, "nopost")) {
            Post = 0;
            continue;
        }
        if (!strcmp(command, "edit")) {
            int color = WHITE;

            while (fgets(line, 256, stdin)) {
                m = line[0];
                if (m == '.')
                    break;
                if (m == '#') {
                    for (i = 0; i < 128; i++)
                        b[i & 0x77] = 0;
                    R = 40;
                    Q = 0;
                    O = S;
                    continue;
                }
                if (m == 'c') {
                    color = WHITE + BLACK - color;
                    Q = -Q;
                    continue;
                }
                if ((m == 'P' || m == 'N' || m == 'B' ||
                     m == 'R' || m == 'Q' || m == 'K')
                    && line[1] >= 'a' && line[1] <= 'h'
                    && line[2] >= '1' && line[2] <= '8') {
                    m = line[1] - 16 * line[2] + 799;
                    switch (line[0]) {
                    case 'P':
                        if (color == WHITE)
                            b[m] = (m & 0x70) == 0x60 ? 9 : 41;
                        else
                            b[m] = (m & 0x70) == 0x10 ? 18 : 50;
                        Q += 2 * 37;
                        break;
                    case 'N':
                        b[m] = 3 + color;
                        Q += 7 * 37;
                        R -= 2;
                        break;
                    case 'B':
                        b[m] = 5 + color;
                        Q += 8 * 37;
                        R -= 2;
                        break;
                    case 'R':
                        b[m] = 6 + color + 32;
                        if ((m == 0x00 || m == 0x07)
                            && color == BLACK || (m == 0x70 || m == 0x77)
                            && color == WHITE)
                            b[m] -= 32;
                        Q += 12 * 37;
                        R -= 3;
                        break;
                    case 'Q':
                        b[m] = 7 + color;
                        Q += 23 * 37;
                        R -= 6;
                        break;
                    case 'K':
                        b[m] = 4 + color + 32;
                        if (m == 0x04 && color == BLACK ||
                            m == 0x74 && color == WHITE)
                            b[m] -= 32;
                        break;
                    }
                    continue;
                }
            }
            if (Side != color)
                Q = -Q;
            continue;
        }
        /* command not recognized, assume input move */
        m = line[0] < 'a' | line[0] > 'h' | line[1] < '1' | line[1] > '8' |
            line[2] < 'a' | line[2] > 'h' | line[3] < '1' | line[3] > '8';
        PromPiece = line[4];
        {
            char *c = line;
            K = c[0] - 16 * c[1] + 799;
            L = c[2] - 16 * c[3] + 799;
        }
        if (m)
            /* doesn't have move syntax */
            printf("Error (unknown command): %s\n", command);
        else if (D(Side, -I, I, Q, O, 8, 3) != I) {
            /* did have move syntax, but illegal move */
            printf("Illegal move:%s\n", line);
        } else {                /* legal move, perform it */
            GameHistory[GamePtr++] = PACK_MOVE;
            Side ^= 24;
            CopyBoard(HistPtr = HistPtr + 1 & 1023);
            if (PrintResult(Side))
                Computer = EMPTY;
        }
    }
}


