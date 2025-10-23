#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <limits>

using namespace std;

enum enGameChoice
{
    Stone = 1,
    Paper = 2,
    Scissors = 3
};
enum enWinner
{
    Human = 1,
    Computer = 2,
    Draw = 3
};

struct stRoundInfo
{
    short RoundNumber = 0;
    enGameChoice HumanChoice;
    enGameChoice ComputerChoice;
    enWinner Winner;
    string WinnerName;
};

struct stGameResults
{
    short GameRounds = 0;
    short HumanWinTimes = 0;
    short ComputerWinTimes = 0;
    short DrawTimes = 0;
    enWinner GameWinner;
    string WinnerName = "";
};

int RandomNumber(int From, int To)
{
    int randNum = rand() % (To - From + 1) + From;
    return randNum;
}

string WinnerName(enWinner Winner)
{
    string arrWinnerName[3] = { "Human", "Computer", "No Winner" };
    return arrWinnerName[static_cast<int>(Winner) - 1];
}

enWinner WhoWonTheRound(const stRoundInfo& RoundInfo)
{
    if (RoundInfo.HumanChoice == RoundInfo.ComputerChoice)
    {
        return enWinner::Draw;
    }

    switch (RoundInfo.HumanChoice)
    {
    case enGameChoice::Stone:
        if (RoundInfo.ComputerChoice == enGameChoice::Paper)
            return enWinner::Computer;
        break;
    case enGameChoice::Paper:
        if (RoundInfo.ComputerChoice == enGameChoice::Scissors)
            return enWinner::Computer;
        break;
    case enGameChoice::Scissors:
        if (RoundInfo.ComputerChoice == enGameChoice::Stone)
            return enWinner::Computer;
        break;
    }
    return enWinner::Human;
}

string ChoiceName(enGameChoice Choice)
{
    string arrGameChoices[3] = { "Stone", "Paper", "Scissors" };
    return arrGameChoices[static_cast<int>(Choice) - 1];
}

void SetWinnerScreenColor(enWinner Winner)
{
#ifdef _WIN32
    switch (Winner)
    {
    case enWinner::Human:
        system("color 2F"); // green
        break;
    case enWinner::Computer:
        system("color 4F"); // red
        cout << "\a";
        break;
    default:
        system("color 6F"); // yellow
        break;
    }
#endif
}

void PrintRoundResults(const stRoundInfo& RoundInfo)
{
    cout << "\n____________Round [" << RoundInfo.RoundNumber << "]____________\n\n";
    cout << "Human Choice    : " << ChoiceName(RoundInfo.HumanChoice) << endl;
    cout << "Computer Choice : " << ChoiceName(RoundInfo.ComputerChoice) << endl;
    cout << "Round Winner    : [" << RoundInfo.WinnerName << "]\n";
    cout << "__________________________________\n" << endl;

    SetWinnerScreenColor(RoundInfo.Winner);
}

enWinner WhoWonTheGame(short HumanWinTimes, short ComputerWinTimes)
{
    if (HumanWinTimes > ComputerWinTimes)
        return enWinner::Human;
    else if (ComputerWinTimes > HumanWinTimes)
        return enWinner::Computer;
    else
        return enWinner::Draw;
}

stGameResults FillGameResults(int GameRounds, short HumanWinTimes, short ComputerWinTimes, short DrawTimes)
{
    stGameResults GameResults;

    GameResults.GameRounds = GameRounds;
    GameResults.HumanWinTimes = HumanWinTimes;
    GameResults.ComputerWinTimes = ComputerWinTimes;
    GameResults.DrawTimes = DrawTimes;
    GameResults.GameWinner = WhoWonTheGame(HumanWinTimes, ComputerWinTimes);
    GameResults.WinnerName = WinnerName(GameResults.GameWinner);

    return GameResults;
}

