#include "player.hpp"

Player::Player(string name_, int price_)
    : name(name_), price(price_), injured(false, -1), red_card(false), num_yellow_card(0) {}

GoalKeeper::GoalKeeper(string name, int price)
    : Player(name, price), num_matches_without_against_goal(0) {}

Defender::Defender(string name, int price)
    : Player(name, price), num_matches_without_against_goal(0), num_scored_goals(0), num_goals_assist(0) {}

Midfielder::Midfielder(string name, int price)
    : Player(name, price), num_matches_without_against_goal(0), num_scored_goals(0), num_goals_assist(0) {}

Forward::Forward(string name, int price)
    : Player(name, price), num_scored_goals(0), num_goals_assist(0) {}

bool Player::is_bitween(const vector<string> &names) {
    for (string name_ : names)
        if (name == name_)
            return true;

    return false;
}

void Player::POST_pass_week() {
    weeks_score.push_back(0.f);

    last_week_played = true;

    if (injured.first == true) {
        last_week_played = false;
        if (weeks_score.size() - injured.second == NUM_OF_INJURED_WEEKS)
            injured.first = false;
    }

    if (num_yellow_card == NUM_OF_YELLOW_CARD_FOR_SUSPENSION) {
        last_week_played = false;
        num_yellow_card = 0;
    }

    if (red_card == true) {
        last_week_played = false;
        red_card = false;
    }
}

void Player::apply_match_info(PlayerMatchInfo match_info) {
    if (match_info.injured)
        injured = {true, weeks_score.size()};

    if (match_info.yellow_card)
        num_yellow_card++;

    if (match_info.red_card) {
        red_card = true;
        num_yellow_card = 0;
    }

    set_week_score(match_info);
}

void Player::set_week_score(const PlayerMatchInfo &match_info) {
    last_week_point = get_result_status_point(match_info.result);
    last_week_point += calc_goals_point(match_info.num_gaols_and_assists.first, match_info.num_gaols_and_assists.second, match_info.num_against_goals);
    last_week_point += get_goals_against_of_this_post_side_point(match_info.num_against_goals_from_this_post_side);
    last_week_point += get_own_goals_point(match_info.own_goals);

    *(weeks_score.end() - 1) = calc_score_from_point(last_week_point);
}

int Player::get_result_status_point(MatchResultstatus result) {
    if (result == MatchResultstatus::win)
        return TEAM_WIN_POINT;
    if (result == MatchResultstatus::equal)
        return TEAM_EQUAL_POINT;
    return TEAM_LOSE_POINT;
}

int GoalKeeper::calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals) {
    int point = GOALKEEPER_BASE_POINT;

    if (num_against_goals == 0) {
        point += GOALKEEPER_WITHOUT_AGAINST_GOAL_POINT;
        num_matches_without_against_goal++;
    }

    point += num_against_goals * GOALKEEPER_AGAINST_GOAL_POINT;

    return point;
}

int Defender::calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals) {
    int point = DEFENDER_BASE_POINT;

    if (num_against_goals == 0) {
        point += DEFENDER_WITHOUT_AGAINST_GOAL_POINT;
        num_matches_without_against_goal++;
    }

    point += num_goals_scored * DEFENDER_SCORED_GOAL_POINT;
    num_scored_goals += num_goals_scored;

    point += num_goals_assists * DEFENDER_GOAL_ASSIST_POINT;
    num_goals_assist += num_goals_assists;

    return point;
}

int Midfielder::calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals) {
    int point = MIDFIELDER_BASE_POINT;

    if (num_against_goals == 0) {
        point += MIDFIELDER_WITHOUT_AGAINST_GOAL_POINT;
        num_matches_without_against_goal++;
    }

    point += num_goals_scored * MIDFIELDER_SCORED_GOAL_POINT;
    num_scored_goals += num_goals_scored;

    point += num_goals_assists * MIDFIELDER_GOAL_ASSIST_POINT;
    num_goals_assist += num_goals_assists;

    return point;
}

int Forward::calc_goals_point(int num_goals_scored, int num_goals_assists, int num_against_goals) {
    int point = FOARWARD_BASE_POINT;

    point += num_goals_scored * FORWARD_SCORED_GOAL_POINT;
    num_scored_goals += num_goals_scored;

    point += num_goals_assists * FORWARD_GOAL_ASSIST_POINT;
    num_goals_assist += num_goals_assists;

    if (num_goals_scored == 0)
        point += FORWARD_WITHOUT_SCORED_GOAL_POINT;

    return point;
}

