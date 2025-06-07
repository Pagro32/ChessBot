#include <bitset>
#include <iostream>
#include <string>

using chessPiece = unsigned long long;

chessPiece wPawns = 0;
chessPiece wKnight = 0;
chessPiece wBishop = 0;
chessPiece wRock = 0;
chessPiece wQueen = 0;
chessPiece wKing = 0;

chessPiece bPawns = 0;
chessPiece bKnight = 0;
chessPiece bBishop = 0;
chessPiece bRock = 0;
chessPiece bQueen = 0;
chessPiece bKing = 0;

chessPiece usedSquares = 0;
chessPiece usedSquaresB = 0;
chessPiece usedSquaresW = 0;

unsigned long long long1 = 1;  // used for bit manipulation above 32 digits
int enPassent = 128;
int enPassentBot[] = {128, 128, 128, 128, 128, 128, 128, 128};
int undoPiece[] = {128, 128, 128, 128, 128, 128};
bool wlcastle = true, wscastle = true, blcastle = true, bscastle = true;
bool bottom;
bool error = false;
bool checkmate2 = false;
bool checkmate1 = false;
int checkround2 = 0;
// Remis variables
int pawnRemis = 0;
int lastMoves[] = {128, 127, 126, 125, 124, 123, 122, 121};
int director = 0;

bool success = false;  // for evaluation
bool botmove = false;  // testing for bots with undoPieces
bool botpromote = false;
bool botcheckSuccess = false;  // if it failed because of check, still try
int recurse = 4;  // how many recursions should be done before the bot plays a move
int recurseHappend = 0;  // where we're at
int moves = 0;           // only Moves counter