// Robust integer input using getline + stoi (prevents cin getting stuck on bad input)
int ReadIntInRange(int minVal, int maxVal, const string& prompt)
{
    string line;
    int value = minVal;
    while (true)
    {
        cout << prompt;
        if (!std::getline(cin, line))
        {
            // EOF or error; clear and continue
            cin.clear();
            continue;
        }
        // simple parsing
        std::istringstream iss(line);
        if (!(iss >> value))
        {
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        if (value < minVal || value > maxVal)
        {
            cout << "Value must be between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return value;
    }
}

enGameChoice ReadHumanChoice()
{
    // ASCII-style emoticons for portability
    const string prompt = "\nYour Choice: [1] Stone (^_^), [2] Paper (>_<), [3] Scissors (>v<) ? ";
    int choice = ReadIntInRange(1, 3, prompt);
    return static_cast<enGameChoice>(choice);
}

enGameChoice GetComputerChoice()
{
    return static_cast<enGameChoice>(RandomNumber(1, 3));
}

string Tabs(short NumberOfTabs)
{
    string t;
    for (short i = 0; i < NumberOfTabs; ++i)
        t += '\t';
    return t;
}

void ShowGameOverScreen()
{
    cout << Tabs(2) << "__________________________________________________________\n\n";
    cout << Tabs(2) << "\t\t  +++ G a m e  O v e r +++\n";
    cout << Tabs(2) << "__________________________________________________________\n\n";
}

void ShowFinalGameResults(const stGameResults& GameResults)
{
    cout << Tabs(2) << "_____________________ [Game Results] _____________________\n\n";
    cout << Tabs(2) << "Game Rounds        : " << GameResults.GameRounds << endl;
    cout << Tabs(2) << "Human Won Times    : " << GameResults.HumanWinTimes << endl;
    cout << Tabs(2) << "Computer Won Times : " << GameResults.ComputerWinTimes << endl;
    cout << Tabs(2) << "Draw Times         : " << GameResults.DrawTimes << endl;
    cout << Tabs(2) << "Final Winner       : " << GameResults.WinnerName << endl;
    cout << Tabs(2) << "_________________________________________________________\n";

    SetWinnerScreenColor(GameResults.GameWinner);
}

short ReadHowManyRounds()
{
    int v = ReadIntInRange(1, 10, "How Many Rounds 1 to 10 ? ");
    return static_cast<short>(v);
}

// --- PlayGame must be defined before StartGame because StartGame calls it ---
stGameResults PlayGame(short HowManyRounds)
{
    stRoundInfo RoundInfo;
    short HumanWinTimes = 0, ComputerWinTimes = 0, DrawTimes = 0;
    for (short GameRound = 1; GameRound <= HowManyRounds; ++GameRound)
    {
        cout << "\nRound [" << GameRound << "] begins:\n";
        RoundInfo.RoundNumber = GameRound;
        RoundInfo.HumanChoice = ReadHumanChoice();
        RoundInfo.ComputerChoice = GetComputerChoice();
        RoundInfo.Winner = WhoWonTheRound(RoundInfo);
        RoundInfo.WinnerName = WinnerName(RoundInfo.Winner);

        if (RoundInfo.Winner == enWinner::Human)
            ++HumanWinTimes;
        else if (RoundInfo.Winner == enWinner::Computer)
            ++ComputerWinTimes;
        else
            ++DrawTimes;

        PrintRoundResults(RoundInfo);
    }
    return FillGameResults(HowManyRounds, HumanWinTimes, ComputerWinTimes, DrawTimes);
}

void ResetScreen()
{
#ifdef _WIN32
    system("cls");
    system("color 0F");
#else
    cout << "\x1B[2J\x1B[H";
#endif
}

static void StartGame()
{
    char PlayAgain = 'Y';
    do
    {
        ResetScreen();
        stGameResults GameResults = PlayGame(ReadHowManyRounds());
        ShowGameOverScreen();
        ShowFinalGameResults(GameResults);

        cout << endl << Tabs(3) << "Do you want to play again? (Y/N): ";
        string ans;
        getline(cin, ans);
        if (!ans.empty())
            PlayAgain = ans[0];
        else
            PlayAgain = 'N';
    } while (PlayAgain == 'Y' || PlayAgain == 'y');
}

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    StartGame();
    return 0;
}
