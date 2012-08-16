#include <stdio.h>
#include "chessengine.h"

int main(int argc, char **argv)
{
    printf("enter a move format letter number letter number\n");
    InitEngine();
    InitGame(PlayerVsAI);

	int gameResult = 0, tookPieceFlag = 0,illegalMoveFlag = 0;
	char resultMove[4];

    for (;;) {
		char cmd[100];
		scanf("%s", cmd);
		PlayerMove(cmd, &gameResult, &tookPieceFlag, &illegalMoveFlag);
		printf("r)%d t)%d i)%d\n", gameResult, tookPieceFlag, illegalMoveFlag);
		if (illegalMoveFlag)
		{
			printf("illegal move.\n");
			continue;
		}
		AIMove(&gameResult, &tookPieceFlag, resultMove);
		printf("r)%d t)%d rm)%s\n", gameResult, tookPieceFlag, resultMove);
	}
	
}