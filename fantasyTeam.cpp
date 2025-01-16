#include "fantasyTeam.hpp"

FantasyTeam::FantasyTeam(string name_, string password_) : name(name_), password(password_),
    point(0.0), players(NUM_OF_FANTASY_TEAM_PLAYERS, nullptr), captain(nullptr), num_of_sells(0),
    num_of_buys(0), has_played_any_game(false), has_passed_week(false), budget(INIT_BUDGET) {}

void FantasyTeam::POST_login(string password_) {
    if (password != password_)
        throw PermissionDenied("incorrect password");
}

void FantasyTeam::POST_pass_week() {
    set_week_point();

    if (is_complete() && did_all_players_last_week_played())
        has_played_any_game = true;

    num_of_sells = 0;
    num_of_buys = 0;
    has_passed_week = true;
}

void FantasyTeam::set_week_point() {
    if (is_complete() && did_all_players_last_week_played())
        for (auto player : players)
            if (player != captain)
                point += player->get_last_week_score();
            else
                point += player->get_last_week_score_as_captain();
}

bool FantasyTeam::is_complete() {
    return find(players.begin(), players.end(), nullptr) == players.end();
}

bool FantasyTeam::did_all_players_last_week_played() {
    for (auto player : players)
        if (!player->did_last_week_play())
            return false;

    return true;
}

pair<Name, Point> FantasyTeam::GET_user_ranking() {
    if (has_passed_week)
        return {name, point};
    
    return {NULL_STRING, 0.f};
}

ostringstream FantasyTeam::GET_squad() {
    if (!is_complete())
        return ostringstream(RESPONSE_TO_EMPTY_LIST);

    sort(players.begin() + Defender1, players.begin() + Defender2 + 1, [](Player* left, Player* right) {
        return left->get_name() < right->get_name();
    }); 

    return make_squad();    
}

ostringstream FantasyTeam::make_squad() {
    ostringstream output;
    output << "fantasy_team: " << name << '\n'
           << "Goalkeeper: " << players[goalkeeper]->get_name();

    if (captain == players[goalkeeper])
        output << " (CAPTAIN)";
    
    output << "\nDefender1: " << players[Defender1]->get_name();

    if (captain == players[Defender1])
        output << " (CAPTAIN)";

    output << "\nDefender2: " << players[Defender2]->get_name();

    if (captain == players[Defender2])
        output << " (CAPTAIN)";

    output << "\nMidfielder: " << players[Midfielder]->get_name();

    if (captain == players[Midfielder])
        output << " (CAPTAIN)";

    output << "\nStriker: " << players[Striker]->get_name();

    if (captain == players[Striker])
        output << " (CAPTAIN)";

    output << "\nTotal Points: " << fixed << setprecision(NUM_OF_DECIMAL_PLACE) << point << '\n'
           << "Team Cost: " << get_team_cost() << '\n';

    return output; 
}

int FantasyTeam::get_team_cost() {
    int cost = 0;

    for (auto fantasy_player : players)
        cost += fantasy_player->get_price();

    return cost;
}

ostringstream FantasyTeam::POST_sell_player(istringstream& input) {
    if (num_of_sells == MAX_PLAYER_TRANSFER && has_played_any_game)
        throw PermissionDenied("this week you can't sell more player");

    Tools::check_question_mark(input);

    Player *&sell_player = find_fantasy_player(get_player_name(input));

    if (sell_player == captain)
        captain = nullptr;

    budget += sell_player->get_price();
    sell_player = nullptr;

    num_of_sells++;

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}

string FantasyTeam::get_player_name(istringstream& input) {
    string NAME, player_name = NULL_STRING;

    if (!(input >> NAME) || NAME != "name")
        throw BadRequest("invalid input");

    while(getline(input, player_name))
        ;

    if (player_name == NULL_STRING || player_name == " ")
        throw BadRequest("invalid input");

    player_name = player_name.substr(1);

    return player_name;
}

Player*& FantasyTeam::find_fantasy_player(string player_name) {
    for (auto& player : players)
        if (player != nullptr && player->is_name(player_name))
            return player;

    throw NotFound("this player doesn't exist to sell");
}

ostringstream FantasyTeam::POST_buy_player(istringstream &input, const PremierLeague* premier_league) {
    if (num_of_buys == MAX_PLAYER_TRANSFER && has_played_any_game)
        throw PermissionDenied("this week you can't buy more player");

    Tools::check_question_mark(input);

    return premier_league->find_player(get_player_name(input))->if_can_buy(players, budget);
}

ostringstream FantasyTeam::POST_set_captain(istringstream &input) {
    Tools::check_question_mark(input);

    captain = find_fantasy_player(get_player_name(input));

    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}