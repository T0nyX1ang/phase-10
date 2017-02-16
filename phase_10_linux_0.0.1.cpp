#include<iostream>
#include<bits/stdc++.h>

using namespace std;

bool isout[109] = {};

int shown = 0;
int not_shown = 0;

string player1_enter;
string player2_enter;

struct player_data
{
    int player1[13];
    int player2[13];
    bool player1_isphased[13];
    bool player2_isphased[13];
    int player1_score;
    int player2_score;
    int player1_phase;
    int player2_phase;
    string player1_word;
    string player2_word;
};

struct phase_data
{
    bool isset;
    int set_num;
    bool isrun;
    int run_start;
    int run_stop;
    bool issamecolor;
};

void initialize();

void transfer(int card_number);

int card_out();

int check_set(int *arr, int total);

bool cmp(int a, int b);

bool isclear(int *arr, int start, int stop);

int get_number(string enter);

int search_card_number(int *arr, int start, int stop, int card_number);

int search_card_order(int *arr, int start, int stop, int card_number);

void show_phase_1();

int search_phase(int num);

int calculate_score(int *arr, int total);

void round_p1();
void round_p2();

void show_card_p1();
void show_card_p2();

void pick_card_p1(int choice);
void pick_card_p2(int choice);

int discard_card_p1(bool discard_module);
int discard_card_p2(bool discard_module);

int phase_1_p1();
int phase_1_p2();

int hit_phase_p1();
int hit_phase_p2();

player_data player;
phase_data phase1[5];

int main()
{
    initialize();

    cout<<"The game is ready to start.\n";
    system("sleep 2");
    system("clear");

    srand(time(0));
    int start_from = 1;//rand() % 2;
    int start_card = card_out();
    int random_card = card_out();
    int discard_card = 0;

    // initial cards
    shown = start_card;
    not_shown = random_card;

    bool player1_isskipped = false;
    bool player2_isskipped = false;
    while ((!isclear(player.player1, 1, 11)) && (!isclear(player.player2, 1, 11)))
    {
        if (start_from % 2 != 0)
        {
            if ((shown <= 104) || (player1_isskipped))
            {
                round_p1();
                player1_isskipped = false;
            }
            else
            {
                cout<<"You are skipped this round.\n";
                player1_isskipped = true;
            }
            //start_from++;
            cout<<"Get ready for your opponent's round.\n";
            system("sleep 2");
            system("clear");
            // return 0;
        }
        else
        {/*
            if ((shown <= 104) || (player2_isskipped))
            {
                round_p2();
                player2_isskipped = false;
            }
            else
            {
                cout<<"You are skipped this round.\n";
                player2_isskipped = true;
            }
            start_from++;
            cout<<"Get ready for your opponent's round.\n";
            system("sleep 5");
            system("clear");*/
        }
    }

    // calculate scores
    if (isclear(player.player1, 1, 11))
        player.player2_score += calculate_score(player.player2, 11);
    else
        player.player1_score += calculate_score(player.player2, 11);

    // show scores
    system("clear");
    cout<<"Hand complete!\nThe scores now:\n";
    cout<<"Player1 "<<player.player1_score<<endl;
    cout<<"Player2 "<<player.player2_score<<endl;
    system("sleep 1");
    cout<<"Get ready for next hand.\n";

    return 0;
}



bool isclear(int *arr, int start, int stop)
{
    for (int i = start; i <= stop; i++)
        if (*(arr + i) != 0)
            return false;
    return true;
}

void initialize()
{
    system("clear");

    // initialize player
    for (int i = 1; i <= 10; i++)
        player.player1[i] = 0;
    for (int i = 1; i <= 10; i++)
        player.player2[i] = 0;

    // initialize card_numbers
    srand(time(0));
    int counter = 1;
    while (counter <= 20)
    {
        int card_number = rand() % 108 + 1;
        if (!isout[card_number])
        {
            if (counter % 2 != 0)
                player.player1[(counter + 1) / 2] = card_number;
            else
                player.player2[counter / 2] = card_number;
            counter++;
            isout[card_number] = true;
        }
    }

    // initialize phase status
    for (int i = 1; i <= 11; i++)
    {
        player.player1_isphased[i] = false;
        player.player2_isphased[i] = false;
    }

    // initialize player phase status
    player.player1_phase = 0;
    player.player2_phase = 0;

    // enter password
    cout<<"Player 1, please enter your password: ";
    getline(cin,player.player1_word);
    system("clear");
    cout<<"Player 2, please enter your password: ";
    getline(cin,player.player2_word);
    system("clear");

    return;
}