int Player::get_goals_against_of_this_post_side_point(int num_against_goals_from_this_post_side) {
    return num_against_goals_from_this_post_side * DEDUCT_GOAL_AGAINST_OF_THIS_POST_SIDE_POINT;
}

int Player::get_own_goals_point(int own_goals) {
    return own_goals * OWN_GOAL_POINT;
}

float Player::calc_score_from_point(int point) {
    return 10 / (1 + exp(-1 * point / 6.f));
}

void GoalKeeper::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::goalkeeper)
        output_players.push_back(GET_player_info());
}

void Defender::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::defender)
        output_players.push_back(GET_player_info()); 
}

void Midfielder::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::midfielder)
        output_players.push_back(GET_player_info());
}

void Forward::GET_player(vector<PlayerInfo>& output_players, PlayerPost player_post) {
    if (player_post == PlayerPost::all || player_post == PlayerPost::forward)
        output_players.push_back(GET_player_info());
}

PlayerInfo Player::GET_player_info() {
    return PlayerInfo{name,
                      get_post(),
                      get_avarage_score(),
                      price,
                      get_num_scored_goals(),
                      get_num_goals_assist(),
                      get_num_matches_without_against_goal()};
}

float Player::get_avarage_score() {
    float total_score = 0.f;
    int num_of_played_week = 0;
    for (auto week_score : weeks_score)
        if (week_score != 0.f) {
            total_score += week_score;
            num_of_played_week++;
        }

    if (total_score == 0.f)
        return total_score;

    return total_score / num_of_played_week;
}

void GoalKeeper::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(team_of_the_week.goal_keeper, week_num);
}

void Defender::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(find_fewer(team_of_the_week.defender1, team_of_the_week.defender2), week_num);

    sort(team_of_the_week.defender1, team_of_the_week.defender2);
}

void Midfielder::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(team_of_the_week.mid_fielder, week_num);
}

void Forward::check_then_add(TeamOfTheWeek &team_of_the_week, int week_num) {
    check_then_replace(team_of_the_week.forward, week_num);
}

void Player::check_then_replace(PlayerScore &player_score, int week_num) {
    if (player_score.second < weeks_score[week_num] ||
        (player_score.second == weeks_score[week_num] && name < player_score.first)) {

        player_score.first = name;
        player_score.second = weeks_score[week_num];
    }
}

PlayerScore& Defender::find_fewer(PlayerScore& first, PlayerScore& second) {
    if (first.second < second.second)
        return first;

    return second;
}

void Defender::sort(PlayerScore& first, PlayerScore& second) {
    if (first.first > second.first) {
        PlayerScore first_copy(first);
        first = second;
        second = first_copy;
    }
}

float Player::get_last_week_score() {
    return *(weeks_score.end() - 1);
}

float Player::get_last_week_score_as_captain() {
    return calc_score_from_point(2 * last_week_point);
}

bool Player::can_next_week_play() {
    if (red_card)
        return false;

    if (yellow_card == NUM_OF_YELLOW_CARD_FOR_SUSPENSION)
        return false;

    if (injured.first)
        return false;

    return true;
}

ostringstream GoalKeeper::if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget) {
    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Goalkeeper], fantasy_team_budget);
}

ostringstream Defender::if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget) {
    try {
        return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Defender1], fantasy_team_budget);
    } catch (...) {}

    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Defender2], fantasy_team_budget);
}

ostringstream Midfielder::if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget) {
    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Midfielder], fantasy_team_budget);
}

ostringstream Forward::if_can_buy(vector<Player*>& fantasy_players, int fantasy_team_budget) {
    return Player::if_can_buy(fantasy_players[FantasyPlayerPost::Striker], fantasy_team_budget);
}

ostringstream Player::if_can_buy(Player* &post_fantasy_player, int fantasy_team_budget) {
    if (price > fantasy_team_budget)
        throw BadRequest("not enough budget to by");

    if (post_fantasy_player != nullptr)
        throw BadRequest("can't buy because this post is full");
    
    if (!can_next_week_play())
        return ostringstream(ANSWER_CAN_NOT_BUY_PLAYER_BECAUSE_CAN_NOT_PLAY_NEXT_WEEK);

    post_fantasy_player = this;
    fantasy_team_budget -= price;
    return ostringstream(RESPONSE_TO_THE_SUCCESS_OF_THE_REQUEST);
}