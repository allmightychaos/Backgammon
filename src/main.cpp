#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string.h>
#include <stdlib.h>

using namespace std;

void print_stones(int num, char c = '0')
{
    for (int i = 0; i < num; i++)
    {
        cout << c;
    }
}

int roll_dice()
{
    return rand() % 6 + 1;
}

void print_board_2(string p1, string p2, int red, int blue, int board[])
{
    system("clear");
    cout << "\033[34m" << p1 << " ";
    cout << "\033[0m";
    for (int i = 0; i < 24; i++)
    {
        cout << "\033[" << (i % 2 ? 37 : 30) << ";" << (i % 2 ? 40 : 47) << "m";
        if (i + 1 < 10)
        {
            cout << " ";
        }
        cout << " " << (i + 1) << " ";
    }
    cout << " \033[31m" << p2;
    cout << "\033[0m" << endl;
    int limit = max(blue, red);
    for (int i = 0; i < 24; i++)
    {
        limit = max(limit, abs(board[i]));
    }
    for (int i = 0; i < limit; i++)
    {
        print_stones(p1.length() - 1, ' ');
        if (blue > i)
        {
            cout << "\033[34m0 \033[0m";
        }
        else
        {
            cout << "  ";
        }
        for (int j = 0; j < 24; j++)
        {
            cout << "\033[" << (j % 2 ? 37 : 30) << ";" << (j % 2 ? 40 : 47) << "m";
            if (board[j] < 0 && abs(board[j]) > i)
            {
                cout << "\033[34m  0 \033[0m";
            }
            else if (board[j] > 0 && board[j] > i)
            {
                cout << "\033[31m  0 \033[0m";
            }
            else
            {
                cout << "    ";
            }
        }
        if (red > i)
        {
            cout << "\033[31m 0\033[0m";
        }
        cout << endl;
    }
}

void print_board(string p1, string p2, int red, int blue, int board[])
{
    int limit = max(p1.length() + blue - 1, p2.length() + red - 1);
    for (int i = 0; i < 24; i++)
    {
        limit = max(limit, abs(board[i]));
    }
    system("clear");
    cout << "\033[34m" << p1 << ": ";
    print_stones(blue, 'X');
    cout << "\033[0m" << endl;
    for (int i = 0; i < 24; i++)
    {
        cout << "\033[" << (i % 2 ? 37 : 30) << ";" << (i % 2 ? 40 : 47) << "m";
        cout << (i + 1 < 10 ? " " : "") << (i + 1) << ": ";
        if (board[i] > 0)
        {
            cout << "\033[31;" << (i % 2 ? 40 : 47) << "m";
            print_stones(board[i], 'O');
            print_stones(limit - board[i], ' ');
        }
        else if (board[i] < 0)
        {
            cout << "\033[34;" << (i % 2 ? 40 : 47) << "m";
            print_stones(board[i] * -1, 'X');
            print_stones(limit + board[i], ' ');
        }
        else
        {
            cout << "\033[34;" << (i % 2 ? 40 : 47) << "m";
            print_stones(limit, ' ');
        }
        cout << "\033[0m" << endl;
    }
    cout << "\033[31m" << p2 << ": ";
    print_stones(red, 'O');
    cout << "\033[0m" << endl;
}

void insert_stone(int board[], int index, int stone, int &my_stones, int &enemy_stones, bool &err)
{
    if (board[index] * stone >= -1)
    {
        if (board[index] * stone == -1)
        {
            board[index] = 0;
            enemy_stones++;
        }
        board[index] += stone;
        my_stones--;
    }
    else
    {
        cout << "The chosen place is not possible" << endl;
        err = true;
    }
}