void transfer(int card_number)
{
    if (card_number <= 96)
        switch ((card_number - 1) / 24)
        {
            case 0:
                cout<<"Red "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
            case 1:
                cout<<"Yellow "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
            case 2:
                cout<<"Blue "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
            case 3:
                cout<<"Green "<<(card_number - 1) / 2 % 12 + 1<<"\n";
                break;
        }
    else if (card_number >= 105)
        cout<<"Skip Card\n";
    else if (card_number <= 0)
        return;
    else
        cout<<"Wild Card\n";
    return;
}

bool cmp(int a, int b)
{
    if ((a <= 96) && (b <= 96) && ((a - 1) / 2 % 12 + 1 != (b - 1) / 2 % 12 + 1))
        return ((a - 1) / 2 % 12 + 1 < (b - 1) / 2 % 12 + 1);
    else
        return (a < b);
}

int card_out()
{
    srand(time(0));
    int start_card = rand() % 108 + 1;
    if (isout[start_card])
        while (isout[start_card])
            start_card = rand() % 108 + 1;
    isout[start_card] = true;
    return start_card;
}

int get_number(string enter)
{
    string middle;
    int output = 0;
    for (int i = 0; i < enter.size(); i++)
        if (enter[i] != ' ')
            middle += tolower(enter[i]);

    // for red
    string red_check = "";
    red_check = red_check + middle[0] + (middle[1]) + middle[2];
    if ((middle[0] == 'r') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 0 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 0 + 2 * int(middle[1] - '0');
    }
    else if ((red_check == "red") && (middle[3] >= '0') && (middle[3] <= '9'))
    {
        if ((middle[4] >= '0') && (middle[4] <= '2') && (middle[3] == '1'))
            output = 0 + 2 * (int(middle[3] - '0') * 10 + int(middle[4] - '0'));
        else
            output = 0 + 2 * int(middle[3] - '0');
    }

    // for yellow
    string yellow_check = "";
    yellow_check = yellow_check + middle[0] + middle[1] + middle[2] + middle[3] + middle[4] + middle[5];
    if ((middle[0] == 'y') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 24 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 24 + 2 * int(middle[1] - '0');
    }
    else if ((yellow_check == "yellow") && (middle[6] >= '0') && (middle[6] <= '9'))
    {
        if ((middle[7] >= '0') && (middle[7] <= '2') && (middle[6] == '1'))
            output = 24 + 2 * (int(middle[6] - '0') * 10 + int(middle[7] - '0'));
        else
            output = 24 + 2 * int(middle[6] - '0');
    }

    // for blue
    string blue_check = "";
    blue_check = blue_check + middle[0] + middle[1] + middle[2] + middle[3];
    if ((middle[0] == 'b') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 48 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 48 + 2 * int(middle[1] - '0');
    }
    else if ((blue_check == "blue") && (middle[4] >= '0') && (middle[4] <= '9'))
    {
        if ((middle[5] >= '0') && (middle[5] <= '2') && (middle[4] == '1'))
            output = 48 + 2 * (int(middle[4] - '0') * 10 + int(middle[5] - '0'));
        else
            output = 48 + 2 * int(middle[4] - '0');
    }

    // for green
    string green_check = "";
    green_check = green_check + middle[0] + middle[1] + middle[2] + middle[3] + middle[4];
    if ((middle[0] == 'g') && (middle[1] >= '0') && (middle[1] <= '9'))
    {
        if ((middle[2] >= '0') && (middle[2] <= '2') && (middle[1] == '1'))
            output = 72 + 2 * (int(middle[1] - '0') * 10 + int(middle[2] - '0'));
        else
            output = 72 + 2 * int(middle[1] - '0');
    }
    else if ((green_check == "green") && (middle[5] >= '0') && (middle[5] <= '9'))
    {
        if ((middle[6] >= '0') && (middle[6] <= '2') && (middle[5] == '1'))
            output = 72 + 2 * (int(middle[5] - '0') * 10 + int(middle[6] - '0'));
        else
            output = 72 + 2 * int(middle[5] - '0');
    }

    // for wild cards
    string wild_check_a = "";
    string wild_check_b = "";
    wild_check_a = wild_check_a + middle[0] + middle[1] + middle[2] + middle[3];
    wild_check_b = wild_check_b + middle[0] + middle[1] + middle[2] + middle[3] + middle[4] + middle[5] + middle[6] + middle[7];
    if ((middle[0] == 'w') && ((wild_check_a == "wild") || (wild_check_b == "wildcard")) || (middle == "w") || (middle == "wc"))
        output = 97;

    // for skip cards
    string skip_check_a = "";
    string skip_check_b = "";
    skip_check_a = skip_check_a + middle[0] + middle[1] + middle[2] + middle[3];
    skip_check_b = skip_check_b + middle[0] + middle[1] + middle[2] + middle[3] + middle[4] + middle[5] + middle[6] + middle[7];
    if ((middle[0] == 's') && ((skip_check_a == "skip") || (skip_check_b == "skipcard")) || (middle == "s") || (middle == "sc"))
        output = 105;

    if (output != 0)
        return output;

    if ((enter != "") && (output == 0))
    {
        cout<<"Wrong syntax! Please check and reenter: ";
        return -1;
    }

    if (enter == "")
        return -1;
}

