//
// Created by sebdis on 18/05/20.
//

#include <iostream>
#include <fstream>
#include <stack>
#include <set>
#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;

short int row_number;
short int col_number;
short int black_number;
short int white_number;
short int total_rings;

struct casella{
    bool visited = false;
    char color = 0;
    vector<char> moves;
};

//short can become char
struct solution{
    char* path;
    short int rings = 0;
    short int path_index = 0;
    short int path_length = 0;
    short int start_x;
    short int start_y;
};

void init_map(bool ** matrix){
    for (int i = 0; i < row_number; i++){
        for (int j = 0; j < col_number; j++){
            matrix[i][j] = false;
        }
    }
}
void print_map(short int ** matrix){
    for (int i = 0; i < row_number; i++){
        for (int j = 0; j < col_number; j++){
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}


//delete and element in a vector
//vec.erase(std::remove(vec.begin(), vec.end(), int_to_remove), vec.end());

void create_sol(casella** map,short int cur_x, short int cur_y,short int prev_x, short int prev_y, solution* sol){
    if (cur_x == sol->start_x && cur_y == sol->start_y && sol->rings == total_rings){
        return;
    } else if (map[cur_x][cur_y].visited){
        //delete last move
        map[prev_x][prev_y].moves.erase(remove(map[prev_x][prev_y].moves.begin(), map[prev_x][prev_y].moves.end(), sol->path[sol->path_index]), map[prev_x][prev_y].moves.end());

        if (map[prev_x][prev_y].moves.size() == 0){//no more moves available

        } else { // chose the next move
            char next_move = map[prev_x][prev_y].moves.back();
            map[prev_x][prev_y].moves.pop_back();//remove the chosen move
            switch(next_move){
                case 'U':{
                    cur_x = prev_x - 1;break;
                };
                case 'D':{
                    cur_x = prev_x + 1;break;
                };
                case 'L':{
                    cur_y = prev_y - 1;break;
                };
                case 'R':{
                    cur_y = prev_y + 1;break;
                };
            } // update curr_x and curr_y

            sol->path[sol->path_index] = next_move;
            sol->path_index++;

        }
        //sol->path[sol->path_index] = NULL;

    }
}//not finished -> too slow



int main(){



    casella ** map;
    solution soluz;
    ifstream in ("input.txt");
    ofstream out ("output.txt");

    in >> row_number >> col_number >> black_number >> white_number;


    total_rings = black_number + white_number;
    soluz.path = new char[col_number * row_number];
    map = new casella* [row_number];
    for (int i = 0; i < row_number; i++){
        map[i] = new casella[col_number];
        for (int j = 0; j < col_number; j++){
            map[i][j].color = 0;
            if(i < (row_number - 1))
                map[i][j].moves.push_back('D');
            if(j < (col_number - 1))
                map[i][j].moves.push_back('R');
            if(i > 0 && i < row_number)
                map[i][j].moves.push_back('U');
            if(j > 0 && j < col_number)
                map[i][j].moves.push_back('L');
        }
    }

    int x,y;
    for (int i = 0; i < black_number; i++){
        in >> x >> y;
        map[x][y].color = 1;
    }
    for (int i = 0; i < white_number; i++){
        in >> x >> y;
        map[x][y].color = 2;
    }


    create_sol(map, soluz.start_x, soluz.start_y, soluz.start_x, soluz.start_y, &soluz);


    out << soluz.rings << " " << soluz.path_length << " " << soluz.start_x << "" << soluz.start_y << " ";
    for (int i = 0; i < soluz.path_length; i++){
        out << soluz.path[i];
    }
    out << "#";

    out.close();
    in.close();
    return 0;
}












/*
    moves = new char[row_number * col_number];
    map = new short int* [row_number];
    visited = new bool * [row_number];
    for (int i = 0; i < col_number; i++){
        visited[i] = new bool [col_number];
        map[i] = new short int [col_number];
    }
    init_map(visited);

    int x,y;
    for (int i = 0; i < black_number; i++){
        in >> x >> y;
        map[x][y] = 1;
    }
    for (int i = 0; i < white_number; i++){
        in >> x >> y;
        map[x][y] = 2;
    }
    */