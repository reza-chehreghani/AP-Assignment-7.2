#pragma once

#include "config.hpp"
#include "tools.hpp"

typedef int WeekNum;
typedef int Price;

typedef int NumGoalsScored;
typedef int NumGoalsAssists;

enum PlayerPost {
    goalkeeper,
    defender,
    midfielder,
    forward,
    all
};

enum PlayerStatus {
    injured,
    yellow_card,
    red_card,
    scorer
};

enum PostIn433 {
    glkeeper,
    def_left,
    def_cen_left,
    def_cen_right,
    def_right,
    midfl_left,
    midfl_cen,
    midfl_right,
    forwr_left,
    forwr_tip,
    forwr_right
};

enum MatchResultstatus {
    win,
    lose,
    equal
};

struct PlayerMatchInfo {
    // PostIn433 post;
    bool injured;
    bool yellow_card;
    bool red_card;
    MatchResultstatus result;
    pair<NumGoalsScored, NumGoalsAssists> num_gaols_and_assists;
    // vector<PostIn433> against_goals;
    int num_against_goals;
    int num_against_goals_from_this_post_side;
    int own_goals;
};

struct PlayerInfo {
    Name name;
    string role;
    float avg_score;
    int cost;
    int goals;
    int assists;
    int clean_sheets;
};

class Player {
    string name;
    int price;
    // float score;
    vector<float> weeks_score;
    int last_week_point;
    pair<bool, WeekNum> injured;
    int num_yellow_card;
    bool red_card;
    bool last_week_played;


    float get_avarage_score();

    void set_week_score(const PlayerMatchInfo &match_info);
    int get_result_status_point(MatchResultstatus result);
    virtual int calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals) = 0;
    int get_goals_against_of_this_post_side_point(int num_against_goals_from_this_post_side);
    int get_own_goals_point(int own_goals);
    float calc_score_from_point(int point);

    virtual string get_post() = 0;
    virtual int get_num_matches_without_against_goal() { return NULL_INT; }
    virtual int get_num_scored_goals() { return NULL_INT; }
    virtual int get_num_goals_assist() { return NULL_INT; }

protected:
    Player(string name_, int price_);

    PlayerInfo GET_player_info();
    void check_then_replace(PlayerScore &player_score, int week_num);

    ostringstream if_can_buy(Player* &post_fantasy_player, int fantasy_team_budget);

public:
    bool is_bitween(const vector<string> &names);
    
    void POST_pass_week();
    void apply_match_info(PlayerMatchInfo match_info);

    virtual void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) = 0;

    virtual void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) = 0;

    float get_last_week_score();
    float get_last_week_score_as_captain();
    string get_name() { return name; }

    bool is_name(string name_) { return name == name_; }

    bool did_last_week_play() { return last_week_played; }
    bool can_next_week_play();

    virtual ostringstream if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget) = 0;

    int get_price() { return price; }
};

class GoalKeeper : public Player {
    int num_matches_without_against_goal;

    int calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals);

    string get_post() { return "gk"; }
    int get_num_matches_without_against_goal() { return num_matches_without_against_goal; }

public:
    GoalKeeper(string name, int price);

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget);
};

class Defender : public Player {
    int num_matches_without_against_goal;
    int num_scored_goals;
    int num_goals_assist;

    PlayerScore& find_fewer(PlayerScore& first, PlayerScore& second);
    void sort(PlayerScore& first, PlayerScore& second);

    int calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals);

    string get_post() { return "df"; }
    int get_num_matches_without_against_goal() { return num_matches_without_against_goal; }
    int get_num_scored_goals() { return num_scored_goals; }
    int get_num_goals_assist() { return num_goals_assist; }

public:
    Defender(string name, int price);

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget);
};

class Midfielder : public Player {
    int num_matches_without_against_goal;
    int num_scored_goals;
    int num_goals_assist;

    int calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals);

    string get_post() { return "md"; }
    int get_num_matches_without_against_goal() { return num_matches_without_against_goal; }
    int get_num_scored_goals() { return num_scored_goals; }
    int get_num_goals_assist() { return num_goals_assist; }

public:
    Midfielder(string name, int price);

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget);
};

class Forward : public Player {
    int num_scored_goals;
    int num_goals_assist;

    int calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals);

    string get_post() { return "fw"; }
    int get_num_scored_goals() { return num_scored_goals; }
    int get_num_goals_assist() { return num_goals_assist; }

public:
    Forward(string name, int price);

    void GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post);

    void check_then_add(TeamOfTheWeek &team_of_the_week, int week_num);

    ostringstream if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget);
};