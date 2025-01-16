#pragma once

#include "config.hpp"
#include "premierLeague.hpp"
#include "tools.hpp"

typedef string Name;
typedef float Point;

class FantasyTeam {
    string name;
    string password;
    float point;
    vector<Player*> players;
    Player* captain;
    int num_of_sells, num_of_buys;
    bool has_played_any_game;
    bool has_passed_week;
    int budget;

    void set_week_point();
    bool is_complete();
    bool did_all_players_last_week_played();

    ostringstream make_squad();

    // bool has_played_any_game() { return point != 0.0; }
    string get_player_name(istringstream& input);
    Player*& find_fantasy_player(string player_name);

    int get_team_cost();
    
public:
    FantasyTeam(string name_, string password_);

    bool is_name(string name_) const { return name == name_; }

    void POST_login(string password_);

    pair<Name, Point> GET_user_ranking();

    void POST_pass_week();

    ostringstream GET_squad();

    ostringstream POST_sell_player(istringstream& input);

    ostringstream POST_buy_player(istringstream &input, const PremierLeague* premier_league);

    ostringstream POST_set_captain(istringstream &input);

    ostringstream GET_show_budget() { return ostringstream(to_string(budget) + '\n'); }
};