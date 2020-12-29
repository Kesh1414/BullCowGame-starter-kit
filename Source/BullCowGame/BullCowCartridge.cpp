// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{   
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    SetupGame();

    PrintLine(FString::Printf(TEXT("The number of possible words is %i"), Words.Num()));
    PrintLine(FString::Printf(TEXT("The number of valid words is: %i"), GetValidWords(Words).Num()));
    PrintLine(FString::Printf(TEXT("The HiddenWord is: %s"), *HiddenWord));
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordList) const
{
    TArray<FString> ValidWords;

    for (int32 Index = 0; Index < WordList.Num(); Index++)
    {
        if (WordList[Index].Len() >= 4 && WordList[Index].Len() <= 8 && IsIsogram(WordList[Index]))
        {
            ValidWords.Emplace(WordList[Index]);
        }
    }
    return ValidWords;
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else //Checking PlayerGuess
    {
        ProcessGuess(Input);  
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("cakes");
    Lives = HiddenWord.Len();
    bGameOver = false;

    // Welcoming the Player
    PrintLine(TEXT("Welcome to Bull Cows Game!"));
    PrintLine(FString::Printf(TEXT("Guess the %i letter word!"), HiddenWord.Len()));
    PrintLine(FString::Printf(TEXT("You have %i Lives."), Lives));
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));

    //const TCHAR HW[] = TEXT("cakes");
    //PrintLine(TEXT("Character 1 of the hidden word is: %c"), HW[0]);
}

void UBullCowCartridge::GameWon()
{
    bGameOver = true;
    PrintLine(TEXT("Congratulations you beat the game!"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    ClearScreen();
    PrintLine(FString::Printf(TEXT("Hidden word was %s\n"), *HiddenWord));
    PrintLine(TEXT("Press enter to play again.\nESC to Quit"));
}

bool UBullCowCartridge::IsIsogram(FString Word) const // compares all guess letters and if they repeat calls up message from ProcessGuess function under !IsIsogram if statement
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}


void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Your guess is correct!"));
        GameWon();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Ops wrong number or charracters!\nTry again."));
        PrintLine(FString::Printf(TEXT("The hidden word is %i letters long."), HiddenWord.Len()));
        return;
    }


    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again!"));
        return;
    }

    else
    {
        PrintLine(FString::Printf(TEXT("Lost a life! You have %i lives left!"), --Lives));
        if (Lives > 0)
        {
            if (Guess.Len() == HiddenWord.Len())
            {
                PrintLine(TEXT("Sorry, try again!"));
                return;
            }
        }
        else
        {
            PrintLine(TEXT("You have no lives left!"));
            EndGame();
        }
    }
}