int search_card_number(int *arr, int start, int stop, int card_number)
{
    for (int i = 1; i <= card_number; i++)
        for (int j = start; j <= stop; j++)
            if ((*(arr + i) == j) && (!player.player1_isphased[i]))
                return j;
    return 0;
}

int search_card_order(int *arr, int start, int stop, int card_number)
{
    for (int i = 1; i <= card_number; i++)
        for (int j = start; j <= stop; j++)
            if ((*(arr + i) == j) && (!player.player1_isphased[i]))
                return i;
    return 0;
}

int check_set(int *arr, int total)
{
    // deal with raw data
    for (int i = 1; i <= total; i++)
        if ((*(arr + i) >= 97) && (*(arr + i) <= 104))
            *(arr + i) = 13;
        else
            *(arr + i) = (*(arr + i) - 2) % 24 / 2 + 1;
    // check if it's all wild cards & find where the first normal card is...
    int counter = 1;
    for (counter = 1; counter <= total; counter++)
        if (*(arr + counter) != 13)
            break;
        else if (counter == total)
        {
            cout<<"You must at least drop a NORMAL CARD!\n";
            return 0;
        }
    // check if it's a set
    for (int i = 1; i <= total; i++)
        if ((*(arr + i) != *(arr + counter)) && (*(arr + i) != 13))
            return 0;
    return *(arr + counter);
}

int search_phase(int num)
{
    // can extend here, but finish first...
    int counter = 0;
    for (int i = 1; i <= 4; i++)
    {
        if (phase1[i].isset)
        {
            if ((num == phase1[i].set_num) || (num == 13))
                counter++;
        }
        cout<<counter<<endl;
    }
    return counter;
}

int calculate_score(int *arr, int total)
{
    int score = 0;
    for (int i = 1; i <= 11; i++)
    {
        if ((*(arr + i) <= 96) && (*(arr + i) > 0))
        {
            if (*(arr + i - 1) % 24 / 2 + 1 <= 9)
                score += 5;
            else
                score += 10;
        }
        else if (*(arr + i) >= 105)
            score += 15;
        else if ((*(arr + i) >= 97) && (*(arr + i) <= 104))
            score += 25;
    }
    return score;
}

void show_phase_1()
{
    for (int i = 1; i <= 4; i++)
        if (phase1[i].set_num != 0)
            cout<<"A set of "<<phase1[i].set_num<<endl;
    return;
}

