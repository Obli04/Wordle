#include <iostream>
#include <time.h>
#include <random>
#include <fstream>
#include <string>
#define len 5757

using namespace std;

void GetWords();
void GetValues();
void SaveValues();
string GenerateWord();
void Check(string guess, string parola);
int multiplier = 20;
string words[len];
int currPoints = 0;
int highScore = 0;
bool isInHighscore = false;

int main()
{
    srand(time(NULL));
    GetWords();
    GetValues();
    bool hasWon = false;
    int ch;
    do
    {
        SaveValues();
        int values = 0;
        string guesses[5];
        hasWon = false;
        system("cls");
        cout << "Current Score : " << currPoints << "\tHighest score: " << highScore << endl;
        cout << "Wanna play WORDLE?\n";
        cout << "1. Yes\n2. No\n> ";
        cin >> ch;
        if(ch == 1)
        {
            string parola = GenerateWord();
            string guess;
            for(int lives = 6; lives > 0 && !hasWon; lives--)
            {
                bool isValid = false;
                while(!isValid)
                {
                    system("cls");
                    for(int i = 0; i < 5; i++) 
                    {
                        Check(guesses[i], parola);
                        cout << " ";
                    }
                    cout << endl << lives << " lives remaining\nGuess the word!\n> ";
                    cin >> guess;
                    if(guess == parola) 
                    {
                        cout << "You're right!\nThe word was " << "\033[0;32m" << parola << "\033[0m\n";
                        hasWon = true;
                        currPoints += lives*multiplier;
                        isInHighscore = true;
                        if(currPoints > highScore) highScore = currPoints;
                        break;
                    }
                    for(int i = 0; i < len && !isValid; i++)
                    {
                        if(guess == words[i])
                        {
                            isValid = true;
                            guesses[values] = guess;
                            values++;
                        }
                    }
                    if(guess.length() != 5) 
                    {
                        cout << "Word length not allowed\n";
                        system("PAUSE");
                    }
                    else if(!isValid) 
                    {
                        cout << "Word does not exist\n";
                        system("PAUSE");
                    }
                }
                if(!hasWon)
                Check(guess, parola);
                system("PAUSE");
            }
            system("cls");
            if(!hasWon)
            {
                currPoints = 0;
                isInHighscore = false;
                cout << "The word was: " << parola << endl;
                system("PAUSE");
            }
        }
    }while(ch != 2);
    return 1;
}

string GenerateWord()
{
    int random = rand() % len;
    return words[random];
}

void SaveValues()
{
    FILE *pf = fopen("Docs/stats.bin", "wb");
    fwrite(&currPoints, sizeof(int), 1, pf);
    fwrite(&highScore, sizeof(int), 1, pf);
    fclose(pf);
}

void GetWords()
{	
    ifstream wordsIF("Docs/words.txt");
    string word;
    int i = 0;
	while( getline (wordsIF, word))
	{
        words[i] = word;
		i++;
	}
}

void GetValues()
{
    FILE *pf = fopen("Docs/stats.bin", "rb");
    fread(&currPoints, sizeof(int), 1, pf);
    fread(&highScore, sizeof(int), 1, pf);
    fclose(pf);
}

void Check(string guess, string parola)
{
    int status[5] = {0};
    for(int i = 0; i < guess.length(); i++)
    {
        bool found = false;
        for(int j = 0; j < parola.length() && !found; j++)
        {
            if(i == j && guess[i] == parola[j]) 
            {
                status[j] = 2;
                found = true;
                cout << "\033[0;32m" << parola[j] << "\033[0m";
            }
            else if(i != j && guess[i] == parola[j] && status[j] != 2 && status[j] != 1 && parola[j] != guess[j]) 
            {
                status[j] = 1;
                found = true;
                cout << "\033[0;33m" << parola[j] << "\033[0m";
            }
        }
        if(!found) cout << guess[i];
    }
}