float vPosition[6][64] = {
    {
     -0.5f,  -0.33f, -0.2f,  -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f,  0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.1f,  -0.3f,  -0.15f, 0.25f, 0.25f, 0.1f,  0,      -0.15f,
     -0.2f,  -0.35f, -0.2f,  0.6f,  0.6f,  0.25f, 0.15f,  0,
     -0.2f,  -0.35f, -0.2f,  0.6f,  0.6f,  0.25f, 0.15f,  0,
     -0.1f,  -0.3f,  -0.15f, 0.25f, 0.25f, 0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f,  0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f,  -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Pawns
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.15f, 0.25f, 0.25f, 0.15f, 0,      -0.15f,
     0,      0.15f,  0.25f, 0.15f, 0.15f, 0.25f, 0.15f,  0,
     0,      0.15f,  0.25f, 0.15f, 0.15f, 0.25f, 0.15f,  0,
     -0.15f, 0,      0.15f, 0.25f, 0.25f, 0.15f, 0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Knight
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.1f,  0.1f,  0.1f,  0.1f,  0,      -0.15f,
     0,      0,      0.25f, 0.5f,  0.5f,  0.25f, 0,      0,
     0,      0,      0.25f, 0.5f,  0.5f,  0.25f, 0,      0,
     -0.15f, 0,      0.1f,  0.1f,  0.1f,  0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Bishops
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.1f,  0.25f, 0.25f, 0.1f,  0,      -0.15f,
     0,      0.15f,  0.25f, 0.5f,  0.5f,  0.25f, 0.15f,  0,
     0,      0.15f,  0.25f, 0.5f,  0.5f,  0.25f, 0.15f,  0,
     -0.15f, 0,      0.1f,  0.25f, 0.25f, 0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Rock
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0.15f, 0.15f, -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.1f,  0.14f, 0.14f, 0.1f,  0,      -0.15f,
     0,      0.15f,  0.2f,  0.2f,  0.2f,  0.2f,  0.15f,  0,
     0,      0.15f,  0.2f,  0.2f,  0.2f,  0.2f,  0.15f,  0,
     -0.15f, 0,      0.1f,  0.14f, 0.14f, 0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0.15f, 0.15f, -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Queen
    {
     0.3f,  0.33f,  0.2f,   0.1f,   0.1f,   0.2f,   0.33f,  0.3f,
     0.15f, 0.1f,   0,      0,      0,      0,      0.1f,   0.15f,
     0.1f,  0,      -0.1f,  -0.25f, -0.25f, -0.1f,  0,      0.1f,
     0,     -0.15f, -0.25f, -0.5f,  -0.5f,  -0.25f, -0.15f, 0,
     0,     -0.15f, -0.25f, -0.5f,  -0.5f,  -0.25f, -0.15f, 0,
     0.1f,  0,      -0.1f,  -0.25f, -0.25f, -0.1f,  0,      0.1f,
     0.15f, 0.1f,   0,      0,      0,      0,      0.1f,   0.15f,
     0.3f,  0.33f,  0.2f,   0.1f,   0.1f,   0.2f,   0.33f,  0.3f
    }  // King

};  // values a position
bool endgame = false;
float vendPosition[6][64] = {
    {
     -2.5f, -2.5f, -2.5f, -2.5f, -2.5f, -2.5f, -2.5f, -2.5f, 
     -0.8f, -0.8f, -0.8f, -0.8f, -0.8f, -0.8f, -0.8f, -0.8f, 
     -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 
     -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, -0.3f, 
     0,     0,     0,     0,     0,     0,     0,     0,
     0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  
     1.2f,  1.2f, 1.2f,  1.2f,  1.2f,  1.2f,  1.2f,  1.2f,  
     2.5f,  2.5f,  2.5f,  2.5f, 2.5f,  2.5f,  2.5f,  2.5f
    },  // Pawns
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.15f, 0.25f, 0.25f, 0.15f, 0,      -0.15f,
     0,      0.15f,  0.25f, 0.15f, 0.15f, 0.25f, 0.15f,  0,
     0,      0.15f,  0.25f, 0.15f, 0.15f, 0.25f, 0.15f,  0,
     -0.15f, 0,      0.15f, 0.25f, 0.25f, 0.15f, 0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Knight
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.1f,  0.1f,  0.1f,  0.1f,  0,      -0.15f,
     0,      0.2f,   0.25f, 0.5f,  0.5f,  0.25f, 0.2f,   0,
     0,      0.2f,   0.25f, 0.5f,  0.5f,  0.25f, 0.2f,   0,
     -0.15f, 0,      0.1f,  0.1f,  0.1f,  0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Bishops
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.1f,  0.25f, 0.25f, 0.1f,  0,      -0.15f,
     0,      0.15f,  0.25f, 0.5f,  0.5f,  0.25f, 0.15f,  0,
     0,      0.15f,  0.25f, 0.5f,  0.5f,  0.25f, 0.15f,  0,
     -0.15f, 0,      0.1f,  0.25f, 0.25f, 0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Rock
    {
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.15f, 0,      0.1f,  0.25f, 0.25f, 0.1f,  0,      -0.15f,
     0,      0.15f,  0.25f, 0.25f, 0.25f, 0.25f, 0.15f,  0,
     0,      0.15f,  0.25f, 0.5f,  0.25f, 0.25f, 0.15f,  0,
     -0.15f, 0,      0.1f,  0.25f, 0.25f, 0.1f,  0,      -0.15f,
     -0.33f, -0.2f,  -0.1f, 0,     0,     -0.1f, -0.2f,  -0.33f,
     -0.5f,  -0.33f, -0.2f, -0.1f, -0.1f, -0.2f, -0.33f, -0.5f
    },  // Queen
    {
     -0.3f, -0.33f, -0.2f,  -0.1f, -0.1f, -0.2f,  -0.33f, -0.3f,  
     -0.15f, -0.1f, 0,     0,      0,      0,     -0.1f, -0.15f, 
     -0.1f,  0,      0.1f,   0.25f, 0.25f, 0.1f,   0,      -0.1f, 
     0,      0.15f, 0.25f, 0.33f,  0.33f,  0.25f,  0.15f,  0,      
     0,      0.15f, 0.25f, 0.33f,  0.33f,  0.25f,  0.15f,  0,
     -0.1f, 0,      0.1f,   0.25f, 0.25f, 0.1f,   0,      -0.1f,  
     -0.15f, -0.1f, 0,     0,      0,      0,     -0.1f, -0.15f, 
     -0.3f,  -0.33f, -0.2f,  -0.1f, -0.1f, -0.2f,  -0.33f, -0.3f
    }  // King
};  // values a position in endgame
float AdditionWhite = 0;
float AdditionBlack = 0;
bool firstMove = true;

// pyhton Exchange
bool python = false;
bool betterBot = true;
FILE* fp;
int tempb = 0;

// converter from Dezimal to inverted Binary
std::string convertToBinary(chessPiece var) {
  std::string temp = "";
  while (var != 0) {
    temp += (var % 2 == 0) ? "0" : "1";  // if even: add 0; uneven; add 1

    var = (var != 1) ? var / 2
                     : 0;  // if var is above 1: devide by 2; else: end loop
  }
  return temp;
}

// finds Position of King or the first instance of another piece
int findKP(chessPiece piece) {
  std::bitset<64> bitpiece = piece;

  for (int i = 0; i < 64; i++) {
    if (bitpiece.test(i)) return i;
  }
  return 128;
}

// basically a Debugger that shows positions of pieceType
void showPieces(chessPiece pieces) {
  std::string binaryPieces = convertToBinary(pieces);  // Pieces in binary

  size_t bitlength = binaryPieces.length();  // bitlength
  std::cout << "\n\n\n";
  int temp2 = 0;  // to go from 56-64 and not 64,63,...
  int counter = 0;

  // print not initialized zeros
  for (int i = 0; i < (64 - bitlength); i++) {
    counter++;
    std::cout << " " << 0;
    if (counter % 8 == 0) std::cout << "\n";
  }

  // print the rest
  for (int i = 0; i < (bitlength); i++) {
    counter++;
    size_t temp = bitlength - temp2;
    temp2 += 1;
    std::cout << " " << binaryPieces.substr(temp - 1, 1);
    if (counter % 8 == 0) {
      std::cout << "\n";
    }
  }
  std::cout << "\n";
}
int checkPosition(int pos);
void showPiecesNum() {
  std::cout << "\n";
  for (int i = 0; i < 64; i++) {
    std::cout << checkPosition(63 - i) << " ";
    if (i % 8 == 7) std::cout << "\n";
  }
  std::cout << "\n\n";
}
void updateBoard() {
  usedSquaresW = wPawns | wKnight | wRock | wBishop | wQueen | wKing;
  usedSquaresB = bPawns | bKnight | bRock | bBishop | bQueen | bKing;
  usedSquares = usedSquaresW | usedSquaresB;
}

// normal start Position innit
void startPosition() {
  bottom = true;  // true = white is at bottom

  // asign pieces at the start
  for (int n = 8; n < 16; n++) {
    wPawns |= (long1 << n);
    AdditionWhite += vPosition[0][n];
  }
  for (int n = 48; n < 56; n++) {
    bPawns |= (long1 << n);
    AdditionBlack += vPosition[0][n];
  }
  wKnight |= (long1 << 1);
  AdditionWhite += vPosition[1][1];
  wKnight |= (long1 << 6);
  AdditionWhite += vPosition[1][6];
  bKnight |= (long1 << 57);
  AdditionBlack += vPosition[1][57];
  bKnight |= (long1 << 62);
  AdditionBlack += vPosition[1][62];

  wBishop |= (long1 << 2);
  AdditionWhite += vPosition[2][2];
  wBishop |= (long1 << 5);
  AdditionWhite += vPosition[2][5];
  bBishop |= (long1 << 58);
  AdditionBlack += vPosition[2][58];
  bBishop |= (long1 << 61);
  AdditionBlack += vPosition[2][61];

  wRock |= (long1 << 0);
  AdditionWhite += vPosition[3][0];
  wRock |= (long1 << 7);
  AdditionWhite += vPosition[3][7];
  bRock |= (long1 << 56);
  AdditionBlack += vPosition[3][56];
  bRock |= (long1 << 63);
  AdditionBlack += vPosition[3][63];

  wQueen |= (long1 << 4);
  AdditionWhite += vPosition[4][4];
  bQueen |= (long1 << 60);
  AdditionBlack += vPosition[4][60];

  wKing |= (long1 << 3);
  AdditionWhite += vPosition[5][3];
  bKing |= (long1 << 59);
  AdditionBlack += vPosition[5][59];

  updateBoard();
}

int checkPosition(chessPiece position) {
  if ((position & usedSquaresW) != 0) {
    if ((position & wPawns) != 0) {
      return 1;
    } else if ((position & wKnight) != 0) {
      return 2;
    } else if ((position & wBishop) != 0) {
      return 3;
    } else if ((position & wRock) != 0) {
      return 4;
    } else if ((position & wQueen) != 0) {
      return 5;
    } else
      return 6;
  } else if ((position & usedSquaresB) != 0) {
    if ((position & bPawns) != 0) {
      return 1;
    } else if ((position & bKnight) != 0) {
      return 2;
    } else if ((position & bBishop) != 0) {
      return 3;
    } else if ((position & bRock) != 0) {
      return 4;
    } else if ((position & bQueen) != 0) {
      return 5;
    } else
      return 6;
  } else
    return 0;
}
int checkPosition(int pos) {
  unsigned long long position = 0;
  position |= (long1 << pos);

  if ((position & usedSquaresW) != 0) {
    if ((position & wPawns) != 0) {
      return 1;
    } else if ((position & wKnight) != 0) {
      return 2;
    } else if ((position & wBishop) != 0) {
      return 3;
    } else if ((position & wRock) != 0) {
      return 4;
    } else if ((position & wQueen) != 0) {
      return 5;
    } else
      return 6;
  } else if ((position & usedSquaresB) != 0) {
    if ((position & bPawns) != 0) {
      return 1;
    } else if ((position & bKnight) != 0) {
      return 2;
    } else if ((position & bBishop) != 0) {
      return 3;
    } else if ((position & bRock) != 0) {
      return 4;
    } else if ((position & bQueen) != 0) {
      return 5;
    } else
      return 6;
  } else
    return 0;
}
int checkPositionColor(int pos) {
  unsigned long long position = 0;
  position |= (long1 << pos);

  if ((position & usedSquaresW) != 0) {
    if ((position & wPawns) != 0) {
      return 1;
    } else if ((position & wKnight) != 0) {
      return 2;
    } else if ((position & wBishop) != 0) {
      return 3;
    } else if ((position & wRock) != 0) {
      return 4;
    } else if ((position & wQueen) != 0) {
      return 5;
    } else
      return 6;
  } else if ((position & usedSquaresB) != 0) {
    if ((position & bPawns) != 0) {
      return -1;
    } else if ((position & bKnight) != 0) {
      return -2;
    } else if ((position & bBishop) != 0) {
      return -3;
    } else if ((position & bRock) != 0) {
      return -4;
    } else if ((position & bQueen) != 0) {
      return -5;
    } else
      return -6;
  } else
    return 0;
}

// unpromote Pieces
void unpromote(int sP, int eP, bool color, int unpromote) {
  int piece = checkPosition(eP);
  if (color)  // white AntiPromotion
  {
    switch (piece) {
      case 2:
        wKnight &= ~(long1 << eP);
        break;
      case 3:
        wBishop &= ~(long1 << eP);
        break;
      case 4:
        wRock &= ~(long1 << eP);
        break;
      case 5:
        wQueen &= ~(long1 << eP);
        break;
    }
    AdditionWhite -= vPosition[piece - 1][eP];
  } else  // blackAntiPromotion
  {
    switch (piece) {
      case 2:
        bKnight &= ~(long1 << eP);
        break;
      case 3:
        bBishop &= ~(long1 << eP);
        break;
      case 4:
        bRock &= ~(long1 << eP);
        break;
      case 5:
        bQueen &= ~(long1 << eP);
        break;
    }
    AdditionBlack -= vPosition[piece - 1][eP];
  }
}
// promote Pieces (also accessable from bot)
void promote(int sP, int eP, bool color, int promote) {
  if (color)  // whitePromotion
  {
    wPawns &= ~(long1 << eP);
    AdditionWhite -= vPosition[0][eP];
    switch (promote) {
      case 2:
        wKnight |= (long1 << eP);
        break;
      case 3:
        wBishop |= (long1 << eP);
        break;
      case 4:
        wRock |= (long1 << eP);
        break;
      case 5:
        wQueen |= (long1 << eP);
        break;
    }
    AdditionWhite += vPosition[promote - 1][eP];
  } else  // blackPromotion
  {
    bPawns &= ~(long1 << eP);
    AdditionBlack -= vPosition[0][eP];
    switch (promote) {
      case 2:
        bKnight |= (long1 << eP);
        break;
      case 3:
        bBishop |= (long1 << eP);
        break;
      case 4:
        bRock |= (long1 << eP);
        break;
      case 5:
        bQueen |= (long1 << eP);
        break;
    }
    AdditionBlack += vPosition[promote - 1][eP];
  }
}

void undoMovePiece(int piece, int sP, int eP, bool color) {
  if (recurseHappend < recurse)
    enPassentBot[recurseHappend + 1] =
        128;  // if recurse 3 gets stopped before recurse 4 begins, enPassent[4]
              // goes over to next  recurse 3 going to 4
  if (recurseHappend == recurse) enPassentBot[5] = 128;

  if (color)  // white
  {
    switch (piece) {  // removie piece from position and add at new position
      case 1:
        wPawns &= ~(long1 << eP);
        wPawns |= (long1 << sP);
        break;
      case 2:
        wKnight &= ~(long1 << eP);
        wKnight |= (long1 << sP);
        break;
      case 3:
        wBishop &= ~(long1 << eP);
        wBishop |= (long1 << sP);
        break;
      case 4:
        wRock &= ~(long1 << eP);
        wRock |= (long1 << sP);
        break;
      case 5:
        wQueen &= ~(long1 << eP);
        wQueen |= (long1 << sP);
        break;
      case 6:
        wKing &= ~(long1 << eP);
        wKing |= (long1 << sP);
        break;
    }
    if (!((eP < 8 || eP > 55) && (piece == 1))) {
      AdditionWhite -= vPosition[piece - 1][eP];
    }
    AdditionWhite += vPosition[piece - 1][sP];

    if (undoPiece[recurseHappend - 1] < 7)  // undo capture: add piece of enemy
    {
      // undo captrue
      switch (undoPiece[recurseHappend - 1]) {
        case 1:
          bPawns |= (long1 << eP);
          break;
        case 2:
          bKnight |= (long1 << eP);
          break;
        case 3:
          bBishop |= (long1 << eP);
          break;
        case 4:
          bRock |= (long1 << eP);
          break;
        case 5:
          bQueen |= (long1 << eP);
          break;
        case 6:
          bKing |= (long1 << eP);
          break;
      }
      AdditionBlack += vPosition[undoPiece[recurseHappend - 1] - 1][eP];
    }

    if (piece == 6 && sP == 3) {
      // castle rock undo
      if (eP == 1) {
        wRock &= ~(long1 << 2);
        wRock |= (long1 << 0);
        AdditionWhite -= vPosition[3][2];
        AdditionWhite += vPosition[3][0];
      }
      if (eP == 5) {
        wRock &= ~(long1 << 4);
        wRock |= (long1 << 7);
        AdditionWhite -= vPosition[3][4];
        AdditionWhite += vPosition[3][7];
      }
    }

  } else if (!color)  // black
  {
    switch (piece) {  // remove piece from position and add at new position
      case 1:
        bPawns &= ~(long1 << eP);
        bPawns |= (long1 << sP);
        break;
      case 2:
        bKnight &= ~(long1 << eP);
        bKnight |= (long1 << sP);
        break;
      case 3:
        bBishop &= ~(long1 << eP);
        bBishop |= (long1 << sP);
        break;
      case 4:
        bRock &= ~(long1 << eP);
        bRock |= (long1 << sP);
        break;
      case 5:
        bQueen &= ~(long1 << eP);
        bQueen |= (long1 << sP);
        break;
      case 6:
        bKing &= ~(long1 << eP);
        bKing |= (long1 << sP);
        break;
    }
    if (!((eP < 8 || eP > 55) && (piece == 1))) {
      AdditionBlack -= vPosition[piece - 1][eP];
    }
    AdditionBlack += vPosition[piece - 1][sP];

    if (undoPiece[recurseHappend - 1] < 7)  // undo capture: add piece of enemy
    {
      // undo capture
      switch (undoPiece[recurseHappend - 1]) {
        case 1:
          wPawns |= (long1 << eP);
          break;
        case 2:
          wKnight |= (long1 << eP);
          break;
        case 3:
          wBishop |= (long1 << eP);
          break;
        case 4:
          wRock |= (long1 << eP);
          break;
        case 5:
          wQueen |= (long1 << eP);
          break;
        case 6:
          wKing |= (long1 << eP);
          break;
      }
      AdditionWhite += vPosition[undoPiece[recurseHappend - 1] - 1][eP];
    }

    if (piece == 6 && sP == 59) {
      // castle rock undo
      if (eP == 57) {
        if (checkPosition(58) == 4 && checkPosition(56) != 4) {
          bRock &= ~(long1 << 58);
          bRock |= (long1 << 56);
          AdditionBlack -= vPosition[3][58];
          AdditionBlack += vPosition[3][56];
        } else {
          showPiecesNum();
        }
      }
      if (eP == 61) {
        if (checkPosition(60) == 4 && checkPosition(63) != 4) {
          bRock &= ~(long1 << 60);
          bRock |= (long1 << 63);
          AdditionBlack -= vPosition[3][60];
          AdditionBlack += vPosition[3][63];
        } else {
          showPiecesNum();
        }
      }
    }
  }

  if (piece == 1) {
    // pawn promoting
    if (eP < 8 || eP > 55) {
      unpromote(sP, eP, color, undoPiece[recurseHappend - 1]);
    }
    // enPassent
    if (((eP == enPassent) || (eP == enPassentBot[recurseHappend])) &&
        (eP - sP) % 8 != 0 &&
        ((sP > 23 && sP < 32 && !color) || (sP > 31 && sP < 40 && color)) &&
        undoPiece[recurseHappend - 1] == 128) {
      if (color) {
        if (botmove && recurseHappend > 1) {
          bPawns |= (long1 << (enPassentBot[recurseHappend] - 8));
          AdditionBlack += vPosition[0][enPassentBot[recurseHappend] - 8];
        } else {
          bPawns |= (long1 << (enPassent - 8));
          AdditionBlack += vPosition[0][enPassent - 8];
        }
      } else {
        if (botmove && recurseHappend > 1) {
          wPawns |= (long1 << (enPassentBot[recurseHappend] + 8));
          AdditionWhite += vPosition[0][enPassentBot[recurseHappend] + 8];
        } else {
          wPawns |= (long1 << (enPassent + 8));
          if (enPassent + 8 <= 63) {
            AdditionWhite += vPosition[0][enPassent + 8];
          }
        }
      }
    }
  }

  undoPiece[recurseHappend - 1] = 128;

  updateBoard();
}

float eva();
void movePiece(int piece, int sP, int eP,
               bool color)  // sP = startPosition    eP = endPosition
// color = true -> white         //Positions in 0,1,...,63
{
  undoPiece[recurseHappend - 1] = 128;

  unsigned long long endP = 0;
  endP |= (long1 << eP);

  if (sP == eP) {
    return;
  }  // not possible to move to same square

  if (color)  // white
  {
    if ((endP & usedSquaresW) != 0)  // stop from taking own peaces
    {
      return;
    }

    if ((endP & usedSquaresB) != 0)  // capture: remove piece of enemy
    {
      undoPiece[recurseHappend - 1] = checkPosition(endP);
      switch (undoPiece[recurseHappend - 1]) {
        case 1:
          bPawns &= ~(long1 << eP);
          break;
        case 2:
          bKnight &= ~(long1 << eP);
          break;
        case 3:
          bBishop &= ~(long1 << eP);
          break;
        case 4:
          bRock &= ~(long1 << eP);
          break;
        case 5:
          bQueen &= ~(long1 << eP);
          break;
        case 6:
          bKing &= ~(long1 << eP);
          break;
      }
      if (undoPiece[recurseHappend - 1] != 0) {
        AdditionBlack -= vPosition[undoPiece[recurseHappend - 1] - 1][eP];
      }
    }

    switch (piece) {  // removie piece from position and add at new position
      case 1:
        wPawns &= ~(long1 << sP);
        wPawns |= (long1 << eP);
        break;
      case 2:
        wKnight &= ~(long1 << sP);
        wKnight |= (long1 << eP);
        break;
      case 3:
        wBishop &= ~(long1 << sP);
        wBishop |= (long1 << eP);
        break;
      case 4:
        wRock &= ~(long1 << sP);
        wRock |= (long1 << eP);
        break;
      case 5:
        wQueen &= ~(long1 << sP);
        wQueen |= (long1 << eP);
        break;
      case 6:
        wKing &= ~(long1 << sP);
        wKing |= (long1 << eP);
        break;
    }
    AdditionWhite -= vPosition[piece - 1][sP];
    AdditionWhite += vPosition[piece - 1][eP];
  } else if (!color)  // black
  {
    if ((endP & usedSquaresB) != 0)  // stop from taking own peaces
    {
      return;
    } else if ((endP & usedSquaresW) != 0)  // capture: remove piece of enemy
    {
      undoPiece[recurseHappend - 1] = checkPosition(endP);
      switch (undoPiece[recurseHappend - 1]) {
        case 1:
          wPawns &= ~(long1 << eP);
          break;
        case 2:
          wKnight &= ~(long1 << eP);
          break;
        case 3:
          wBishop &= ~(long1 << eP);
          break;
        case 4:
          wRock &= ~(long1 << eP);
          break;
        case 5:
          wQueen &= ~(long1 << eP);
          break;
        case 6:
          wKing &= ~(long1 << eP);
          break;
      }
      AdditionWhite -= vPosition[undoPiece[recurseHappend - 1] - 1][eP];
    }

    switch (piece) {  // remove piece from position and add at new position
      case 1:
        bPawns &= ~(long1 << sP);
        bPawns |= (long1 << eP);
        break;
      case 2:
        bKnight &= ~(long1 << sP);
        bKnight |= (long1 << eP);
        break;
      case 3:
        bBishop &= ~(long1 << sP);
        bBishop |= (long1 << eP);
        break;
      case 4:
        bRock &= ~(long1 << sP);
        bRock |= (long1 << eP);
        break;
      case 5:
        bQueen &= ~(long1 << sP);
        bQueen |= (long1 << eP);
        break;
      case 6:
        bKing &= ~(long1 << sP);
        bKing |= (long1 << eP);
        break;
    }
    AdditionBlack -= vPosition[piece - 1][sP];
    AdditionBlack += vPosition[piece - 1][eP];
  }

  moves++;

  if (piece == 1) {
    // pawn promoting
    if (eP < 8 || eP > 55) {
      int b = 0;
      if (!botpromote && !python) {
        std::cout << "Select a promotion piece (2-5) ";
        std::cin >> b;
      } else
        b = 5;
      promote(sP, eP, color, b);
    }
  }

  updateBoard();
  // check if King is not in check
  // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if (color) {
    int counter = 0;
    unsigned long long checking = 0;
    int currentKing = 0;
    if (piece != 6)
      currentKing = convertToBinary(wKing).length() -
                    1;  // if moving King checked for end Position or he could
                        // never move out of check
    else {
      currentKing = eP;
      usedSquaresW &= ~(long1 << sP);  // hiding King for calculation
    }

    int tempEnd = currentKing;
    // check for opponent King
    int oKing = convertToBinary(bKing).length() - 1;
    if ((oKing == currentKing + 1) || (oKing == currentKing - 1) ||
        (oKing == currentKing + 8) || (oKing == currentKing - 8) ||
        (oKing == currentKing + 9) || (oKing == currentKing - 9) ||
        (oKing == currentKing + 7) || (oKing == currentKing - 7)) {
      if ((oKing % 8) - (currentKing & 8) < 2 ||
          (oKing % 8) - (currentKing & 8) > -2) {
        error = true;
      }
    }
    // check for pawns

    checking |= (long1 << (currentKing + 7));
    if ((checking & bPawns) != 0 && currentKing % 8 != 0) {
      error = true;
    }
    checking = 0;
    checking |= (long1 << (currentKing + 9));
    if ((checking & bPawns) != 0 && currentKing % 8 != 7) {
      error = true;
    }
    checking = 0;

    // check rock/queen
    int temp = (63 - currentKing) / 8;
    for (int i = 0; i < temp;
         i++)  // changed from =< to < (cause it func. with 0)
    {
      checking |= (long1 << (currentKing + 8 * (i + 1)));  // upwards
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }

    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentKing - 8 * (i + 1)));  // downwards
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    temp = currentKing % 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentKing - (i + 1)));  // right
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentKing + (i + 1)));  // left
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }
    checking = 0;

    // check pawn,bishop,queen
    while (tempEnd % 8 != 0 && tempEnd + 7 <= 63) {
      checking |= (long1 << (tempEnd + 7));
      tempEnd += 7;
      if ((checking & usedSquaresW) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          error = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentKing;
    while (tempEnd % 8 != 7 && tempEnd + 9 <= 63) {
      checking |= (long1 << (tempEnd + 9));
      tempEnd += 9;
      if ((checking & usedSquaresW) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          error = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentKing;
    while (tempEnd % 8 != 7 && tempEnd - 7 >= 0) {
      checking |= (long1 << (tempEnd - 7));
      tempEnd -= 7;
      if ((checking & usedSquaresW) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          error = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;
    tempEnd = currentKing;
    while (tempEnd % 8 != 0 && tempEnd - 9 >= 0) {
      checking |= (long1 << (tempEnd - 9));
      tempEnd -= 9;
      if ((checking & usedSquaresW) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          error = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;

    tempEnd = currentKing + 6;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 6;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing + 10;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 10;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing + 15;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 15;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing + 17;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 17;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    if ((checking & bKnight) != 0) {
      error = true;
    }
    if (piece == 6)
      usedSquaresW |= (long1 << sP);  // undo hiding King for calculation
  } else {
    int counter = 0;
    unsigned long long checking = 0;
    int currentKing = 0;
    if (piece != 6)
      currentKing = convertToBinary(bKing).length() -
                    1;  // if moving King checked for end Position or he could
                        // never move out of check
    else {
      currentKing = eP;
      usedSquaresB &= ~(long1 << sP);  // hiding King for calculation
    }

    int tempEnd = currentKing;
    // check for opponent King
    int oKing = convertToBinary(wKing).length() - 1;
    if ((oKing == currentKing + 1) || (oKing == currentKing - 1) ||
        (oKing == currentKing + 8) || (oKing == currentKing - 8) ||
        (oKing == currentKing + 9) || (oKing == currentKing - 9) ||
        (oKing == currentKing + 7) || (oKing == currentKing - 7)) {
      if ((oKing % 8) - (currentKing & 8) < 2 ||
          (oKing % 8) - (currentKing & 8) > -2) {
        error = true;
      }
    }
    // check for pawns

    checking |= (long1 << (currentKing - 7));
    if ((checking & wPawns) != 0 && currentKing % 8 != 7) {
      error = true;
    }
    checking = 0;
    checking |= (long1 << (currentKing - 9));
    if ((checking & wPawns) != 0 && currentKing % 8 != 0) {
      error = true;
    }
    checking = 0;

    // check rock/queen
    int temp = (63 - currentKing) / 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentKing + 8 * (i + 1)));  // upwards
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }

    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentKing - 8 * (i + 1)));  // backwards
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    temp = currentKing % 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentKing - (i + 1)));  // right
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentKing + (i + 1)));  // left
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          error = true;
        } else
          i = 100;
      }
    }
    checking = 0;

    // check pawn,bishop,queen
    while (tempEnd % 8 != 0 && tempEnd + 7 <= 63) {
      checking |= (long1 << (tempEnd + 7));
      tempEnd += 7;
      if ((checking & usedSquaresB) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          error = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentKing;
    while (tempEnd % 8 != 7 && tempEnd + 9 <= 63) {
      checking |= (long1 << (tempEnd + 9));
      tempEnd += 9;
      if ((checking & usedSquaresB) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          error = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentKing;
    while (tempEnd % 8 != 7 && tempEnd - 7 >= 0) {
      checking |= (long1 << (tempEnd - 7));
      tempEnd -= 7;
      if ((checking & usedSquaresB) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          error = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;
    tempEnd = currentKing;
    while (tempEnd % 8 != 0 && tempEnd - 9 >= 0) {
      checking |= (long1 << (tempEnd - 9));
      tempEnd -= 9;
      if ((checking & usedSquaresB) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          error = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;

    tempEnd = currentKing + 6;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 6;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing + 10;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 10;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing + 15;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 15;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing + 17;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentKing - 17;
    if (currentKing % 8 - tempEnd % 8 < 3 && currentKing % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    if ((checking & wKnight) != 0) {
      error = true;
    }
    if (piece == 6)
      usedSquaresB |= (long1 << sP);  // undo hiding King for calculation
  }
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  if (error) {
    error = false;
    undoMovePiece(piece, sP, eP, color);
    botcheckSuccess = true;
    return;
  }
  // continues here if no illegal King move has been made ---v
  if (!botmove) undoPiece[recurseHappend - 1] = 128;
  success = true;

  if (!botmove) {
    // disable castling if piece moved
    if (wscastle || wlcastle || bscastle || blcastle) {
      if (piece == 6) {
        if (color) {
          wlcastle = false;
          wscastle = false;
        } else {
          blcastle = false;
          bscastle = false;
        }
      }
      if (piece == 4) {
        switch (sP) {
          case 0:
            wscastle = false;
            break;
          case 7:
            wlcastle = false;
            break;
          case 56:
            bscastle = false;
            break;
          case 63:
            blcastle = false;
            break;
        }
      }
    }
  }

  updateBoard();
  // Remis rules
  if (!botmove) {
    if (piece == 1)
      pawnRemis = 0;
    else
      pawnRemis++;
    lastMoves[director] = piece * 100 + eP;
    director++;
    if (director == 8) director = 0;
  }
}
bool isAttacked(bool, int);

void gameRules(int piece, int startPosition, int endPosition,
               bool color)  // implementing game rules
{
  if (endPosition < 0 || endPosition > 63) {
    return;
  }  // out of bounds detecter
  unsigned long long leP = 0;
  leP |= (long1 << endPosition);             // binary endPosition
  bool usedEP = ((leP & usedSquares) != 0);  // true = endPosition is occupied

  unsigned long long lsP = 0;
  lsP |= (long1 << startPosition);
  if ((lsP & usedSquares) == 0) {
    return;
  }           // no piece there
  if (color)  // check if piece type and color is correct
  {
    switch (piece) {
      case 1:
        if ((wPawns & lsP) == 0) {
          return;
        }
        break;
      case 2:
        if ((wKnight & lsP) == 0) {
          return;
        }
        break;
      case 3:
        if ((wBishop & lsP) == 0) {
          return;
        }
        break;
      case 4:
        if ((wRock & lsP) == 0) {
          return;
        }
        break;
      case 5:
        if ((wQueen & lsP) == 0) {
          return;
        }
        break;
      case 6:
        if ((wKing & lsP) == 0) {
          return;
        }
        break;
    }
  } else {
    switch (piece) {
      case 1:
        if ((bPawns & lsP) == 0) {
          return;
        }
        break;
      case 2:
        if ((bKnight & lsP) == 0) {
          return;
        }
        break;
      case 3:
        if ((bBishop & lsP) == 0) {
          return;
        }
        break;
      case 4:
        if ((bRock & lsP) == 0) {
          return;
        }
        break;
      case 5:
        if ((bQueen & lsP) == 0) {
          return;
        }
        break;
      case 6:
        if ((bKing & lsP) == 0) {
          return;
        }
        break;
    }
  }
  // end of checking

  // Pawns
  if (piece == 1) {
    int diffPosition = endPosition - startPosition;
    int bottomColor = (color) ? 1 : -1;
    if (diffPosition == 16 || diffPosition == -16) {
      if ((startPosition > 7 && startPosition < 16) ||
          (startPosition > 47 && startPosition < 56)) {
        unsigned long long mP = 0;
        mP |= (long1 << (startPosition + 8 * bottomColor));
        if (!usedEP && (mP & usedSquares) == 0) {
          if (botmove) {
            enPassentBot[recurseHappend + 1] = startPosition + 8 * bottomColor;
          } else
            enPassent = startPosition + 8 * bottomColor;
          movePiece(piece, startPosition, endPosition, color);
          if (!success) {
            if (botmove) {
              enPassentBot[recurseHappend + 1] = 128;
            } else
              enPassent = 128;
          }
          return;
        }
      }
    }
    if ((endPosition % 8 == 7 && startPosition % 8 == 0) ||
        (endPosition % 8 == 0 && startPosition % 8 == 7)) {
      return;
    }
    if (((endPosition == enPassent && recurseHappend < 2) ||
         (endPosition == enPassentBot[recurseHappend] && recurseHappend > 1)) &&
        (diffPosition == 7 * bottomColor || diffPosition == 9 * bottomColor) &&
        startPosition > 23 && startPosition < 40 &&
        (endPosition > 39 || endPosition < 24)) {
      if (color) {
        if (botmove && recurseHappend > 1) {
          bPawns &=
              ~(long1 << (enPassentBot[recurseHappend] - 8 * bottomColor));
          AdditionBlack -=
              vPosition[0][enPassentBot[recurseHappend] - 8 * bottomColor];
        } else {
          bPawns &= ~(long1 << (enPassent - 8 * bottomColor));
          AdditionBlack -= vPosition[0][enPassent - 8 * bottomColor];
        }
      } else {
        if (botmove && recurseHappend > 1) {
          wPawns &=
              ~(long1 << (enPassentBot[recurseHappend] - 8 * bottomColor));
          AdditionWhite -=
              vPosition[0][enPassentBot[recurseHappend] - 8 * bottomColor];
        } else {
          wPawns &= ~(long1 << (enPassent - 8 * bottomColor));
          AdditionWhite -= vPosition[0][enPassent - 8 * bottomColor];
        }
      }
      movePiece(piece, startPosition, endPosition, color);
      return;
    }

    if (((endPosition == startPosition + 8 * bottomColor) && !usedEP) ||
        ((endPosition == startPosition + (8 - bottomColor) * bottomColor) &&
         usedEP && (startPosition % 8 != 0)) ||
        ((endPosition == startPosition + (8 + bottomColor) * bottomColor) &&
         usedEP && (startPosition % 8 != 7))) {
      movePiece(piece, startPosition, endPosition, color);
      return;
    }
  }

  // Knights
  if (piece == 2) {
    if (((endPosition == startPosition + 15 && (startPosition % 8 != 0)) ||
         (endPosition == startPosition + 17 && (startPosition % 8 != 7)) ||
         (endPosition == startPosition - 15 && (startPosition % 8 != 7)) ||
         endPosition == startPosition - 17 && (startPosition % 8 != 0) ||
         (endPosition == startPosition - 10 && (startPosition % 8 > 1)) ||
         (endPosition == startPosition + 10 && (startPosition % 8 < 6)) ||
         (endPosition == startPosition + 6 && (startPosition % 8 > 1)) ||
         (endPosition == startPosition - 6 && (startPosition % 8 < 6)))) {
      movePiece(piece, startPosition, endPosition, color);
      return;
    }
  }

  // Bishop
  if (piece == 3) {
    int diffPosition = endPosition - startPosition;
    unsigned long long lcP = 0;  // current Position

    int num = 0;
    int lr = 0;  // declares left or right border
    if (diffPosition % 9 == 0) {
      num = 9;
      lr = 1;
    } else if (diffPosition % 7 == 0) {
      num = 7;
    } else {
      return;
    }

    if (diffPosition < 0) {
      num = -num;
    }  // decide for direction of Bishop
    lr = (lr == 1 && diffPosition <= 0 || lr == 0 && diffPosition >= 0) ? 0 : 7;
    if ((lr == 7 && startPosition % 8 == 7) ||
        (lr == 0 && startPosition % 8 == 0)) {
      return;
    }  // if at edge and trys to go beyond

    // check for borders and peices in the way
    for (int i = 1; i <= (diffPosition / num); i++) {
      lcP |= (long1 << (startPosition + num * i));
      if ((startPosition + num * i) % 8 == lr || ((lcP & usedSquares) != 0)) {
        if (startPosition + num * i != endPosition) {
          return;
        }
      }
    }
    movePiece(piece, startPosition, endPosition, color);
    return;
  }
  // Rock
  if (piece == 4) {
    int diffPosition = endPosition - startPosition;
    unsigned long long lcP = 0;  // current Position
    int num = 0;
    bool leftRight = true;

    if (diffPosition < 8 && diffPosition > -8) {
      num = (diffPosition >= 0) ? 1 : -1;
    } else if (diffPosition % 8 == 0) {
      num = (diffPosition >= 0) ? 8 : -8;
      leftRight = false;
    } else {
      return;
    }  // ^--- decide direction

    if ((num == 1 && startPosition % 8 == 7) ||
        (num == -1 && startPosition % 8 == 0)) {
      return;
    }  // if at edge and trys to go beyond

    // check for borders and pieces in the way
    for (int i = 1; i <= (diffPosition / num); i++) {
      lcP |= (long1 << (startPosition + num * i));
      if (((startPosition + num * i) % 8 == 7 && leftRight) ||
          ((lcP & usedSquares) != 0) ||
          ((startPosition + num * i) % 8 == 0 && leftRight)) {
        if (startPosition + num * i != endPosition) {
          return;
        }
      }
    }
    movePiece(piece, startPosition, endPosition, color);
    return;
  }
  // Queen
  if (piece == 5) {
    int diffPosition = endPosition - startPosition;
    if (diffPosition == -1 && startPosition % 8 != 0 ||
        diffPosition == 1 && startPosition % 8 != 7 || diffPosition == 8 ||
        diffPosition == -8 || diffPosition == -7 && startPosition % 8 != 7 ||
        diffPosition == 7 && startPosition % 8 != 0 ||
        diffPosition == 9 && startPosition % 8 != 7 ||
        diffPosition == -9 && startPosition % 8 != 0) {
      movePiece(piece, startPosition, endPosition, color);
      return;
    }

    bool borderRock = true;
    if (diffPosition == 7 && startPosition % 8 == 0 ||
        diffPosition == -7 && startPosition % 8 == 7)
      borderRock = false;  // modification to allow rock go border to border

    //----------
    unsigned long long lcP = 0;  // current Position
    int num = 0;
    int lr = 0;  // declares left or right border

    if (diffPosition % 9 == 0) {
      num = 9;
      lr = 1;
    } else if (diffPosition % 7 == 0 && borderRock && diffPosition != 56 &&
               diffPosition != -56) {
      num = 7;
    }  //----------- Bishop part
    else {
      // ---Rock movement injection

      bool leftRight = true;
      bool jumpOver = true;

      if (diffPosition < 8 && diffPosition > -8) {
        num = (diffPosition >= 0) ? 1 : -1;
      } else if (diffPosition % 8 == 0) {
        num = (diffPosition >= 0) ? 8 : -8;
        leftRight = false;
      } else {
        jumpOver = false;
      }  // ^--- decide direction

      if (jumpOver) {
        if ((num == 1 && startPosition % 8 == 7) ||
            (num == -1 && startPosition % 8 == 0)) {
          return;
        }  // if at edge and trys to go beyond
        else {
          // check for borders and pieces in the way
          for (int i = 1; i <= (diffPosition / num); i++) {
            lcP |= (long1 << (startPosition + num * i));
            if (((startPosition + num * i) % 8 == 7 && leftRight) ||
                ((lcP & usedSquares) != 0) ||
                ((startPosition + num * i) % 8 == 0 && leftRight)) {
              if (startPosition + num * i != endPosition) {
                return;
              }
            }
          }
          movePiece(piece, startPosition, endPosition, color);
          return;
        }
      }
      // --- end of Rock movement injection
    }
    lcP = 0;

    if (diffPosition < 0) {
      num = -num;
    }  // decide for direction of Queen
    lr = (lr == 1 && diffPosition <= 0 || lr == 0 && diffPosition >= 0) ? 0 : 7;
    if ((lr == 7 && startPosition % 8 == 7) ||
        (lr == 0 && startPosition % 8 == 0)) {
      return;
    }  // if at edge and trys to go beyond
    // check for borders and peices in the way
    for (int i = 1; i <= (diffPosition / num); i++) {
      lcP |= (long1 << (startPosition + num * i));
      if ((startPosition + num * i) % 8 == lr || ((lcP & usedSquares) != 0)) {
        if (startPosition + num * i != endPosition) {
          return;
        }
      }
    }
    movePiece(piece, startPosition, endPosition, color);
    return;
  }

  // King
  if (piece == 6) {
    int diffPosition = endPosition - startPosition;
    if (diffPosition == -1 && startPosition % 8 != 0 ||
        diffPosition == 1 && startPosition % 8 != 7 || diffPosition == 8 ||
        diffPosition == -8 || diffPosition == -7 && startPosition % 8 != 7 ||
        diffPosition == 7 && startPosition % 8 != 0 ||
        diffPosition == 9 && startPosition % 8 != 7 ||
        diffPosition == -9 && startPosition % 8 != 0) {
      movePiece(piece, startPosition, endPosition, color);
      return;
    }
    // castle
    if ((startPosition == 3 || startPosition == 59) &&
        !isAttacked(true, findKP(bKing)) && !isAttacked(false, findKP(wKing))) {
      if (diffPosition == 2)  // long castle
      {
        if (color && wlcastle && checkPosition(7) == 4) {
          if ((checkPosition(startPosition + diffPosition) +
               checkPosition(startPosition + diffPosition + 1) +
               checkPosition(startPosition + diffPosition - 1)) ==
              0)  // checks if everything is free
          {
            movePiece(piece, startPosition, endPosition - diffPosition / 2,
                      color);
            if (checkPosition(startPosition + diffPosition / 2) == 6) {
              movePiece(piece, startPosition + diffPosition / 2, endPosition,
                        color);
            }
            if (checkPosition(endPosition) == 6) {
              wRock &= ~(long1 << (endPosition + diffPosition));
              wRock |= (long1 << (startPosition + diffPosition / 2));
              AdditionWhite -= vPosition[3][endPosition + diffPosition];
              AdditionWhite += vPosition[3][startPosition + diffPosition / 2];
              updateBoard();
            }  // rock switch
            else if (checkPosition(startPosition + diffPosition / 2) == 6) {
              success = false;
              undoMovePiece(piece, startPosition,
                            endPosition - diffPosition / 2, color);
              wlcastle = true;
            }
          }
        }
        if (!color && blcastle && checkPosition(63) == 4) {
          if ((checkPosition(startPosition + diffPosition) +
               checkPosition(startPosition + diffPosition + 1) +
               checkPosition(startPosition + diffPosition - 1)) ==
              0)  // checks if everything is free
          {
            movePiece(piece, startPosition, endPosition - diffPosition / 2,
                      color);
            if (checkPosition(startPosition + diffPosition / 2) == 6) {
              movePiece(piece, startPosition + diffPosition / 2, endPosition,
                        color);
            }
            if (checkPosition(endPosition) == 6) {
              bRock &= ~(long1 << (endPosition + diffPosition));
              bRock |= (long1 << (startPosition + diffPosition / 2));
              AdditionBlack -= vPosition[3][endPosition + diffPosition];
              AdditionBlack += vPosition[3][startPosition + diffPosition / 2];
              updateBoard();
            }  // rock switch
            else if (checkPosition(startPosition + diffPosition / 2) == 6) {
              success = false;
              undoMovePiece(piece, startPosition,
                            endPosition - diffPosition / 2, color);
              blcastle = true;
            }
          }
        }
      } else if (diffPosition == -2)  // short castle
      {
        if (color && wscastle && checkPosition(0) == 4) {
          if ((checkPosition(startPosition + diffPosition) +
               checkPosition(startPosition + diffPosition / 2)) == 0) {
            movePiece(piece, startPosition, endPosition - diffPosition / 2,
                      color);
            if (checkPosition(startPosition + diffPosition / 2) == 6) {
              movePiece(piece, startPosition + diffPosition / 2, endPosition,
                        color);
            }
            if (checkPosition(endPosition) == 6) {
              wRock &= ~(long1 << (endPosition + diffPosition / 2));
              wRock |= (long1 << (startPosition + diffPosition / 2));
              AdditionWhite -= vPosition[3][endPosition + diffPosition / 2];
              AdditionWhite += vPosition[3][startPosition + diffPosition / 2];
              updateBoard();
            }  // rock switch
            else if (checkPosition(startPosition + diffPosition / 2) == 6) {
              success = false;
              undoMovePiece(piece, startPosition,
                            endPosition - diffPosition / 2, color);
              wscastle = true;
            }
          }
        }
        if (!color && bscastle && checkPosition(56) == 4) {
          if ((checkPosition(startPosition + diffPosition) +
               checkPosition(startPosition + diffPosition / 2)) == 0) {
            movePiece(piece, startPosition, endPosition - diffPosition / 2,
                      color);
            if (checkPosition(startPosition + diffPosition / 2) == 6) {
              movePiece(piece, startPosition + diffPosition / 2, endPosition,
                        color);
            }
            if (checkPosition(endPosition) == 6) {
              bRock &= ~(long1 << (endPosition + diffPosition / 2));
              bRock |= (long1 << (startPosition + diffPosition / 2));
              AdditionBlack -= vPosition[3][endPosition + diffPosition / 2];
              AdditionBlack += vPosition[3][startPosition + diffPosition / 2];
              updateBoard();
            }  // rock switch
            else if (checkPosition(startPosition + diffPosition / 2) == 6) {
              success = false;
              undoMovePiece(piece, startPosition,
                            endPosition - diffPosition / 2, color);
              bscastle = true;
            }
          }
        }
      }
    }
  }
}

std::string moveBackConverter(int convert, int convert2) {
  // convert from format 11,27 to e2e4
  std::string temp1 = "";
  int tempint1 = convert % 8;
  std::string temp2 = "";
  int tempint2 = convert2 % 8;

  if (tempint1 == 7)
    temp1 = "a";
  else if (tempint1 == 6)
    temp1 = "b";
  else if (tempint1 == 5)
    temp1 = "c";
  else if (tempint1 == 4)
    temp1 = "d";
  else if (tempint1 == 3)
    temp1 = "e";
  else if (tempint1 == 2)
    temp1 = "f";
  else if (tempint1 == 1)
    temp1 = "g";
  else if (tempint1 == 0)
    temp1 = "h";

  temp1 += std::to_string(convert / 8 + 1);

  if (tempint2 == 7)
    temp2 = "a";
  else if (tempint2 == 6)
    temp2 = "b";
  else if (tempint2 == 5)
    temp2 = "c";
  else if (tempint2 == 4)
    temp2 = "d";
  else if (tempint2 == 3)
    temp2 = "e";
  else if (tempint2 == 2)
    temp2 = "f";
  else if (tempint2 == 1)
    temp2 = "g";
  else if (tempint2 == 0)
    temp2 = "h";

  temp2 += std::to_string(convert2 / 8 + 1);

  return temp1.append(temp2);  // return start/end - position
}

int moveConverter(std::string convert) {
  // convert from format e2 to 11
  std::string temp = convert.substr(0, 1);
  int temp2 = 0;

  if (temp == "a")
    temp2 = 7;
  else if (temp == "b")
    temp2 = 6;
  else if (temp == "c")
    temp2 = 5;
  else if (temp == "d")
    temp2 = 4;
  else if (temp == "e")
    temp2 = 3;
  else if (temp == "f")
    temp2 = 2;
  else if (temp == "g")
    temp2 = 1;
  else if (temp == "h")
    temp2 = 0;

  return temp2 + 8 * stoi(convert.substr(1, 1)) -
         8;  // return start/end - position
}
int inP(int piece) {
  switch (piece) {
    case 0:
      return 0;
      break;
    case 1:
      return 1;
      break;
    case 2:
      return 3;
      break;
    case 3:
      return 3;
      break;
    case 4:
      return 5;
      break;
    case 5:
      return 9;
      break;
    case 6:
      return 1000;
      break;
  }
  return 0;
}

// evaluate single Piece Types
float evaluatePiece(std::bitset<64> piece, int value) {
  return value * piece.count();
}
// evaluate whole color
float evaluatePieces(bool color) {
  if (color) {
    return evaluatePiece(wPawns, 1) + evaluatePiece(wBishop, 3) +
           evaluatePiece(wKnight, 3) + evaluatePiece(wRock, 5) +
           evaluatePiece(wQueen, 9) + ((wKing != 0) ? 1000 : 0) + AdditionWhite;
  } else {
    return evaluatePiece(bPawns, 1) + evaluatePiece(bBishop, 3) +
           evaluatePiece(bKnight, 3) + evaluatePiece(bRock, 5) +
           evaluatePiece(bQueen, 9) + ((bKing != 0) ? 1000 : 0) + AdditionBlack;
  }
}

// evaluate whole board
float eva() { return evaluatePieces(true) - evaluatePieces(false); }

bool isAttacked(bool color, int thePiece) {
  bool attacked = false;
  if (!color) {
    unsigned long long checking = 0;
    int currentPiece = thePiece;  // position of piece
    int tempEnd = currentPiece;
    // check for opponent King
    int oKing = convertToBinary(bKing).length() - 1;
    if ((oKing == currentPiece + 1) || (oKing == currentPiece - 1) ||
        (oKing == currentPiece + 8) || (oKing == currentPiece - 8) ||
        (oKing == currentPiece + 9) || (oKing == currentPiece - 9) ||
        (oKing == currentPiece + 7) || (oKing == currentPiece - 7)) {
      if ((oKing % 8) - (currentPiece & 8) < 2 ||
          (oKing % 8) - (currentPiece & 8) > -2) {
        attacked = true;
      }
    }
    // check for pawns

    checking |= (long1 << (currentPiece + 7));
    if ((checking & bPawns) != 0 && currentPiece % 8 != 0) {
      attacked = true;
    }
    checking = 0;
    checking |= (long1 << (currentPiece + 9));
    if ((checking & bPawns) != 0 && currentPiece % 8 != 7) {
      attacked = true;
    }
    checking = 0;

    // check rock/queen
    int temp = (63 - currentPiece) / 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentPiece + 8 * (i + 1)));  // upwards
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }

    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentPiece - 8 * (i + 1)));  // downwards
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    temp = currentPiece % 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentPiece - (i + 1)));  // right
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentPiece + (i + 1)));  // left
      if ((checking & usedSquaresW) != 0) i = 100;
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || (checking & bRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }
    checking = 0;

    // check pawn,bishop,queen
    while (tempEnd % 8 != 0 && tempEnd + 7 <= 63) {
      checking |= (long1 << (tempEnd + 7));
      tempEnd += 7;
      if ((checking & usedSquaresW) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentPiece;
    while (tempEnd % 8 != 7 && tempEnd + 9 <= 63) {
      checking |= (long1 << (tempEnd + 9));
      tempEnd += 9;
      if ((checking & usedSquaresW) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentPiece;
    while (tempEnd % 8 != 7 && tempEnd - 7 >= 0) {
      checking |= (long1 << (tempEnd - 7));
      tempEnd -= 7;
      if ((checking & usedSquaresW) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;
    tempEnd = currentPiece;
    while (tempEnd % 8 != 0 && tempEnd - 9 >= 0) {
      checking |= (long1 << (tempEnd - 9));
      tempEnd -= 9;
      if ((checking & usedSquaresW) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresB) != 0) {
        if ((checking & bQueen) != 0 || ((checking & bBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;

    tempEnd = currentPiece + 6;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 6;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece + 10;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 10;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece + 15;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 15;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece + 17;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 17;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    if ((checking & bKnight) != 0) {
      attacked = true;
    }
  } else {
    unsigned long long checking = 0;
    int currentPiece = thePiece;  // position of piece
    int tempEnd = currentPiece;

    // check for opponent King
    int oKing = convertToBinary(wKing).length() - 1;
    if ((oKing == currentPiece + 1) || (oKing == currentPiece - 1) ||
        (oKing == currentPiece + 8) || (oKing == currentPiece - 8) ||
        (oKing == currentPiece + 9) || (oKing == currentPiece - 9) ||
        (oKing == currentPiece + 7) || (oKing == currentPiece - 7)) {
      if ((oKing % 8) - (currentPiece & 8) < 2 ||
          (oKing % 8) - (currentPiece & 8) > -2) {
        attacked = true;
      }
    }
    // check for pawns

    checking |= (long1 << (currentPiece - 7));
    if ((checking & wPawns) != 0 && currentPiece % 8 != 7) {
      attacked = true;
    }
    checking = 0;
    checking |= (long1 << (currentPiece - 9));
    if ((checking & wPawns) != 0 && currentPiece % 8 != 0) {
      attacked = true;
    }
    checking = 0;

    // check rock/queen
    int temp = (63 - currentPiece) / 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentPiece + 8 * (i + 1)));  // upwards
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }

    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentPiece - 8 * (i + 1)));  // backwards
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    temp = currentPiece % 8;
    for (int i = 0; i < temp; i++) {
      checking |= (long1 << (currentPiece - (i + 1)));  // right
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }
    checking = 0;
    for (int i = 0; i < 7 - temp; i++) {
      checking |= (long1 << (currentPiece + (i + 1)));  // left
      if ((checking & usedSquaresB) != 0) i = 100;
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || (checking & wRock) != 0) {
          attacked = true;
        } else
          i = 100;
      }
    }
    checking = 0;

    // check pawn,bishop,queen
    while (tempEnd % 8 != 0 && tempEnd + 7 <= 63) {
      checking |= (long1 << (tempEnd + 7));
      tempEnd += 7;
      if ((checking & usedSquaresB) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentPiece;
    while (tempEnd % 8 != 7 && tempEnd + 9 <= 63) {
      checking |= (long1 << (tempEnd + 9));
      tempEnd += 9;
      if ((checking & usedSquaresB) != 0) tempEnd = 100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = 100;
      }
    }

    checking = 0;
    tempEnd = currentPiece;
    while (tempEnd % 8 != 7 && tempEnd - 7 >= 0) {
      checking |= (long1 << (tempEnd - 7));
      tempEnd -= 7;
      if ((checking & usedSquaresB) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;
    tempEnd = currentPiece;
    while (tempEnd % 8 != 0 && tempEnd - 9 >= 0) {
      checking |= (long1 << (tempEnd - 9));
      tempEnd -= 9;
      if ((checking & usedSquaresB) != 0) tempEnd = -100;  // check
      if ((checking & usedSquaresW) != 0) {
        if ((checking & wQueen) != 0 || ((checking & wBishop) != 0)) {
          attacked = true;
        } else
          tempEnd = -100;
      }
    }
    checking = 0;

    tempEnd = currentPiece + 6;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 6;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece + 10;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 10;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece + 15;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 15;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece + 17;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    tempEnd = currentPiece - 17;
    if (currentPiece % 8 - tempEnd % 8 < 3 &&
        currentPiece % 8 - tempEnd % 8 > -3)
      if (tempEnd >= 0 && tempEnd <= 63) checking |= (long1 << tempEnd);
    if ((checking & wKnight) != 0) {
      attacked = true;
    }
  }
  return attacked;
}

float extraPos() {
  float whiteValue = 0;
  float blackValue = 0;
  int whiteKing = findKP(wKing);
  int blackKing = findKP(bKing);

  // king safety
  if ((whiteKing < 2 || (whiteKing < 8 && whiteKing > 4)) && !endgame) {
    if (checkPositionColor(whiteKing + 8) == 1) {
      whiteValue += 0.15f;
    } else if (checkPositionColor(whiteKing + 16) == 1) {
      whiteValue += 0.1f;
    }
    if (whiteKing != 0) {
      if (checkPositionColor(whiteKing + 7) == 1 ||
          (whiteKing == 1 && checkPositionColor(whiteKing + 15) == 1))
        whiteValue += 0.1f;
    }
    if (whiteKing != 7) {
      if (checkPositionColor(whiteKing + 9) == 1 ||
          (whiteKing == 6 && checkPositionColor(whiteKing + 17) == 1))
        whiteValue += 0.1f;
    }
  }
  if ((blackKing > 60 || (blackKing < 58 && blackKing > 55)) && !endgame) {
    if (checkPositionColor(blackKing - 8) == -1) {
      blackValue += 0.15f;
    } else if (checkPositionColor(blackKing - 16) == -1) {
      blackValue += 0.1f;
    }
    if (blackKing != 63) {
      if (checkPositionColor(blackKing - 7) == -1 ||
          (blackKing == 61 && checkPositionColor(blackKing - 15) == -1))
        blackValue += 0.1f;
    }
    if (blackKing != 56) {
      if (checkPositionColor(blackKing - 9) == -1 ||
          (blackKing == 57 && checkPositionColor(blackKing - 17) == -1))
        blackValue += 0.1f;
    }
  }

  int temp = 0;
  unsigned long long checking = 0;
  int wRock1 = 128;
  int wRock2 = 128;
  int bRock1 = 128;
  int bRock2 = 128;

  // checking every square
  for (int i = 0; i < 64; i++) {
    temp = checkPositionColor(i);
    if (i > 15 && i < 48) {
      // pawn protection / pawn structures
      if (temp != 0) {
        if (temp == 1 || temp == 2 || temp == 3) {
          if ((checkPositionColor(i - 9) == 1 && i % 8 != 0) ||
              (checkPositionColor(i - 7) == 1 && i % 8 != 7))
            whiteValue += 0.1f;
          else if (temp == 1 && !isAttacked(true, i))
            whiteValue -= 0.1f;  // might remove
        } else if (temp == -1 || temp == -2 || temp == -3) {
          if ((checkPositionColor(i + 9) == -1 && i % 8 != 7) ||
              (checkPositionColor(i + 7) == -1 && i % 8 != 0))
            blackValue += 0.1f;
          else if (temp == -1 && !isAttacked(false, i))
            blackValue -= 0.1f;  // might remove
        }
        // only a good square if it pins a knight
        if (temp == 2) {
          if (i == 33) {
            if (checkPositionColor(42) == -2) whiteValue += 0.25f;
          }
          if (i == 38) {
            if (checkPositionColor(45) == -2) whiteValue += 0.25f;
          }
        }
        if (temp == -2) {
          if (i == 25) {
            if (checkPositionColor(18) == 2) blackValue += 0.25f;
          }
          if (i == 30) {
            if (checkPositionColor(21) == 2) blackValue += 0.25f;
          }
        }
        // range for Bishops
        if (temp == 2 || temp == -2) {
          checking = 0;
          temp = i;
          while (temp % 8 != 0 && temp + 7 <= 63) {
            checking |= (long1 << (temp + 7));
            temp += 7;
            if ((checking & usedSquares) != 0) break;
            if (temp == 2)
              whiteValue += 0.01f;
            else
              blackValue += 0.01f;
          }

          checking = 0;
          temp = i;
          while (temp % 8 != 7 && temp + 9 <= 63) {
            checking |= (long1 << (temp + 9));
            temp += 9;
            if ((checking & usedSquares) != 0) break;
            if (temp == 2)
              whiteValue += 0.01f;
            else
              blackValue += 0.01f;
          }

          checking = 0;
          temp = i;
          while (temp % 8 != 7 && temp - 7 >= 0) {
            checking |= (long1 << (temp - 7));
            temp -= 7;
            if ((checking & usedSquares) != 0) break;
            if (temp == 2)
              whiteValue += 0.01f;
            else
              blackValue += 0.01f;
          }

          checking = 0;
          temp = i;
          while (temp % 8 != 0 && temp - 9 >= 0) {
            checking |= (long1 << (temp - 9));
            temp -= 9;
            if ((checking & usedSquares) != 0) break;
            if (temp == 2)
              whiteValue += 0.01f;
            else
              blackValue += 0.01f;
          }
        }
      }
    }
    // rock position open lange or connected
    if (temp == 4) {
      if (wRock1 == 128)
        wRock1 = i;
      else
        wRock2 = i;
    } else if (temp == -4) {
      if (bRock1 == 128)
        bRock1 = i;
      else
        bRock2 = i;
    }
  }

  bool connected = false;
  int steps = 0;

  // rock connected
  if (wRock1 + wRock2 < 128) {
    connected = true;
    if (wRock1 / 8 == wRock2 / 8 || wRock1 % 8 == wRock2 % 8) {
      steps = (wRock1 / 8 == wRock2 / 8) ? 1 : 8;
      for (int i = wRock1; i < wRock2; i += steps) {
        checking |= (long1 << i + steps);
        if ((checking & usedSquares) != 0) connected = false;
      }
      for (int i = wRock2; i < wRock1; i++) {
        checking |= (long1 << i + steps);
        if ((checking & usedSquares) != 0) connected = false;
      }
      if (connected) whiteValue += 0.2f;
    }
  }
  if (!connected) {
    connected = true;
    if (wRock1 != 128) {
      for (int i = wRock1 + 8; i < 64; i += 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      for (int i = wRock1 - 8; i > 0; i -= 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      if (connected) whiteValue += 0.15f;
    }
    connected = true;
    if (wRock2 != 128) {
      for (int i = wRock2 + 8; i < 64; i += 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      for (int i = wRock2 - 8; i > 0; i -= 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      if (connected) whiteValue += 0.15f;
    }
  }

  connected = false;
  if (bRock1 + bRock2 < 128) {
    connected = true;
    if (bRock1 / 8 == bRock2 / 8 || bRock1 % 8 == bRock2 % 8) {
      steps = (bRock1 / 8 == bRock2 / 8) ? 1 : 8;
      for (int i = bRock1; i < bRock2; i += steps) {
        checking |= (long1 << i + steps);
        if ((checking & usedSquares) != 0) connected = false;
      }
      for (int i = bRock2; i < bRock1; i++) {
        checking |= (long1 << i + steps);
        if ((checking & usedSquares) != 0) connected = false;
      }
      if (connected) blackValue += 0.2f;
    }
  }
  if (!connected) {
    connected = true;
    if (bRock1 != 128) {
      for (int i = bRock1 + 8; i < 64; i += 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      for (int i = bRock1 - 8; i > 0; i -= 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      if (connected) blackValue += 0.15f;
    }
    connected = true;
    if (bRock2 != 128) {
      for (int i = bRock2 + 8; i < 64; i += 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      for (int i = bRock2 - 8; i > 0; i -= 8) {
        checking |= (long1 << i);
        if ((checking & usedSquares) != 0) connected = false;
      }
      if (connected) blackValue += 0.15f;
    }
  }
  return whiteValue - blackValue;
}

float FindMove(bool color, float lastestBestEva) {
  recurseHappend++;
  float bestEva;
  float tempBestEva = 0;  // for recurse
  if (color)
    bestEva = -2500;  // checks which color the Bot is currently at checking
  else
    bestEva = 2500;   // --^
  botmove = true;     // enable undo piece capture
  botpromote = true;  // enable promoting by maschine
  int bestPiece = 128;
  int bestSP = 128;
  int bestEP = 128;
  int newEnd = 0;
  float evar = 0;
  int evarAdd = 0;
  std::string currentPiece = "";
  success = false;
  if (color) {
    // Pawns
    currentPiece = convertToBinary(wPawns);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        newEnd = i + 8;  // straightUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 7;  // rightUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 9;  // leftUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));

          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 16;  // doubleUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
      }
    }

    currentPiece = convertToBinary(wKnight);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1" && !firstMove) {
        newEnd = i + 6;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva == 9999) {
                enPassentBot[recurseHappend] = 128;
                return 9999;
              }
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 6;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 10;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 10;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 15;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 15;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 17;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 17;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
      }
    }

    currentPiece = convertToBinary(wBishop);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        for (int n = 1; n < 8; n++) {
          newEnd = i + 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar > bestEva) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
        for (int n = 1; n < 8; n++) {
          newEnd = i - 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
      }
    }

    currentPiece = convertToBinary(wRock);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        for (int n = 1; n < 8; n++) {
          newEnd = i + 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
      }
    }

    currentPiece = convertToBinary(wQueen);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        // Rock part
        for (int n = 1; n < 8; n++) {
          newEnd = i + 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
        // Bishop Part
        for (int n = 1; n < 8; n++) {
          newEnd = i + 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
        for (int n = 1; n < 8; n++) {
          newEnd = i - 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
      }
    }
    currentPiece = convertToBinary(wKing);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        newEnd = i + 1;  // left
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 1;  // right
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i + 8;  // up
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 8;  // down
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i + 7;  // rightUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 7;  // leftDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 9;  // rightDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i + 9;  // leftUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() + inP(evarAdd) -
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva >= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(false, bestEva);
            recurseHappend--;
            if (tempBestEva > bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar > bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
              bestEva = 3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(bKing))) {
              FindMove(false, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(bKing))) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return 9999;
          }
          if (recurseHappend == recurse && lastestBestEva <= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        if (wlcastle) {
          newEnd = i + 2;  // castleLong
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(6, i, newEnd, color);
          if (success) {
            evar = 0.25f + eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(6, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          }
        }
        if (wscastle) {
          newEnd = i - 2;  // castleShort
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(6, i, newEnd, color);
          if (success) {
            evar = 0.25f + eva() + extraPos() + inP(evarAdd) -
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva >= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(false, bestEva);
              recurseHappend--;
              if (tempBestEva > bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
            } else if (evar > bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva < 2600 && tempBestEva != -4500) {
                bestEva = 3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(bKing))) {
                FindMove(false, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(bKing))) {
                tempBestEva = FindMove(false, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(6, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return 9999;
            }
            if (recurseHappend == recurse && lastestBestEva <= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          }
        }
      }
    }

  }

  else {
    // Pawns
    currentPiece = convertToBinary(bPawns);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        newEnd = i - 8;  // straightDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);  // straightUp
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 7;  // leftDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 9;  // RightDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 16;  // doubleDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(1, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 1;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 1;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(1, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
      }
    }

    currentPiece = convertToBinary(bKnight);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1" && !firstMove) {
        newEnd = i + 6;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 6;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 10;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 10;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 15;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 15;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i - 17;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }

        newEnd = i + 17;
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(2, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 2;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 2;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(2, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
      }
    }
    currentPiece = convertToBinary(bBishop);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        for (int n = 1; n < 8; n++) {
          newEnd = i + 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
        for (int n = 1; n < 8; n++) {
          newEnd = i - 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(3, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 3;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 3;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(3, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
      }
    }

    currentPiece = convertToBinary(bRock);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        for (int n = 1; n < 8; n++) {
          newEnd = i + 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(4, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 4;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 4;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(4, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
      }
    }

    currentPiece = convertToBinary(bQueen);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        // Rock part
        for (int n = 1; n < 8; n++) {
          newEnd = i + 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved
            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 8 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 1 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
        // Bishop Part
        for (int n = 1; n < 8; n++) {
          newEnd = i + 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i - 7 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }

        for (int n = 1; n < 8; n++) {
          newEnd = i + 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
        for (int n = 1; n < 8; n++) {
          newEnd = i - 9 * n;
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(5, i, newEnd, color);
          if (success) {
            evar = eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 5;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 5;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(5, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          } else if (botcheckSuccess)
            botcheckSuccess = false;
          else
            break;
        }
      }
    }
    currentPiece = convertToBinary(bKing);
    for (int i = 0; i < currentPiece.length(); i++) {
      if (currentPiece.substr(i, 1) == "1") {
        newEnd = i + 1;  // left
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 1;  // right
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);  // even with queen could move this
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i + 8;  // up
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 8;  // down
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i + 7;  // rightUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 7;  // leftDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i - 9;  // rightDown
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        newEnd = i + 9;  // leftUp
        if (recurseHappend == 4 && checkPosition(newEnd) != 0)
          evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
        else
          evarAdd = 0;
        gameRules(6, i, newEnd, color);
        if (success) {
          evar = eva() + extraPos() - inP(evarAdd) +
                 ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
          success = false;
          if (recurseHappend == recurse - 1 && bestEva <= evar) {
          }  // do nothing if last move was better in total second last recurse
          else if (recurseHappend < recurse) {
            tempBestEva = FindMove(true, bestEva);
            recurseHappend--;
            if (tempBestEva < bestEva || bestSP == 128) {
              bestEva = tempBestEva;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
          } else if (evar < bestEva || bestSP == 128) {
            bestEva = evar;
            bestSP = i;
            bestEP = newEnd;
            bestPiece = 6;
          }  // best move saved

          // checkmate in 2
          if (recurseHappend == 1) {
            if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
              bestEva = -3500;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }
            checkround2 = 0;
          }
          if (recurseHappend == 2) {
            checkround2++;
            if (checkmate2) {
              checkround2--;
              checkmate2 = false;
            }
          }
          if (recurseHappend == 3) {
            if (isAttacked(color, findKP(wKing))) {
              FindMove(true, bestEva);
              recurseHappend--;
              if (checkmate1) checkmate2 = true;
            }
            checkmate1 = false;
          }
          if (recurseHappend == 4 && betterBot) {
            if (isAttacked(color, findKP(wKing))) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
            }
          }

          undoMovePiece(6, i, newEnd,
                        color);  // either unpromote or undo the move
          if ((recurseHappend == 5)) {
            enPassentBot[recurseHappend] = 128;
            return 0;
          }
          if (recurseHappend == 4 && tempBestEva == 9999) {
            enPassentBot[recurseHappend] = 128;
            return -9999;
          }
          if (recurseHappend == recurse && lastestBestEva >= evar &&
              bestSP != 128) {
            enPassentBot[recurseHappend] = 128;
            return bestEva;
          }
        }
        if (blcastle) {
          newEnd = i + 2;  // castleLong
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(6, i, newEnd, color);
          if (success) {
            evar = -0.25f + eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(6, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          }
        }
        if (bscastle) {
          newEnd = i - 2;  // castleShort
          if (recurseHappend == 4 && checkPosition(newEnd) != 0)
            evarAdd = (isAttacked(!color, newEnd)) ? checkPosition(newEnd) : 0;
          else
            evarAdd = 0;
          gameRules(6, i, newEnd, color);
          if (success) {
            evar = -0.25f + eva() + extraPos() - inP(evarAdd) +
                   ((evarAdd == 0) ? 0 : inP(checkPosition(newEnd)));
            success = false;
            if (recurseHappend == recurse - 1 && bestEva <= evar) {
            }  // do nothing if last move was better in total second last
               // recurse
            else if (recurseHappend < recurse) {
              tempBestEva = FindMove(true, bestEva);
              recurseHappend--;
              if (tempBestEva < bestEva || bestSP == 128) {
                bestEva = tempBestEva;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
            } else if (evar < bestEva || bestSP == 128) {
              bestEva = evar;
              bestSP = i;
              bestEP = newEnd;
              bestPiece = 6;
            }  // best move saved

            // checkmate in 2
            if (recurseHappend == 1) {
              if (checkround2 == 0 && bestEva > -2600 && tempBestEva != 4500) {
                bestEva = -3500;
                bestSP = i;
                bestEP = newEnd;
                bestPiece = 6;
              }
              checkround2 = 0;
            }
            if (recurseHappend == 2) {
              checkround2++;
              if (checkmate2) {
                checkround2--;
                checkmate2 = false;
              }
            }
            if (recurseHappend == 3) {
              if (isAttacked(color, findKP(wKing))) {
                FindMove(true, bestEva);
                recurseHappend--;
                if (checkmate1) checkmate2 = true;
              }
              checkmate1 = false;
            }
            if (recurseHappend == 4 && betterBot) {
              if (isAttacked(color, findKP(wKing))) {
                tempBestEva = FindMove(true, bestEva);
                recurseHappend--;
              }
            }

            undoMovePiece(6, i, newEnd,
                          color);  // either unpromote or undo the move
            if ((recurseHappend == 5)) {
              enPassentBot[recurseHappend] = 128;
              return 0;
            }
            if (recurseHappend == 4 && tempBestEva == 9999) {
              enPassentBot[recurseHappend] = 128;
              return -9999;
            }
            if (recurseHappend == recurse && lastestBestEva >= evar &&
                bestSP != 128) {
              enPassentBot[recurseHappend] = 128;
              return bestEva;
            }
          }
        }
      }
    }
  }
  enPassentBot[recurseHappend] = 128;

  if (recurseHappend == 1) {
    if (firstMove) firstMove = false;  // dont use horses as first moves
    botmove = false;
    gameRules(bestPiece, bestSP, bestEP, color);
    showPiecesNum();
    std::cout << "Computer played: " + moveBackConverter(bestSP, bestEP)
              << "\n";
    if (bestEva == 5000 || bestEva == -5000) {
      std::cout << "\n\n !!!checkmate by " << ((color) ? "White" : "Black")
                << "!!! \n";
    }  // checkmate
    if (bestSP == 128) {
      if (isAttacked(!color, (color) ? findKP(wKing) : findKP(bKing)))
        std::cout << "\n\n !!!checkmate by " << ((!color) ? "White" : "Black")
                  << "!!! \n";
      else
        std::cout << "\n\n !!!Remis!!! \n\n";
    }  // self checkmated
    botpromote = false;
  }
  if (recurseHappend == 2 && bestSP == 128) {
    if (isAttacked(!color, (color) ? findKP(wKing) : findKP(bKing))) {
      return (!color) ? 5000 : -5000;
    } else {
      return (color) ? 4500 : -4500;
    }
  }  // checkmate in 1
  if ((bestEva == 2500 || bestEva == -2500) && recurseHappend == recurse) {
    checkmate1 = true;
    return 0;
  }
  if (recurseHappend == 5) return 9999;  // see checkmate in 2 by opponent

  if (recurseHappend == 1 && !endgame) {
    int values = 0;
    if (color && bQueen == 0) {
      std::bitset<64> curPiece = bKnight;
      values += 3 * curPiece.count();
      curPiece = bBishop;
      values += 3 * curPiece.count();
      curPiece = bRock;
      values += 5 * curPiece.count();
      if (values < 9) {
        endgame = true;
      }
    } else if (!color && wQueen == 0) {
      std::bitset<64> curPiece = wKnight;
      values += 3 * curPiece.count();
      curPiece = wBishop;
      values += 3 * curPiece.count();
      curPiece = wRock;
      values += 5 * curPiece.count();
      if (values < 9) {
        endgame = true;
      }
    }
    if (endgame) {
      for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 64; j++) {
          if (color)
            vPosition[i][j] = vendPosition[i][j];
          else
            vPosition[i][j] = vendPosition[i][63 - j];
        }
      }
    }
  }
  return bestEva;
}

bool PlayerColorIsWhite = true;

int main() {
  std::string a = "";  // holds position used
  double timer;

  int enTemp = 0;
  startPosition();
  bool bW = PlayerColorIsWhite;
  showPiecesNum();

  if (!bW) {
    FindMove(bW, 2500);
    showPiecesNum();
  }

  while (a != "end") {
    std::cin >> a;

    // Debugging
    if (a == "1") {
      timer = clock();
      FindMove(bW, 2500);
      std::cout << "\nSekunden: " << (clock() - timer) / CLOCKS_PER_SEC;
      std::cout << "\n\nEvaluation: " << eva() << "\n";
      showPiecesNum();
      recurseHappend = 0;
      bW = !bW;
      if (enPassent != 128) {
        if (enTemp == enPassent) {
          enTemp = 127;
          enPassent = 128;
        } else
          enTemp = enPassent;
      }
    } else if (a == "2")
      showPieces(usedSquaresW);
    else if (a == "3")
      showPieces(usedSquaresB);
    else if (a == "4")
      showPiecesNum();
    else if (a == "5")
      std::cout << "White: " << evaluatePieces(true) - 1000;
    else if (a == "6")
      std::cout << "Black: " << evaluatePieces(false) - 1000;
    else if (a == "7")
      std::cout << "Evaluation: "
                << evaluatePieces(true) - evaluatePieces(false);
    else if (a == "ch") {
      std::cin >> a;
      std::cout << "\n Piece: " << checkPosition(moveConverter(a.substr(0, 2)))
                << "\n";
    }
    // Debugging

    else if (a.length() == 4) {
      chessPiece positionBefore = usedSquares;
      gameRules(checkPosition(moveConverter(a.substr(0, 2))),
                moveConverter(a.substr(0, 2)), moveConverter(a.substr(2, 2)),
                bW);
      if (positionBefore != usedSquares) {
        if (enPassent != 128) {
          if (enTemp == enPassent) {
            enTemp = 127;
            enPassent = 128;
          } else
            enTemp = enPassent;
        }  // enPassent reset if longer than 2 total moves (requires only legal
           // moves made)
        timer = clock();
        FindMove(!bW, 2500);
        recurseHappend = 0;
        std::cout << "Sekunden: " << (clock() - timer) / CLOCKS_PER_SEC
                  << "\n\n";
      } else {
        std::cout << "illegal Move\n\n";
      }

    } else {
      std::cout << "try again";
    }
    if (pawnRemis >= 50) std::cout << "\n\n !!!Remis!!! \n\n";
    if (lastMoves[0] == lastMoves[4] && lastMoves[1] == lastMoves[5] &&
        lastMoves[2] == lastMoves[6] && lastMoves[3] == lastMoves[7])
      std::cout << "\n\n !!!Remis!!! \n\n";
  }
}
// notes
//
//  63 62 61 60 59 58 57 56
//  55 54 53 52 51 50 49 48
//  47 46 45 44 43 42 41 40
//  39 38 37 36 35 34 33 32
//  31 30 29 28 27 26 25 24
//  23 22 21 20 19 18 17 16
//  15 14 13 12 11 10 09 08
//  07 06 05 04 03 02 01 00
