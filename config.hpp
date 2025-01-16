#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <cmath>

using  namespace std;

const string PREMIER_LEAGUE_FILE_DIRECTORY = "data/premier_league.csv";
const string WEEKS_STATS_FILE_DIRECTORY = "data/weeks_stats/week_";
const char CSV_DELIMITER_INSIDE_COLUMN = ';';
const char CSV_DELIMITER_FOR_PAIR_ELEMETS = ':';
const string RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST = "OK\n";
const string RESPONSE_TO_EMPTY_LIST = "Empty\n";
const int WIN_POINTS = 3, EQUAL_POINTS = 1;
const int CSV_NUM_COL_TEAMS = 0;
const int CSV_NUM_COL_RESULTS = 1;
const int CSV_NUM_COL_INJURED = 2;
const int CSV_OFFSET_PLAYERS = 1;
const int CSV_NUM_COL_TEAM_NAME = 0;
const int CSV_NUM_COL_PLAYERS_SCORE_MINUS_COL_NAME_AND_RESULT = 3;
const int CSV_NUM_COL_TEAM1_FORMATION = 6;
const int CSV_NUM_COL_TEAM2_FORMATION = 7;
const int CSV_OFFSET_TEAMS_AND_RESULT_COL = 2;
const int NUM_OF_INJURED_WEEKS = 3;
const int NUM_OF_YELLOW_CARD_FOR_SUSPENSION = 3;
const char DELEMETER_BETWEEN_ORDER_AND_ARGUMENT = '?';
const char TEAM_NAME_DELEMETER = '_';
const string NULL_STRING = "";
const int NUM_OF_DECIMAL_PLACE = 1;
const char DOT = '.';
const string ADMIN_USERNAME = "admin";
const string ADMIN_PASSWORD = "123456";
const int NUM_OF_FANTASY_TEAM_PLAYERS = 5;
const bool TRASFER_WINDOW_BEGINING_STATUS = true;
const int MAX_PLAYER_TRANSFER = 2;
const string ANSWER_CAN_NOT_BUY_PLAYER_BECAUSE_CAN_NOT_PLAY_NEXT_WEEK = "This player is not available for next week\n";
const string OWN_GOAL = "OWN_GOAL";
const int TEAM_WIN_POINT = 5;
const int TEAM_LOSE_POINT = -1;
const int TEAM_EQUAL_POINT = 1;
const int OWN_GOAL_POINT = -3;
const int GOALKEEPER_WITHOUT_AGAINST_GOAL_POINT = 5;
const int GOALKEEPER_AGAINST_GOAL_POINT = -1;
const int GOALKEEPER_BASE_POINT = 3;
const int DEFENDER_WITHOUT_AGAINST_GOAL_POINT = 2;
const int DEFENDER_SCORED_GOAL_POINT = 4;
const int DEFENDER_GOAL_ASSIST_POINT = 3;
const int DEFENDER_BASE_POINT = 1;
const int MIDFIELDER_WITHOUT_AGAINST_GOAL_POINT = 1;
const int MIDFIELDER_SCORED_GOAL_POINT = 3;
const int MIDFIELDER_GOAL_ASSIST_POINT = 2;
const int MIDFIELDER_BASE_POINT = 0;
const int FORWARD_SCORED_GOAL_POINT = 3;
const int FORWARD_GOAL_ASSIST_POINT = 1;
const int FORWARD_WITHOUT_SCORED_GOAL_POINT = -1;
const int FOARWARD_BASE_POINT = 0;
const int DEDUCT_LEFT_DEFENDER_GOAL_AGAINST_POINT = -1;
// const int DEDUCT_RIGHT_DEFENDER_GOAL_AGAINST_POINT = -1;
// const int DEDUCT_MID_DEFENDER_GOAL_AGAINST_POINT = -1;
// const int DEDUCT_MIDFIELDER_GOAL_AGAINSTD_POINT = -1;
const int NUM_FOOTBALL_PLAYER_IN_A_MATCH = 11;
const int DEDUCT_GOAL_AGAINST_OF_THIS_POST_SIDE_POINT = -1;
const int INIT_BUDGET = 2500;
const int NULL_INT = -1;

enum Sequence {
    first,
    second
};

enum FantasyPlayerPost {
    Goalkeeper,
    Defender1,
    Defender2,
    Midfielder,
    Striker
};

typedef string Name;
typedef float Score;
typedef pair<Name, Score> PlayerScore;

struct TeamOfTheWeek {
    PlayerScore goal_keeper = {"" , -1.f};
    PlayerScore defender1 = {"" , -1.f};
    PlayerScore defender2 = {"" , -1.f};
    PlayerScore mid_fielder = {"" , -1.f};
    PlayerScore forward = {"" , -1.f};
};

class BadRequest : invalid_argument{
public:
    BadRequest(const char *ex);
};

class NotFound : invalid_argument {
public:
    NotFound(const char *ex);
};

class PermissionDenied : invalid_argument {
public:
    PermissionDenied(const char *ex);
};