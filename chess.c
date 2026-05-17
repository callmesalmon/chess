#include <stdio.h>
#include <ctype.h>

enum chessPieceType {
    PAWN,
    KING,
    QUEEN,
    KNIGHT,
    BISHOP,
    ROOK,

    EMPTY
};

enum chessPieceColor {
    BLACK,
    WHITE,

    NO_COLOR
};

typedef struct {
    enum chessPieceType type;
    enum chessPieceColor color;
} chessPiece;

typedef struct {
    chessPiece piece;
    // There is no reason to combine these by default, i. e 'a1'
    char row;  // i. e '1'
    char file; // i. e 'a'
} chessSquare;

chessSquare chess_board[8][8]; // Row comes first, i. e chess_board[0][3] = e1

int is_good_square(chessSquare square) {
    if (square.row > '8' || square.row < '1') return 0;
    if (square.file > 'h' || square.file < 'a') return 0;

    return 1;
}

int set_square(int rank, int file, enum chessPieceType type, enum chessPieceColor color) {
    chess_board[rank][file].piece.type = type;
    chess_board[rank][file].piece.color = color;
    chess_board[rank][file].row = '1' + rank;
    chess_board[rank][file].file = 'a' + file;
}

int make_board() {
   for (int r = 0; r < 8; r++) {
        for (int f = 0; f < 8; f++) {
            set_square(r, f, EMPTY, NO_COLOR);
        }
    }

    set_square(0, 0, ROOK, WHITE);
    set_square(0, 1, KNIGHT, WHITE);
    set_square(0, 2, BISHOP, WHITE);
    set_square(0, 3, QUEEN, WHITE);
    set_square(0, 4, KING, WHITE);
    set_square(0, 5, BISHOP, WHITE);
    set_square(0, 6, KNIGHT, WHITE);
    set_square(0, 7, ROOK, WHITE);
    for (int f = 0; f < 8; f++) set_square(1, f, PAWN, WHITE);

    set_square(7, 0, ROOK, BLACK);
    set_square(7, 1, KNIGHT, BLACK);
    set_square(7, 2, BISHOP, BLACK);
    set_square(7, 3, QUEEN, BLACK);
    set_square(7, 4, KING, BLACK);
    set_square(7, 5, BISHOP, BLACK);
    set_square(7, 6, KNIGHT, BLACK);
    set_square(7, 7, ROOK, BLACK);
    for (int f = 0; f < 8; f++) set_square(6, f, PAWN, BLACK);

    return 0;
}

int print_square(chessPiece piece) {

    char to_print = '.';


    if (piece.color == NO_COLOR) goto print;

    switch (piece.type) {
        case PAWN:   to_print = 'P'; break;
        case KING:   to_print = 'K'; break;
        case QUEEN:  to_print = 'Q'; break;
        case KNIGHT: to_print = 'N'; break;
        case BISHOP: to_print = 'B'; break;
        case ROOK:   to_print = 'R'; break;
        case EMPTY:
        default:     to_print = '.';
    }

    if (piece.color == BLACK) to_print = tolower((unsigned char)to_print);

print:
    
    putchar(to_print);
    
    return 0;
}

int print_board() {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            if (j == 0) printf("%c ", chess_board[i][j].row);
            print_square(chess_board[i][j].piece);
        }
        printf("\n");
    }
    printf("  ABCDEFGH\n");

    return 0;
}

chessSquare *parse_coordinates(const char *coords) {
    int file = coords[0] - 'a';
    int row  = coords[1] - '1';
    return &chess_board[row][file];
}

int row_to_int(chessSquare *square) {
    return (square->row - '1');
}

int file_to_int(chessSquare *square) {
    return (square->file - 'a');
}

#define MOVE_LEGAL   24
#define MOVE_ILLEGAL 25

