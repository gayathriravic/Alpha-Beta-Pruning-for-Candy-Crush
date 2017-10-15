#include<iostream>
#include<queue>
#include<fstream>
#include<algorithm>
using namespace std;
//// GLOBAL VARIABLES//////

int n;
int p;
int a=0;
int c=0; // for index.
char **board1;
char **board;
char **board2;
int *storeRow;
int *storeCol;
int **allPossibleMoves;
int num_stars=0;
int star_count;
int kcount;
int getrow, getcol, getscore;
int *scoreArray;
int *rowArray;
int *colArray;
char ** duplicate_board;
char** duplicate_board1;
int *heur_array;
int **heuristicIndex;
int rowfind;
int colfind;
int h;

//// FUNCTION DEFINITIONS ////


void printBoard(char ** board)
{

    int i,j;
    for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
      {

          cout<<board[i][j]<<"\t";
      }

      cout<<"\n";
    }
}


//Assign heuristics for depth n-1

void assignHeuristic()
{
    int i;
    int j;
    heuristicIndex= new int*[10];
    //cout<<a<<c;
    for(i=0;i<a;i++)
        heuristicIndex[i]= new int[10];
    for(i=0;i<a;i++)
    {
        heuristicIndex[i][0]= storeRow[i];
        heuristicIndex[i][1]= storeCol[i];
        heuristicIndex[i][2]= heur_array[i];
    }


}


// Get the input from the user
char ** getInput()
{

  ifstream infile;
  ofstream outfile;
  int i;
  int j;

  double t;
  infile.open("input.txt");
  infile>>n;

  infile>>p;
  infile>>t;
  board= new char*[n];
  board1= new char*[n];
  board2= new char*[n];


  for(i=0;i<n;i++)
  board[i]= new char[n];

  for(i=0;i<n;i++)
  board1[i]= new char[n];


  for(i=0;i<n;i++)
  {

      for(j=0;j<n;j++)
      {
          infile>>board[i][j];
          if(board[i][j]=='*')
            num_stars++;

      }
  }


  for(i=0;i<n;i++)
  {

      for(j=0;j<n;j++)
      {

          board1[i][j]= board[i][j];
      }
  }



  return board;

}

// Apply Gravity


char** applyGravity(char **board)
{

    cout<<"\n";
    int i;
    int j;
    int temp;
    int k;

    for(j=0;j<n;j++)
    {

        for(i=1;i<n;i++)
        {

            if(board[i][j]=='*' && board[i-1][j]!='*')
            {

                temp = board[i][j];
                board[i][j]= board[i-1][j];
                board[i-1][j]= temp;
            }

        }

        for(k=1;k<n;k++)     // if there are fruits below stars even after executing once.
        {

            if(board[k][j]=='*' && board[k-1][j]!='*')
            {
                j--;
                break;
            }
        }
    }

    return board;
}


char **createDuplicate(char **board)
{
    char **duplicate_board;
    int i;
    int j;
    duplicate_board= new char*[n];
    for(i=0;i<n;i++)
        duplicate_board[i]= new char[n];
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {

            duplicate_board[i][j]= board[i][j];
        }
    }

    return duplicate_board;
}

// Burst elements

char ** burstPropagate(char **board1, int rowBoard, int colBoard)
{
    std::queue<int> scoreList;
    int i;
   // cout<<"\n";
    int type= board1[rowBoard][colBoard];
    scoreList.push(rowBoard);
    scoreList.push(colBoard);
    board1[rowBoard][colBoard]='*';
    int r,c;
    int row, col;
    while(!scoreList.empty())
    {
        row= scoreList.front();
        scoreList.pop();
        col=scoreList.front();
        scoreList.pop();

        r=row-1;
        if(r>=0)
        {
            if(board1[r][col]==type)
            {
                scoreList.push(r);
                scoreList.push(col);
                board1[r][col]='*';
            }

        }
        r=row+1;
        if(r<n)
        {
            if(board1[r][col]==type)
            {
                scoreList.push(r);
                scoreList.push(col);
                board1[r][col]='*';
            }

        }

        c=col-1;

         if(c>=0)
        {
            if(board1[row][c]==type)
            {
                scoreList.push(row);
                scoreList.push(c);
                board1[row][c]='*';
            }

        }

        c=col+1;

         if(c<n)
        {
            if(board1[row][c]==type)
            {
                scoreList.push(row);
                scoreList.push(c);
                board1[row][c]='*';
            }

        }
    }

    board=applyGravity(board1);
    return board;

}


// compute burst score for each cell