void use_dice(bool current, int &dice_count, int dice[], int &blue, int &red, int board[])
{
    int input = 0;
    bool err = false;
    if (current && blue > 0 || !current && red > 0)
    {
        cout << "Choose \t0 Pass ";
        for (int i = 1; i <= dice_count; i++)
        {
            cout << "\t" << i << " Dice [" << dice[i - 1] << "]";
        }
        cout << ":";
        cin >> input;
        if (input == 0)
        {
            dice_count = 0;
            return;
        }
        else if (input > 0 && input <= dice_count)
        {
            // reduce input to index
            input--;
            int index = 0;
            if (current)
            {
                index = dice[input] - 1;
                // insert at top
                insert_stone(board, index, -1, blue, red, err);
                if(!err)
                {
                    dice[input] = dice[dice_count - 1];
                    dice_count--;
                }
            }
            else
            {
                // insert at the bottom
                index = 24 - dice[input];
                insert_stone(board, index, 1, red, blue, err);
                if(!err)
                {
                    dice[input] = dice[dice_count - 1];
                    dice_count--;
                }
            }
        }
    }
    else
    {
        // draw
        cout << "Choose \t0 Pass\n> ";
        for (int i = 1; i <= dice_count; i++)
        {
            cout << "\t" << i << " Dice [" << dice[i - 1] << "]";
        }
        cout << ":";
        cin >> input;
        if (input == 0)
        {
            dice_count = 0;
            return;
        }
        else if (input > 0 && input <= dice_count)
        {
            // reduce input to index
            input--;
            cout << "select the stone to be moved (line number): " << endl;
            int input2 = 0;
            cin >> input2;
            if (input2 > 0 && input2 < 25)
            {
                // reduce input2 to index
                input2--;
                if (current)
                {
                    if (board[input2] < 0)
                    {
                        if (input2 + dice[input] > 23)
                        {
                            // outside the field
                            bool can_remove = true;
                            for (int i = 0; i < 18; i++)
                            {
                                if (board[i] < 0)
                                {
                                    can_remove = false;
                                    cout << "All the stones must be in the target area" << endl;
                                    err = true;
                                    break;
                                }
                            }
                            if (can_remove)
                            {
                                board[input2]++;
                            }
                        }
                        else
                        {
                            if (board[input2 + dice[input]] <= 1)
                            {
                                // move
                                if (board[input2 + dice[input]] == 1)
                                {
                                    board[input2 + dice[input]] = 0;
                                    red++;
                                }
                                board[input2]++;
                                board[input2 + dice[input]]--;
                                dice[input] = dice[dice_count - 1];
                                dice_count--;
                            }
                            else
                            {
                                cout << "The field is occupied by the opponent" << endl;
                                err = true;
                            }
                        }
                    }
                    else
                    {
                        cout << "There are no own stones on the field" << endl;
                        err = true;
                    }
                }
                else
                {
                    if (board[input2] > 0)
                    {
                        if (input2 - dice[input] < 0)
                        {
                            // outside the field
                            bool can_remove = true;
                            for (int i = 6; i < 24; i++)
                            {
                                if (board[i] > 0)
                                {
                                    can_remove = false;
                                    cout << "All the stones must be in the target area." << endl;
                                    err = true;
                                    break;
                                }
                            }
                            if (can_remove)
                            {
                                board[input2]--;
                            }
                        }
                        else
                        {
                            if (board[input2 - dice[input]] >= -1)
                            {
                                // move
                                if (board[input2 - dice[input]] == -1)
                                {
                                    board[input2 - dice[input]] = 0;
                                    blue++;
                                }
                                board[input2]--;
                                board[input2 - dice[input]]++;
                                dice[input] = dice[dice_count - 1];
                                dice_count--;
                            }
                            else
                            {
                                cout << "The field is occupied by the opponent" << endl;
                                err = true;
                            }
                        }
                    }
                    else
                    {
                        cout << "There are no own stones on the field" << endl;
                        err = true;
                    }
                }
            }
            else
            {
                cout << "The line number is not in the field" << endl;
                err = true;
            }
        }
    }
    if (err)
    {
        cout << "Continue with any input!" << endl;
        string s = "";
        cin >> s;
    }
}

int roll_dice(int dice[])
{
    dice[0] = roll_dice();
    dice[1] = roll_dice();

    if (dice[0] == dice[1])
    {
        dice[2] = dice[3] = dice[0];
        return 4;
    }
    return 2;
}

int main(int argc, char **args)
{
    bool horz = false;
    if (argc < 3)
    {
        cout << "The two player names are required!" << endl;
        return 0;
    }
    if (argc == 4)
    {
        if (strcmp(args[3], "-h") == 0)
        {
            horz = true;
        }
    }
    srand(time(nullptr));
    bool current_player = rand() % 2 == 0;
    int board[] = {-2, 0, 0, 0, 0, 5, 0, 3, 0, 0, 0, -5, 5, 0, 0, 0, -3, 0, -5, 0, 0, 0, 0, 2};
    int blue = 0;
    int red = 0;
    int dice[4] = {0};
    int dice_count = 0;
    int input = 0;
    while (!cin.fail())
    {
        if (horz)
        {
            print_board_2(args[1], args[2], red, blue, board);
        }
        else
        {
            print_board(args[1], args[2], red, blue, board);
        }

        cout << "It is ";
        if (current_player)
        {
            cout << "\033[34m" << args[1];
        }
        else
        {
            cout << "\033[31m" << args[2];
        }
        cout << "\033[0m" << "'s turn" << endl;
        if (dice_count == 0)
        {
            dice_count = roll_dice(dice);
        }
        cout << "Dice: ";
        for (int i = 0; i < dice_count; i++)
        {
            cout << dice[i] << " ";
        }
        cout << endl;
        cout << "Select:\n(1) Dice Draw\t(2) Pass\t(3) End Game:\n> ";
        cin >> input;
        switch (input)
        {
        case 1:
            if (horz)
            {
                print_board_2(args[1], args[2], red, blue, board);
            }
            else
            {
                print_board(args[1], args[2], red, blue, board);
            }
            use_dice(current_player, dice_count, dice, blue, red, board);
            if (dice_count == 0)
            {
                current_player = !current_player;
            }
            break;
        case 2:
            dice_count = 0;
            current_player = !current_player;
            continue;
            break;
        case 3:
            return 0;
            break;
        }
    }
}