void round_p1()
{
    cout<<"Player1, enter your password: ";
    getline(cin, player1_enter);

    while (player1_enter != player.player1_word)
    {
        cout<<"Wrong password! Please reenter!\n";
        getline(cin, player1_enter);
    }

    show_card_p1();
    cout<<endl;

    cout<<"The shown card is: ";
    transfer(shown);
    cout<<"And there is a random card which can't be shown.\n\n";

    if (shown <= 104)
    {
        cout<<"Pick one, enter 1 for the shown card, 2 for the random one.\n";
        int confirmer = 0;
        while ((!(cin>>confirmer)) || ((confirmer != 1) && (confirmer != 2)))
        {
            cin.clear();
            while (cin.get() != '\n')
                continue;
            cout<<"Wrong syntax! Please reenter: ";
        }
        pick_card_p1(confirmer);
        if (confirmer == 2)
            not_shown = card_out();
    }
    else
    {
        cout<<"You can't pick Skip Card.\nAutomatically take the random card instead.\n";
        pick_card_p1(2);
        not_shown = card_out();
    }

    // phase
    if (player.player1_phase != 1)
    {
        system("sleep 1");
        system("clear");
        show_card_p1();
        cout<<"Do you have a phase? Enter \"yes(y)\" to confirm.\n";
        string confirmer_phase = "";
        while (confirmer_phase == "")
            getline(cin, confirmer_phase);
        if ((confirmer_phase == "y") || (confirmer_phase == "yes"))
        {
            player.player1_phase = phase_1_p1();
            while (player.player1_phase == -1)
                player.player1_phase = phase_1_p1();
        }
    }

    // hit
    if (player.player1_phase == 1)
    {
        system("sleep 1");
        system("clear");
        show_card_p1();
        cout<<"You have phased! Do you have cards to hit the phases?\nEnter \"yes(y)\" to confirm.\n";
        cout<<"By the way, the phases on the desk are:\n";
        show_phase_1();
        cout<<endl;
        string confirmer_hit = "";
        while (confirmer_hit == "")
            getline(cin, confirmer_hit);
        if ((confirmer_hit == "y") || (confirmer_hit == "yes"))
        {
            cout<<"Enter the card you want to hit: ";
            int copyhit = hit_phase_p1();
            while (copyhit == -1)
                copyhit = hit_phase_p1();
        }
    }

    // discard
    system("sleep 1");
    system("clear");
    show_card_p1();
    cout<<"Enter a card you want to discard: ";
    int copyshown = discard_card_p1(1);
    while (copyshown == -1)
        copyshown = discard_card_p1(1);
    shown = copyshown;
    system("sleep 2");

    return;
}

void show_card_p1()
{
    system("clear");
    cout<<"You own:\n";
    sort(player.player1 + 1, player.player1 + 11 + 1, cmp);
    for (int i = 1; i <= 11; i++)
        if (player.player1[i] != 0)
        {
            transfer(player.player1[i]);
            cout<<"Card No."<<player.player1[i]<<"\n\n";
        }
    return;
}

void pick_card_p1(int choice)
{
    if (choice == 1)
    {
        player.player1[1] = shown;
        show_card_p1();
    }
    else if (choice == 2)
    {
        cout<<"You get a ";
        transfer(not_shown);
        system("sleep 2");
        player.player1[1] = not_shown;
        show_card_p1();
    }
    return;
}

int discard_card_p1(bool discard_module)
{
    string p1_enter = "";
    int card_number = 0;
    int card_order = 0;

    if ((get_number(p1_enter) == -1) && (p1_enter != ""))
        return -1;

    while (p1_enter == "")
        getline(cin, p1_enter);
    int card_range = 0;
    card_range = get_number(p1_enter);

    if ((card_range > 0) && (card_range <= 96))
    {
        card_number = search_card_number(player.player1, card_range - 1, card_range, 11);
        card_order = search_card_order(player.player1, card_range - 1, card_range, 11);
        if (card_number != 0)
        {
            if (discard_module)
            {
                cout<<"You discard a ";
                transfer(card_number);
                player.player1[card_order] = 0;
                system("sleep 2.0");
                show_card_p1();
            }
            return card_number;
        }
        else
        {
            cout<<"You don't have a ";
            transfer(card_range);
            cout<<"Please reenter: ";
            return -1;
        }
    }
    else if ((card_range >= 97) && (card_range <= 104))
    {
        card_number = search_card_number(player.player1, card_range, card_range + 7, 11);
        card_order = search_card_order(player.player1, card_range, card_range + 7, 11);
        if (card_number != 0)
        {
            if (discard_module)
            {
                cout<<"You discard a ";
                transfer(card_number);
                player.player1[card_order] = 0;
                system("sleep 2.0");
                show_card_p1();
            }
            return card_number;
        }
        else
        {
            cout<<"You don't have a ";
            transfer(card_range);
            cout<<"Please reenter: ";
            return -1;
        }
    }
    else if ((card_range >= 105) && (card_range <= 108))
    {
        card_number = search_card_number(player.player1, card_range, card_range + 3, 11);
        card_order = search_card_order(player.player1, card_range, card_range + 3, 11);
        if (card_number != 0)
        {
            if (discard_module)
            {
                cout<<"You discard a ";
                transfer(card_number);
                player.player1[card_order] = 0;
                system("sleep 2.0");
                show_card_p1();
            }
            return card_number;
        }
        else
        {
            cout<<"You don't have a ";
            transfer(card_range);
            cout<<"Please reenter: ";
            return -1;
        }
    }
    else
        return -1;
}

