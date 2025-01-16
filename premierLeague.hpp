#pragma once

#include "config.hpp"
#include "team.hpp"
#include "weekStats.hpp"


class PremierLeague {
    vector<Team> teams;
    vector<WeekStats> weeks_stats;
    
    void init_teams();
    Team make_team(vector<string> team_elements);
    vector<pair<Name, Price>> make_post_players(vector<string> post_players_name_price);

    Match make_match(string teams_name, string result);
    void update_match_teams(Match match, vector<string> match_players_elemets);
    // vector<PlayerScore> make_players_score(vector<string> players_score_elements);
    vector<pair<Scorer, ScorerAssist>> make_goals_with_assists(vector<string> goals_with_assists);
    void separate_goals(const vector<pair<Scorer, ScorerAssist>> &goals,
                        vector<Name> team1_formation, vector<Name> team2_formation,
                        vector<pair<Scorer, ScorerAssist>> &team1_goals_for, vector<pair<Scorer, ScorerAssist>> &team2_goals_for,
                        vector<PostIn433> &team1_goals_against, vector<PostIn433> &team2_goals_against,
                        vector<Name> &team1_own_goals, vector<Name> &team2_own_goals);
    Team& find_team(string name);

    int get_week_num(istringstream &input);
    ostringstream make_team_of_week(TeamOfTheWeek team_of_the_week);

    Team& get_team(istringstream &input);
    pair<PlayerPost, Ranks> get_post_and_ranks(istringstream &input);
    PlayerPost find_post(string post);

    static bool compare_teams(const TeamInfo& left, const TeamInfo& right);
    ostringstream make_league_standings(const vector<TeamInfo>& teams_info);

public:
    PremierLeague();

    ostringstream GET_team_of_the_week(istringstream &input);

    ostringstream GET_players(istringstream &input);

    ostringstream GET_league_standings();

    ostringstream GET_matches_result_league(istringstream &input);

    ostringstream POST_pass_week();

    Player* find_player(string player_name) const;
};