int is_valid_move(chessSquare *piece, chessSquare *dest) {
    int piece_row = row_to_int(piece);
    int piece_file = file_to_int(piece);

    int dest_row = row_to_int(dest);
    int dest_file = file_to_int(dest);


    // You can never stay in the same place after a move.
    if (dest_file == piece_file && \
            dest_row == piece_row) {
        return MOVE_ILLEGAL;
    }

    // You can't capture a piece of the same color.
    if (piece->piece.color == BLACK && dest->piece.color == BLACK) return MOVE_ILLEGAL;
    if (piece->piece.color == WHITE && dest->piece.color == WHITE) return MOVE_ILLEGAL;

    if (piece->piece.type == PAWN) {
        if (piece->piece.color == WHITE) {
            if (piece_row == 1) {
                if (dest_row > piece_row + 2) {
                    return MOVE_ILLEGAL;
                }
            }
            else if (dest_row > piece_row + 1) {
                return MOVE_ILLEGAL;
            }

            if (piece_file != dest_file) {
                if (dest_row == piece_row + 1 && dest->piece.type != EMPTY) {
                    return MOVE_LEGAL;
                }
                return MOVE_ILLEGAL;
            }
        }
        if (piece->piece.color == BLACK) {
            if (piece_row == 6) {
                if (dest_row < piece_row - 2) {
                    return MOVE_ILLEGAL;
                }
            }
            else if (dest_row < piece_row - 1) {
                return MOVE_ILLEGAL;
            }
            if (piece_file != dest_file) {
                if (dest_row == piece_row - 1 && dest->piece.type != EMPTY) {
                    return MOVE_LEGAL;
                }
                return MOVE_ILLEGAL;
            }
        }
        if (dest->piece.type != EMPTY && piece_file == dest_file) return MOVE_ILLEGAL;
    }
    else if (piece->piece.type == KNIGHT) {
        if (dest_row == piece_row + 2 || \
            dest_row == piece_row - 2) {
            if (dest_file == piece_file + 1 || \
                    dest_file == piece_file - 1) {
                return MOVE_LEGAL;
            }
        }
        else if (dest_file == piece_file + 2 || \
            dest_file == piece_file - 2) {
            if (dest_row == piece_row + 1 || \
                    dest_row == piece_row - 1) {
                return MOVE_LEGAL;
            }
        }
        return MOVE_ILLEGAL;
    }
    else if (piece->piece.type == KING) {
        if (dest_file > piece_file + 1 || \
                dest_file < piece_file - 1) {
            return MOVE_ILLEGAL;
        }
        if (dest_row > piece_row + 1 || \
                dest_row < piece_row - 1) {
            return MOVE_ILLEGAL;
        }
    }
    else if (piece->piece.type == ROOK) {
        if ((dest_file > piece_file || dest_file < piece_file) && \
                (dest_row > piece_row || dest_row < piece_row)) {
            return MOVE_ILLEGAL;
        }

        // `block` is the coords on the file/row where a piece blocks
        // the view of another piece.
        int block = -1;

        // We handle the cases differently depending on the four directions
        // the rook can go in.
        if (dest_row > piece_row) {
            for (int i = piece_row; i < dest_row; i++) {
                if (chess_board[i][dest_file].piece.type != EMPTY \
                    && i != piece_row) block = i;
                if (block < dest_row && block != -1) return MOVE_ILLEGAL;
            }
        }
        else if (dest_row < piece_row) {
            for (int i = piece_row; i > dest_row; i--) {
                if (chess_board[i][dest_file].piece.type != EMPTY \
                    && i != piece_row) block = i;
                if (block > dest_row && block != -1) return MOVE_ILLEGAL;
            }
        }
        else if (dest_file > piece_file) {
            for (int i = piece_row; i < dest_row; i++) {
                if (chess_board[dest_row][i].piece.type != EMPTY \
                    && i != piece_file) block = i;
                if (block < dest_file && block != -1) return MOVE_ILLEGAL;
            }
        }
        else if (dest_file < piece_file) {
            for (int i = piece_row; i > dest_row; i--) {
                if (chess_board[dest_row][i].piece.type != EMPTY \
                    && i != piece_file) block = i;
                if (block > dest_file && block != -1) return MOVE_ILLEGAL;
            }
        }
    }

    return MOVE_LEGAL;
}

int move_piece(chessSquare *piece, chessSquare *dest) {
    if (piece->piece.type == EMPTY || piece->piece.color == NO_COLOR) return MOVE_ILLEGAL;
    if (!is_good_square(*piece) || !is_good_square(*dest)) return MOVE_ILLEGAL;
    if (is_valid_move(piece, dest) == MOVE_ILLEGAL) return MOVE_ILLEGAL;

    dest->piece.type = piece->piece.type;
    dest->piece.color = piece->piece.color;
    piece->piece.type = EMPTY;
    piece->piece.color = NO_COLOR;

    return 0;
}

int main() {
    make_board();

    enum chessPieceColor turn = WHITE;
    while (1) {
        print_board();
        printf("\n");

        printf("Turn: %s\n", (turn == WHITE) ? "White" : "Black");

        char from[3];
        printf("from: ");
        scanf("%2s", from);

        char to[3];
        printf("to: ");
        scanf("%2s", to);

        chessSquare *from_square = parse_coordinates(from);
        chessSquare *to_square = parse_coordinates(to);

        printf("\n(from:\n  file: %d\n  row: %d\nto:\n  file: %d\n  row: %d)\n\n",
            file_to_int(from_square), row_to_int(from_square),
            file_to_int(to_square), row_to_int(to_square));

        if (from_square->piece.type == EMPTY) {
            printf("Can't move an empty square!\n\n");
            continue;
        }
        if (from_square->piece.color != turn) {
            printf("Don't move other players' pieces!\n\n");
            continue;
        }
        if (move_piece(from_square, to_square) == MOVE_ILLEGAL) {
            printf("Illegal move! Try again!\n\n");
            continue;
        }

        turn = (turn == WHITE) ? BLACK : WHITE;
        printf("\n");
    }
}
