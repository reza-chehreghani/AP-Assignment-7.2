#include "team.hpp"

Team::Team(string name_, vector<vector<pair<Name, Price>>> players_name)
    : name(name_), score(0), num_goals_for(0), num_goals_against(0) {
    init_players(players_name);
}

void Team::init_players(vector<vector<pair<Name, Price>>> posts_players_name) {
    init_posted_players<class::GoalKeeper>(posts_players_name[PlayerPost::goalkeeper]);
    init_posted_players<class::Defender>(posts_players_name[PlayerPost::defender]);
    init_posted_players<class::Midfielder>(posts_players_name[PlayerPost::midfielder]);
    init_posted_players<class::Forward>(posts_players_name[PlayerPost::forward]);
}

template<typename PlayerPost> void Team::init_posted_players(vector<pair<Name, Price>> posted_players_name) {
    for (auto posted_player_name : posted_players_name)
        players.push_back(make_shared<PlayerPost>(PlayerPost(posted_player_name.first, posted_player_name.second)));
}

void Team::POST_pass_week(const TeamMatchInfo& match_info) {
    num_goals_for += match_info.num_goals_for;
    num_goals_against += match_info.num_goals_against;

    MatchResultstatus match_result_status = set_score_and_get_match_result_status(match_info.num_goals_for, match_info.num_goals_against);

    for (auto& player : players)
        player->POST_pass_week();

    update_new_week_players_info(match_info, match_result_status);
}

MatchResultstatus Team::set_score_and_get_match_result_status(int num_goals_for_, int num_goals_against_) {
    if (num_goals_for_ > num_goals_against_) {
        score += WIN_POINTS;
        return MatchResultstatus::win;
    }
    else if (num_goals_for_ == num_goals_against_) {
        score += EQUAL_POINTS;
        return MatchResultstatus::equal;
    }

    return MatchResultstatus::lose;
}

void Team::update_new_week_players_info(const TeamMatchInfo& match_info, MatchResultstatus match_result_status) {
    vector<int> num_against_goals_from_post_players_side = get_num_against_goals_from_post_players_side(match_info.goals_against);

    for (PostIn433 post = glkeeper; post < NUM_FOOTBALL_PLAYER_IN_A_MATCH; post = (PostIn433)((int)post + 1))
        update_new_week_player_info(find_player(match_info.formation[post]), match_info, num_against_goals_from_post_players_side[post], match_result_status);
}

vector<int> Team::get_num_against_goals_from_post_players_side(vector<PostIn433> goals_against) {
    vector<int> num_against_goals_from_post_players_side(NUM_FOOTBALL_PLAYER_IN_A_MATCH, 0);

    for (auto goal_against : goals_against)
        switch (goal_against) {
        case forwr_right:
        case def_right:
            num_against_goals_from_post_players_side[def_left]++;
            break;
        case forwr_left:
        case def_left:
            num_against_goals_from_post_players_side[def_right]++;
            break;
        case forwr_tip:
        case def_cen_left:
        case def_cen_right:
            num_against_goals_from_post_players_side[def_cen_left]++;
            num_against_goals_from_post_players_side[def_cen_right]++;
            break;
        case midfl_left:
        case midfl_cen:
        case midfl_right:
            num_against_goals_from_post_players_side[midfl_left]++;
            num_against_goals_from_post_players_side[midfl_cen]++;
            num_against_goals_from_post_players_side[midfl_right]++;
            break;
        }

    return num_against_goals_from_post_players_side;
}

void Team::update_new_week_player_info(Player* player, const TeamMatchInfo& match_info, int num_against_goals_from_this_post_side, MatchResultstatus match_result_status) {
    player->apply_match_info(PlayerMatchInfo{
            player->is_bitween(match_info.injureds),
            player->is_bitween(match_info.yellow_cards),
            player->is_bitween(match_info.red_cards),
            match_result_status,
            get_goals_and_assists(match_info.goals_for, player),
            (int)(match_info.goals_against.size() + match_info.own_goals.size()),
            num_against_goals_from_this_post_side,
            (int)count(match_info.own_goals.begin(), match_info.own_goals.end(), player->get_name())
        });
}

pair<NumGoalsScored, NumGoalsAssists> Team::get_goals_and_assists(const vector<pair<Scorer, ScorerAssist>> &match_goals, Player* player) {
    pair<NumGoalsScored, NumGoalsAssists> num_player_gaols_and_assists {0, 0};

    for (auto match_goal : match_goals) {
        if (player->is_name(match_goal.first))
            num_player_gaols_and_assists.first++;

        if(player->is_name(match_goal.second))
            num_player_gaols_and_assists.second++;
    }

    return num_player_gaols_and_assists;
}

void Team::check_then_add_player(TeamOfTheWeek& team_of_the_week, int week_num) {
    for (auto player : players)
        player->check_then_add(team_of_the_week, week_num);
}

ostringstream Team::GET_players(pair<PlayerPost, Ranks> post_and_ranks) {
    vector<PlayerInfo> output_players = GET_post_players(post_and_ranks.first);

    if (post_and_ranks.second == true)
        sort(output_players.begin(), output_players.end(), &Team::compare_players_by_score);
    else
        sort(output_players.begin(), output_players.end(), &Team::compare_players_by_name);

    return make_list_of_players(output_players);
}

vector<PlayerInfo> Team::GET_post_players(PlayerPost player_post) {
    vector<PlayerInfo> output_players;

    for (auto& player : players)
        player->GET_player(output_players, player_post);

    return output_players;
}

bool Team::compare_players_by_score(const PlayerInfo &left, const PlayerInfo &right) {
    if (left.avg_score != right.avg_score)
        return left.avg_score > right.avg_score;

    return left.name < right.name;
}

bool Team::compare_players_by_name(const PlayerInfo &left, const PlayerInfo &right) {
    return left.name < right.name;
}

// string Team::extract_score_from_player_info(const string &player_info) {
//     string SCORE = "score: ";
//     size_t score_pos = player_info.find(SCORE) + SCORE.length();
//     size_t score_length = player_info.find_last_of(DOT) + NUM_OF_DECIMAL_PLACE - score_pos + 2;
//     string score = player_info.substr(score_pos, score_length);
//     score.pop_back();
//     return score;
// }

ostringstream Team::make_list_of_players(const vector<PlayerInfo>& output_players) {
    ostringstream output;
    output << "list of players:\n";
    for (int i = 0; i < output_players.size(); i++) {
        output << to_string(i + 1) << ". name: " << output_players[i].name
               << " | role: " << output_players[i].role << " | score: "
               << fixed << setprecision(NUM_OF_DECIMAL_PLACE) << output_players[i].avg_score
               << " | cost: " << output_players[i].cost;

        if (output_players[i].goals != NULL_INT)
            output << " | goals: " << output_players[i].goals;

        if (output_players[i].assists != NULL_INT)
            output << " | assists: " << output_players[i].assists;

        if (output_players[i].clean_sheets != NULL_INT)
            output << " | clean sheets: " << output_players[i].clean_sheets;

        output << '\n';
    }
               
    
    return output;
}

TeamInfo Team::get_team_info() {
    return {name, score, num_goals_for, num_goals_against};
}

Player* Team::find_player(string player_name) const {
    for (auto player : players)
        if (player->is_name(player_name))
            return &*player;

    return nullptr;
}