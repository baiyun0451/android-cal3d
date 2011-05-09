#include "GameStateManager.h"


GameStateManager::GameStateManager()
{

}
void GameStateManager::changeState(GameState * state)
{
	popState();
	pushState(state);
}void GameStateManager::pushState(GameState * state)
{
	state->onEnter();
	mGameState.push(state);
}
void GameStateManager::popState()
{
	if(!mGameState.isEmpty())
	{
		mGameState.top()->onExit();
		mGameState.pop();
	}
}
void GameStateManager::clear()
{
	while(!mGameState.isEmpty())
	{
		popState();
	}
}
GameState * GameStateManager::getCurrentState()
{
	return mGameState.top();
}
GameStateManager::~GameStateManager()
{
	clear();
}
