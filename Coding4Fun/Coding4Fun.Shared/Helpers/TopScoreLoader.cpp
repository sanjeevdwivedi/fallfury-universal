#include "pch.h"
#include "TopScoreLoader.h"

using namespace Coding4Fun::FallFury::Helpers;

TopScoreLoader::TopScoreLoader(void)
{
}

TopScoreLoader::TopScoreLoader(Coding4Fun::FallFury::Models::TopScoreSet^ set)
{
	m_currentEntrySet = set;
}

Windows::Foundation::Collections::IVector<TopScore^>^ TopScoreLoader::Extract()
{
	Windows::Foundation::Collections::IVector<TopScore^>^ returnCollection 
		= ref new Platform::Collections::Vector<TopScore^>();

	int size = m_currentEntrySet->Scores->Length;
	for (int i = 0; i< size; i++)
	{
		FallFury::Models::TopScoreEntry^ score = m_currentEntrySet->Scores[i];	
		
		TopScore^ topScore = ref new TopScore();
		topScore->Name = score->Name;
		topScore->Score = score->Score;
		topScore->Level = score->Level;
		topScore->ScoreMedalType = score->Medal;

		returnCollection->Append(topScore);
	}

	return returnCollection;
}