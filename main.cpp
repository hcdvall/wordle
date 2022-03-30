#include <iostream>
#include <fstream>
#include <string>
#include <random>

#define FOREGROUND(color, text) "\x1B[" << static_cast<int>(color) << ";1m" << " " << text << " " << "\033[0m"
#define BACKGROUND(color, text) "\x1B[" << static_cast<int>(color) << ";1m" << text << " " << "\033[0m"
 
enum class ForegroundColor : int {
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    BrightRed = 91,
    BrightGreen = 92,
    BrightYellow = 93
};

enum class BackgroundColor : int {
    Grey = 37,
    Red = 41,
    Green = 42,
    Yellow = 43,
    BrightRed = 101,
    BrightGreen = 102,
    BrightYellow = 103
};

std::string* readFromFile(int number_of_lines)
{
    std::string* word_arr = new std::string[number_of_lines];
    std::string line;
    std::ifstream myfile("words.txt");
    int count = 0;

    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            word_arr[count] = line;
            count++;
        }
        myfile.close();
    }
    return word_arr;
}

int randomInteger(int number_of_lines)
{
    std::random_device random_device;
    std::mt19937 gen(random_device()); 
    std::uniform_int_distribution<> distr(0, number_of_lines-1); 
    return distr(gen);
}

char* stringToArray(std::string a_word)
{
    char* letter_arr = new char[4];
    for (int i = 0; i < a_word.length(); i++)
    {
        letter_arr[i] = (char)toupper(a_word[i]);
    }
    return letter_arr;
}

void testInputValidity(std::string& player_input, std::string* word_arr, int number_of_lines)
{
    bool valid_word = false;
    while (!valid_word)
    {   
        for (int i = 0; i < number_of_lines; i++)
        {
            if (word_arr[i] == player_input)
            {
                valid_word = true;
                break;
            }
        }
        if (!valid_word)
        {
            if (player_input.length() != 5)
            {
                std::cout << "The word have to the 5 letters long.\nPlease try again:\n";
            }
            else
            {
                std::cout << "The word does not exist or was not uppercase.\nPlease Try again:\n";
                // feedback: a bit frustrating that it forces me to caps lock! Would be nice to have an auto-upper case feature!
            }
            std::cin >> player_input;
        }
    }
}

void compareArrays(std::string* word_arr, char* game_word, std::string game_word_str, bool& win, bool& in_word, int number_of_lines)
{
    for (int i = 0; i < 6; i++)
    {
        // Take player input and make it an array of chars
        std::cout << "\nGuess a 5 letter word: " << std::endl;
        std::string player_input;
        std::cin >> player_input;
        if (player_input == game_word_str)
        { 
            win = true; 
            break; 
        }
        /* Implement test for checking validity of input string */
        testInputValidity(player_input, word_arr, number_of_lines);

        auto player_word = stringToArray(player_input);
        
        //Function to take two arrays and compare them
        for (int j = 0; j < 5; j++)
        {
            if (game_word[j] == player_word[j])
            {
                //print green bg on playerWord letter
                std::cout << BACKGROUND(BackgroundColor::BrightGreen, FOREGROUND(ForegroundColor::Black, player_word[j]));
                continue;
            }
            for (int k = 0; k < 5; k++)
            {
                if (player_word[j] == game_word[k])
                {
                    //Set bg color to yellow
                    std::cout << BACKGROUND(BackgroundColor::BrightYellow, FOREGROUND(ForegroundColor::Black, player_word[j]));
                    in_word = true;
                    break;
                }
            }
            if (!in_word)
            {
                //Set bg color to grey
                std::cout << BACKGROUND(BackgroundColor::Grey, FOREGROUND(ForegroundColor::Black, player_word[j]));
            }
            in_word = false;  
        }
    }
}

int main()
{
    int number_of_lines = 8598;
    // Read lines from file to an array
    auto word_arr = readFromFile(number_of_lines);
    bool wanna_play = true;

    /* OUTER GAME LOOP*/
    while (wanna_play)
    {
        // Get random line from word_arr as array of chars
        int random_int = randomInteger(number_of_lines);
        std::string game_word_str = word_arr[random_int];
        auto game_word = stringToArray(game_word_str);
        bool win = false;
        bool in_word = false;
        
        /* INNER GAME LOOP */
        compareArrays(word_arr, game_word, game_word_str, win, in_word, number_of_lines); // feedback: not sure I totally understand the need to have both
                                                                                        // initial array and target word as arguments here. Seems like we only
                                                                                        // need target word and references to set the result of the compare.

        if (win == true)
        {
            std::cout << "\nCongratulations, you guessed the correct word!" << std::endl;
        }
        else
        {
            std::cout << "\nToo bad, you guessed wrong. The word was: " << game_word_str << std::endl;
        }

        std::cout << "Play again? [y/n]\n";
        char yesno;
        std::cin >> yesno;
        if (yesno == 'y')
        {
            wanna_play = true;
        }
        else
        {
            wanna_play = false;
        }
    }
}