int phase_1_p1()
{
    int pool[4] = {};

    for (int i = 1; i <= 11; i++)
        player.player1_isphased[i] = false;

    for (int i = 1; i <=4; i++)
    {
        phase1[i].isset = true;
        phase1[i].isrun = false;
    }

    cout<<"\nPhase 1: 2 sets of 3.\nEnter the first set of 3.\n";
    // part 1
    for (int i = 1; i <= 3; i++)
    {
        cout<<"Card "<<i<<": ";
        pool[i] = discard_card_p1(0);
        if ((pool[i] >= 105) || (player.player1_isphased[search_card_order(player.player1, pool[i], pool[i], 11)]) || (pool[i] == -1))
            return -1;
        player.player1_isphased[search_card_order(player.player1, pool[i], pool[i], 11)] = true;
        if ((pool[i] <= 96) && (pool[i] % 2 != 0))
            pool[i]++;
    }

    cout<<"Checking the validity of the cards...\nPlease wait...";
    int copyset_num = check_set(pool, 3);
    if (copyset_num > 0)
    {
        cout<<"Correct! Enter the next set of 3.\n";
        phase1[1].set_num = copyset_num;
    }
    else
    {
        cout<<"Wrong phase! Please reenter the whole phase!\n";
        for (int i = 1; i <= 3; i++)
            pool[i] = 0;
        for (int i = 1; i <= 11; i++)
            player.player1_isphased[i] = false;
        return -1;
    }

    // part 2
    for (int i = 1; i <= 3; i++)
    {
        cout<<"Card "<<i<<": ";
        pool[i] = discard_card_p1(0);
        if ((pool[i] >= 105) || (player.player1_isphased[search_card_order(player.player1, pool[i], pool[i], 11)]) || (pool[i] == -1))
            return -1;
        player.player1_isphased[search_card_order(player.player1, pool[i], pool[i], 11)] = true;
        if ((pool[i] <= 96) && (pool[i] % 2 != 0))
            pool[i]++;
    }

    cout<<"Checking the validity of the cards...\nPlease wait...";
    copyset_num = check_set(pool, 3);
    if (copyset_num > 0)
    {
        cout<<"Congratulations! You phase the card!\n";
        phase1[2].set_num =copyset_num;
    }
    else
    {
        cout<<"Wrong phase! Please reenter the whole phase!\n";
        for (int i = 1; i <= 3; i++)
            pool[i] = 0;
        for (int i = 1; i <= 11; i++)
            player.player1_isphased[i] = false;
        return -1;
    }

    // clear the cards
    for (int i = 1; i <= 11; i++)
    {
        if (player.player1_isphased[i])
            player.player1[i] = 0;
        player.player1_isphased[i] = false; // reset phase status
    }

    system("sleep 2");
    show_phase_1();
    cout<<endl;

    // reshow the cards
    show_card_p1();

    return 1;
}

int hit_phase_p1()
{

    int hit_seq = 0;
    int hit = discard_card_p1(0);
    if (hit == -1)
        return -1;
    if (hit <= 96)
        hit_seq = ((hit - 1) % 24 / 2 + 1);
    else if (hit >= 105)
        hit_seq = 14;
    else
        hit_seq = 13;
    int counter = search_phase(hit_seq);
    if (counter == 0)
    {
        cout<<"Phase to hit not found! Please reenter:\n";
        return -1;
    }
    else
    {
        player.player1[search_card_order(player.player1, hit, hit, 11)] = 0;
        show_card_p1();
        cout<<"Do you have other cards to hit? Enter \"yes(y)\" to confirm.\n";
        string confirmer_hit = "";
        while (confirmer_hit == "")
            getline(cin, confirmer_hit);
        if ((confirmer_hit == "y") || (confirmer_hit == "yes"))
            return -1;
        else return 0;
    }
}
