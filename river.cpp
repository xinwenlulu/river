#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

using namespace std;

#include "river.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM_ERROR:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

char** make_river_scene(const char* left, const char* boat){
  //initialise background
  char **scene = create_scene();
  add_to_scene(scene, 0, 0,"bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 3, 30, "sun.txt");
  add_to_scene(scene, 19, 19, "river.txt");

  int num_M_left = 0, num_C_left = 0;
  bool boatleft = false;
  if (*left != '\0'){
    for(int i = 0; i< strlen(left); i++){
      if (left[i] == 'M'){ num_M_left++; }
      if (left[i] == 'C'){ num_C_left++; }
      if (left[i]=='B'){ boatleft = true; }
    }
  }

  if (num_M_left > 0 ){
    switch(num_M_left){
      case 3: add_to_scene(scene, 2, 13, "missionary.txt");
      case 2: add_to_scene(scene, 2, 7, "missionary.txt");
      case 1: add_to_scene(scene, 2, 1, "missionary.txt");
    }
  }

  if (num_C_left >0){
    switch(num_C_left){
      case 3: add_to_scene(scene,11, 13, "cannibal.txt");
      case 2: add_to_scene(scene,11, 7, "cannibal.txt");
      case 1: add_to_scene(scene,11, 1, "cannibal.txt");
    }
  }

  int m_boat = 0, c_boat = 0, capacity = 2;
  if (boatleft == false){
    //draw boat on the right
    add_to_scene(scene, 17, 36, "boat.txt");
    if (*boat != '\0'){
      for(int i = 0; i< strlen(boat); i++){
        if(boat[i]== 'M' && capacity == 2){
          add_to_scene(scene,11, 39, "missionary.txt");
          m_boat++;
          capacity--;
        } else if(boat[i]== 'M' && capacity == 1){
          add_to_scene(scene,11, 45, "missionary.txt");
          m_boat++;
          capacity--;
        } else if(boat[i]== 'C' && capacity == 2){
          add_to_scene(scene,11, 39, "cannibal.txt");
          c_boat++;
          capacity--;
        } else if(boat[i]== 'C' && capacity == 1){
          add_to_scene(scene,11, 45, "cannibal.txt");
          c_boat++;
          capacity--;
        }
      }
    }
  }else{
    // boat on the left
    add_to_scene(scene, 17, 19, "boat.txt");
    if (*boat != '\0'){
      for(int i = 0; i< strlen(boat); i++){
        if(boat[i]== 'M' && capacity == 2){
          add_to_scene(scene,11, 22, "missionary.txt");
          m_boat++;
          capacity--;
        } else if(boat[i]== 'M' && capacity == 1){
          add_to_scene(scene,11, 28, "missionary.txt");
          m_boat++;
          capacity--;
        } else if(boat[i]== 'C' && capacity == 2){
          add_to_scene(scene,11, 22, "cannibal.txt");
          c_boat++;
          capacity--;
        } else if(boat[i]== 'C' && capacity == 1){
          add_to_scene(scene,11, 28, "cannibal.txt");
          c_boat++;
          capacity--;
        }
      }
    }
  }

  int num_M_right = 3-num_M_left-m_boat, num_C_right = 3-num_C_left-c_boat;

  if (num_M_right > 0 ){
    switch(num_M_right){
      case 3: add_to_scene(scene, 2, 66, "missionary.txt");
      case 2: add_to_scene(scene, 2, 60, "missionary.txt");
      case 1: add_to_scene(scene, 2, 54, "missionary.txt");
    }
  }

  if (num_C_right >0){
    switch(num_C_right){
      case 3: add_to_scene(scene,11, 66, "cannibal.txt");
      case 2: add_to_scene(scene,11, 60, "cannibal.txt");
      case 1: add_to_scene(scene,11, 54, "cannibal.txt");
    }
  }

  //add_to_scene(scene, 12, 20, "pot.txt");
  return scene;
}

Status perform_crossing(char* left, const char* targets){
  cout << left << endl;
  //on one on boat
  if (*targets == '\0'){ return ERROR_INVALID_MOVE; }
  //more than 2 people on boat
  if (strlen(targets) > 2){return ERROR_INVALID_MOVE; }

  int num_M_left = 0, num_C_left = 0, m_target = 0, c_target = 0;
  bool target_right = false;
  for (int i = 0; i < strlen(left); i++){
    if (left[i] == 'M'){ num_M_left++; }
    if (left[i] == 'C'){ num_C_left++; }
    if (left[i] == 'B'){ target_right = true; }
  }
  if (num_M_left > 3){ return ERROR_INVALID_MISSIONARY_COUNT; }
  if (num_C_left > 3){ return ERROR_INVALID_CANNIBAL_COUNT; }
  
  int m_right = 3-num_M_left, c_right = 3-num_C_left;
  
  for (int i = 0; i < strlen(targets); i++){
    if (targets[i] == 'M'){ m_target++; }
    else if (targets[i] == 'C'){ c_target++; }
    else{ return ERROR_INVALID_MOVE; }
  }
  if (target_right == true && m_target > num_M_left){ return ERROR_INVALID_MISSIONARY_COUNT; } 
  if (target_right == true && c_target > num_C_left){ return ERROR_INVALID_CANNIBAL_COUNT; }
  

  if (target_right == false && m_target > m_right){ return ERROR_INVALID_MISSIONARY_COUNT; }
  if (target_right == false && c_target > c_right){ return ERROR_INVALID_CANNIBAL_COUNT; }

  
  if (target_right == true){
    num_M_left -= m_target;
    num_C_left -= c_target;
  }else{ //right to left
    m_right -= m_target;
    c_right -=c_target;
  }
 
  //draw loading the boat
  cout << "Loading the boat..." << endl;
  if (target_right == true){
    strcpy(left,"B");
    for(int i = 0; i < num_M_left; i++){
      strcat(left,"M");
    }
    for(int i = 0; i < num_C_left; i++){
      strcat(left,"C");
    }
  }
  char **scene = make_river_scene(left, targets);
  print_scene(scene);  

  //draw crossing
  cout << "\n\nCrossing the river..." << endl;
  if (target_right == true){
    strcpy(left,"");
    for(int i = 0; i < num_M_left; i++){
      strcat(left,"M");
    }
    for(int i = 0; i < num_C_left; i++){
      strcat(left,"C");
    }
  }else{
    strcat(left,"B");
  }
  scene = make_river_scene(left, targets);
  print_scene(scene);  

  //update results
  cout << "\n\nUnloading the boat..." << endl;

  if (target_right == true){
    scene = make_river_scene(left, "");
    print_scene(scene);  
    m_right += m_target;
    m_target = 0;
    c_right += c_target;
    c_target = 0;
    if ( m_right > 0 ){
      if (m_right < c_right){ return ERROR_MISSIONARIES_EATEN; }
    }
    if (num_M_left > 0){
      if (num_M_left < num_C_left){ return ERROR_MISSIONARIES_EATEN; }
    }
  }else{
    num_M_left += m_target;
    m_target = 0;
    num_C_left += c_target;
    c_target = 0;
    strcpy(left,"B");
    for(int i = 0; i < num_M_left; i++){
      strcat(left,"M");
    }
    for(int i = 0; i < num_C_left; i++){
      strcat(left,"C");
    }
    scene = make_river_scene(left, "");
    print_scene(scene);  

    if ( m_right > 0 ){
      if (m_right < c_right){ return ERROR_MISSIONARIES_EATEN; }
    }

    if ( num_M_left > 0 ){
      if (num_M_left < num_C_left){ return ERROR_MISSIONARIES_EATEN; }
    }
  }

  if(num_M_left == 0 && num_C_left == 0){
    strcpy(left, ""); 
    return VALID_GOAL_STATE;
  }

  return VALID_NONGOAL_STATE;
}

Status play_game(){
  char left[8];
  strcpy(left,"BCCCMMM");
  //initialise game
  char **scene = make_river_scene(left,"");
  print_scene(scene);

  char target[3];
  Status s = VALID_NONGOAL_STATE;
  while (s == VALID_NONGOAL_STATE){
    cout << "Please specify crossing targets: M for missonary and C for Cannibal!" <<endl;
    cin >> target;
    s = perform_crossing(left, target);
  }
  if (s == VALID_GOAL_STATE){ cout << "Congratulations!" << endl; }
  return s;
}

Status find_solution(char* left, char* answer){
  if (strcmp(left,"")==0){ answer[strlen(answer)-1] = '\0'; return VALID_GOAL_STATE; }
  char tmp[10];
  char answertmp[512];
  char move[3]; // previous move
  if (strcmp(answer,"")!=0){
    last_move(answer,move);
  }
  Status s;

  //try M
  if(strcmp("M",move)!=0){
    strcpy(tmp,left);
    strcpy(answertmp,answer);
    s = perform_crossing(left, "M");
    if (s==VALID_NONGOAL_STATE || s == VALID_GOAL_STATE){
      strcat(answer,"M,");
      s = find_solution(left, answer);
      if (s == VALID_GOAL_STATE) { return VALID_GOAL_STATE; }
    } 
    //undo M
    strcpy(left,tmp);
    strcpy(answer,answertmp);
  }

  //try MM
  if (strcmp("MM",move)!=0){
    strcpy(tmp,left);
    strcpy(answertmp,answer);
    s = perform_crossing(left, "MM");
    if (s==VALID_NONGOAL_STATE || s == VALID_GOAL_STATE){
      strcat(answer,"MM,");
      s = find_solution(left, answer);
      if (s == VALID_GOAL_STATE) { return VALID_GOAL_STATE; }
    } 
    //undo MM
    strcpy(left,tmp);
    strcpy(answer,answertmp);
  }

   //try MC
  if (strcmp("MC",move)!=0){
    strcpy(tmp,left);
    strcpy(answertmp,answer);
    s = perform_crossing(left, "MC");
    if (s==VALID_NONGOAL_STATE || s == VALID_GOAL_STATE){
      strcat(answer,"MC,");
      s = find_solution(left, answer);
      if (s == VALID_GOAL_STATE) { return VALID_GOAL_STATE; }
    } 
    //undo MC
    strcpy(left,tmp);
    strcpy(answer,answertmp);
  }
 
  //try CC
  if (strcmp("CC",move) != 0){
    strcpy(tmp,left);
    strcpy(answertmp,answer);
    s = perform_crossing(left, "CC");
    if (s==VALID_NONGOAL_STATE || s == VALID_GOAL_STATE){
      strcat(answer,"CC,");
      s = find_solution(left, answer);
      if (s == VALID_GOAL_STATE) { return VALID_GOAL_STATE; }
    } 
    //undo CC
    strcpy(left,tmp);
    strcpy(answer,answertmp);
  }
  

  //try C
  if (strcmp("C",move)!=0){
    strcpy(tmp,left);
    strcpy(answertmp,answer);
    s = perform_crossing(left, "C");
    if (s==VALID_NONGOAL_STATE || s == VALID_GOAL_STATE){
      strcat(answer,"C,");
      s = find_solution(left, answer);
      if (s == VALID_GOAL_STATE) { return VALID_GOAL_STATE; }
    } 
    //undo C
    strcpy(left,tmp);
    strcpy(answer,answertmp);
  }

  //exhausted all possibilities
  strcpy(answer,"NO SOLUTION!");
  return ERROR_BONUS_NO_SOLUTION;
}

void last_move(const char* answer, char* move){
  int length = strlen(answer);
  int index = 0;
  for (int i = length-2; i >= 0; i--){
    if (answer[i] == ','){
      index = i+1;
      break;
    }
  }

  move[0] = answer[index];
  if (length - index == 3){ 
    move[1] = answer[index+1];
    move[2] = '\0';
  } else{ 
    move[1] = '\0'; 
  }
}