int compute_score(int rowBoard, int colBoard, char **duplicate_board)
{
    std:queue<int> scoreList;
    int count=0;
    int row,col;
    int type= duplicate_board[rowBoard][colBoard];

    scoreList.push(rowBoard);
    scoreList.push(colBoard);
    duplicate_board[rowBoard][colBoard]='*';
    count+=1;
    int r,c;

    while(!scoreList.empty())
    {
        row=scoreList.front();
        scoreList.pop();
        col= scoreList.front();
        scoreList.pop();

        r=row-1;
        if(r>=0)
        {
            if(duplicate_board[r][col]==type)
            {
                scoreList.push(r);
                scoreList.push(col);
                count+=1;
                duplicate_board[r][col]='*';
            }

        }
        r=row+1;
        if(r<n)
        {
            if(duplicate_board[r][col]==type)
            {
                scoreList.push(r);
                scoreList.push(col);
                count+=1;
                duplicate_board[r][col]='*';
            }

        }
        c=col-1;

         if(c>=0)
        {
            if(duplicate_board[row][c]==type)
            {
                scoreList.push(row);
                scoreList.push(c);
                count+=1;
                duplicate_board[row][c]='*';
            }

        }

        c=col+1;

         if(c<n)
        {
            if(duplicate_board[row][c]==type)
            {
                scoreList.push(row);
                scoreList.push(c);
                count+=1;
                duplicate_board[row][c]='*';
            }

        }
    }

    return count;
}

// make a move on the board, and calculate scores of all possible moves.

char** makeMove(char **duplicate_board)
{

    int i,j;
    int k=0;
    int row_index, col_index;
    int max_score= -99;
    int score;

    for(i=0;i<n;i++)
    {

        for(j=0;j<n;j++)
        {

            if(duplicate_board[i][j]!='*')
            {

                score= compute_score(i,j,duplicate_board);
                allPossibleMoves[k][0]=i;
                allPossibleMoves[k][1]=j;
                allPossibleMoves[k][2]= score;
                k++;


            }
        }
    }

    kcount=k;

 i=0;
 return board;

}


// count the number of stars/ empty positions in the board.


int findStars(char**board)
{
    star_count=0;
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            if(board[i][j]=='*')
            star_count++;
        }
    }
    return star_count;
}

// find the k maximum moves , which would serve as a branching factor.

void findKMaximumMoves(int **allPossibleMoves)
{

    int i;
    int j=0;

    int temp;
    int temp1, temp2;
    for(i=0;i<kcount-1;i++)
    {
        for(j=0;j<kcount-i-1;j++)
        {
            if(allPossibleMoves[j][2]< allPossibleMoves[j+1][2])
            {
                temp= allPossibleMoves[j][2];
                allPossibleMoves[j][2]= allPossibleMoves[j+1][2];
                allPossibleMoves[j+1][2]= temp;
                temp1=allPossibleMoves[j][0];
                allPossibleMoves[j][0]=allPossibleMoves[j+1][0];
                allPossibleMoves[j+1][0]=temp1;
                temp2=allPossibleMoves[j][1];
                allPossibleMoves[j][1]= allPossibleMoves[j+1][1];
                allPossibleMoves[j+1][1]= temp2;
            }
       }


    }



}

// given the heuristic values, find out the corresponding index [row,col]

void findIndex(int h)
{

int i;

for(i=0;i<a;i++)
{

    if(heuristicIndex[i][2]==h)
    {

        rowfind= heuristicIndex[i][0];
        colfind= heuristicIndex[i][1];
    }
}
}

int num_boards=0;


// Minmax algorithm with alpha-beta pruning

