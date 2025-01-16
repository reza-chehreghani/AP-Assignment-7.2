#pragma once

#include "config.hpp"
#include "player.hpp"

typedef void (Player:: *UpdateStatus)();
typedef void (Player:: *UpdateScore)(int);

typedef bool Ranks;

typedef string Scorer;
typedef string ScorerAssist;

struct TeamMatchInfo {
    int num_goals_for;
    int num_goals_against;
    vector<string> injureds;
    vector<string> yellow_cards;
    vector<string> red_cards;
    // vector<PlayerScore> players_score;
    vector<Name> formation;
    vector<pair<Scorer, ScorerAssist>> goals_for;
    vector<PostIn433> goals_against;
    vector<Name> own_goals;
};

struct TeamInfo {
    string name;
    int score;
    int num_goals_for;
    int num_goals_against;
};

class Team {
    string name;
    int score;
    int num_goals_for;
    int num_goals_against;
    vector<shared_ptr<Player>> players;

    void init_players(vector<vector<pair<Name, Price>>> posts_players_name);
    template<typename PlayerPost> void init_posted_players(vector<pair<Name, Price>> posted_players_name);

    MatchResultstatus set_score_and_get_match_result_status(int num_goals_for_, int num_goals_against_);
    void update_new_week_players_info(const TeamMatchInfo& match_info, MatchResultstatus match_result_status);
    vector<int> get_num_against_goals_from_post_players_side(vector<PostIn433> goals_against);
    void update_new_week_player_info(Player* player, const TeamMatchInfo& match_info, int num_against_goals_from_this_post_side, MatchResultstatus match_result_status);
    pair<NumGoalsScored, NumGoalsAssists> get_goals_and_assists(const vector<pair<Scorer, ScorerAssist>> &match_goals, Player* player);

    vector<PlayerInfo> GET_post_players(PlayerPost player_post);
    static bool compare_players_by_score(const PlayerInfo &left, const PlayerInfo &right);
    static bool compare_players_by_name(const PlayerInfo &left, const PlayerInfo &right);
    // static string extract_score_from_player_info(const string &player_info);
    ostringstream make_list_of_players(const vector<PlayerInfo>& players);

public:
    Team(string name_, vector<vector<pair<Name, Price>>> players_name);
  
    void POST_pass_week(const TeamMatchInfo& match_info);

    bool is_name(string name_) { return name == name_; }

    void check_then_add_player(TeamOfTheWeek& team_of_the_week, int week_num);

    ostringstream GET_players(pair<PlayerPost, Ranks> post_and_ranks);

    TeamInfo get_team_info();

    Player* find_player(string player_name) const;
};