int minimax(char **board, int depth, int row, int col, int player, int propagation_value, int num_stars, int alphaa, int betaa)
{
    int i,j;
    int v;
    int min_score;
    int max_score;
    int value; //score.
    star_count= num_stars;
    int alpha,beta;
    int prop_value= propagation_value;
    int heuristic_value;

    if(depth<0 || star_count==n*n)
       {

         heuristic_value= prop_value;
         return heuristic_value;
       }

     if(depth>=0)
       {

         char **duplicate_board;
         duplicate_board= createDuplicate(board);  // create a duplicate copy of the board.
         duplicate_board=makeMove(duplicate_board);
         num_boards++;
         //cout<<"Finding k maximum moves\n";
         findKMaximumMoves(allPossibleMoves);

         if(player == -1)
           {
               heuristic_value = 99999;
               beta= 9999999;
           }
         else
           {
               heuristic_value = -99999;
               alpha= -999999;

           }

         for(i=0;i<3;i++)  // BRANCHING FACTOR (k in the k maximums)
         {

             char **duplicate_board1;    // create a duplicate board.
             duplicate_board1= createDuplicate(board);
             if(i!=0 && (num_stars<n*n))
             {
                 duplicate_board1= makeMove(duplicate_board1);
                 duplicate_board1= createDuplicate(board);
             }

             findKMaximumMoves(allPossibleMoves);

             if(depth==3 && kcount-1>=i)
             {
              storeRow[a]= allPossibleMoves[i][0];
              storeCol[a]= allPossibleMoves[i][1];
              a++;
             }

             else if(depth==3 && (kcount==1 || kcount<=i ))
             {

                storeRow[a]= allPossibleMoves[0][0];
                storeCol[a]= allPossibleMoves[0][1];
                a++;
             }
             if(kcount-1>=i)
             duplicate_board1= burstPropagate(duplicate_board1,allPossibleMoves[i][0],allPossibleMoves[i][1]);
             else if(kcount==1 || kcount<=i)
                duplicate_board1= burstPropagate(duplicate_board1,allPossibleMoves[0][0],allPossibleMoves[0][1]);// make a move for this row and col.
             star_count= findStars(duplicate_board1);
             if(player==-1)
             {
                 if(kcount-1>=i)
                    value= (allPossibleMoves[i][2])*(allPossibleMoves[i][2]);
                 else if(kcount==1|| kcount<=i)
                    value= (allPossibleMoves[0][2])*(allPossibleMoves[0][2]);

                  if(depth==3)
                  {
                    heur_array[c]=v;

                    c++;
                  }
                  if(kcount-1>=i)
                   v=minimax(duplicate_board1,depth-1,allPossibleMoves[i][0],allPossibleMoves[i][1],1,(prop_value-value),star_count,alpha,beta);
                  else if(kcount==1 || kcount<=i)
                    v=minimax(duplicate_board1,depth-1,allPossibleMoves[0][0],allPossibleMoves[0][1],1,(prop_value-value),star_count, alpha, beta);

                     if(heuristic_value>v)
                    {

                        heuristic_value= v;
                    }

                     if(beta>v)
                        {
                            beta=v;

                        }
                 if(alpha>=beta)
                    break;


             }
             else if(player==1)
             {
                 if(kcount-1>=i)
                    value= (allPossibleMoves[i][2])*(allPossibleMoves[i][2]);
                 else if(kcount==1 || kcount <=i)
                    value= (allPossibleMoves[0][2])*(allPossibleMoves[0][2]);
                 if(kcount-1>=i)
                    v= minimax(duplicate_board1,depth-1,allPossibleMoves[i][0],allPossibleMoves[i][1],-1,(prop_value+value),star_count,alpha,beta);
                 else if(kcount==1 || kcount<=i)
                    v= minimax(duplicate_board1,depth-1,allPossibleMoves[0][0],allPossibleMoves[0][1],-1,(prop_value+value),star_count,alpha, beta);

                 if(v>alpha)
                 {
                     alpha=v;
                 }

                if(heuristic_value<v)
                   {
                       heuristic_value=v;

                   }

                    if(depth==3) //&& heuristic_value<v)
                  {
                    heur_array[c]=v;

                    c++;
                  }

             }

             if(alpha>=beta)
                break;
         }

         if(depth!=3)
         return heuristic_value;
        if(depth==3)
         {
             assignHeuristic();
             h= heuristic_value;

             findIndex(h);
             return heuristic_value;   // rowfind and colfind will contain the required index.
         }
       }

}

// caller function

void callToMinimax(char  **board)
{

    int i;
    int depth=3;
    int value;

    value=minimax(board,depth,0,0,1,0,0,-999999,9999999);  // call to minimax.
}

// returns a character for the column number

char intToAlphabet( int i )
{
   char a=(char)('A' + i);
   return a;
}

int main()
{
   int i,j;
   allPossibleMoves= new int*[26*26];
   for(i=0;i<26*26;i++)
    allPossibleMoves[i]= new int[26*26];
    board=getInput();
    storeRow= new int[n];
    storeCol= new int[n];
    heur_array= new int[n];
    if(num_stars!=n*n)
    callToMinimax(board);
    else
    {

         ofstream outfile;
        outfile.open("output.txt");
        outfile<<"No Move"<<"\n";
        return 0;
    }
    board=getInput(); // generate a new board.
    char charcolfind;

    charcolfind= intToAlphabet(colfind);
    board=burstPropagate(board,rowfind,colfind);
    printBoard(board);
    ofstream outfile;
     outfile.open("output.txt");
     outfile<<charcolfind<<rowfind+1<<"\n";
     for(i=0;i<n;i++)
     {
         for(j=0;j<n;j++)
         {
             outfile<<board[i][j];
         }
         outfile<<"\n";
     }
}

