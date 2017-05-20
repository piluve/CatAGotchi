/*
	cat.h nachocpol@gmail.com
*/

#include <vector>
#include <memory>

struct CatStats
{
	float Hunger;
	float Happiness;
	float Weight;
	float Misses;
	float Age;	
};

enum CatStates
{
	kIdle,
	kPlaying,
	kSleeping,
	kStats
};

class RenderWindow;
class Sprite;
class Button;
class Cat
{
public:
	Cat();
	~Cat();
	void Update(float dt);
	void Render(sf::RenderWindow* renderWindow);

private:
	void UpdateIdle(float dt);
	void UpdatePlaying(float dt);
	void UpdateSleeping(float dt);
	void UpdateStats(float dt);

	void RenderIdle(sf::RenderWindow* renderWindow);
	void RenderPlaying(sf::RenderWindow* renderWindow);
	void RenderSleeping(sf::RenderWindow* renderWindow);
	void RenderStats(sf::RenderWindow* renderWindow);

	CatStats mStats;
	CatStates mState;

	// Font
	const char* mFontPath = "../data/fonts/arial.ttf";
	sf::Font mMainFont;

	// Ratios and caps
	float mMaxAge = 1800.0f;	// 30minutes
	float mHungerRatio = 0.01f;	// +0.01 each second
	float mSnackWeight = 10.0f;	// + 10grames
	float mSnackHunger = 1.0f;	// -1.0f hunger
	float mMealWeight = 100.0f;	// + 100grames
	float mMealHunger = 2.0f;	// - 2.0f hunger
	
	// Hearts
	const char* mHeartPath = "../data/textures/heart.png";
	const char* mEmptHeartPath = "../data/textures/heartempt.png";
	int mHearts = 4;
	int mCurHears = 4;
	std::vector<Sprite> mHeartsSprites;
	std::vector<Sprite> mEmptHeartsSprites;

	// Food
	const char* mFodPath = "../data/textures/bread.png";
	const char* mEmpFoodPath = "../data/textures/breadempt.png";
	int mFoods = 4;
	int mCurFoods = 0;
	std::vector<Sprite> mFoodSprites;
	std::vector<Sprite> mEmpFoodSprites;

	// Buttons
	// Feed
	const char* mStdButton = "../data/textures/btntest.png";
	const char* mStdButtonPressed = "../data/textures/btntestpres.png";
	std::shared_ptr<Button> mFeedBtn;
	float mFeedBtnCd = 1.5f;
	float mFedCurTimer = 1.5f;
	// Stats
	// Go to stats
	std::shared_ptr<Button> mGoToStatsBtn;
	// Back to idle
	std::shared_ptr<Button> mBackToIdleStatsBtn;

	// Stats text
	sf::Text mStatsAge;
	sf::Text mStatsWeight;
	sf::Text mHungerText;
	sf::Text mHappinessText;

	// Cat
	const char* mCatPath = "../data/textures/cat.png";
	std::shared_ptr<Sprite> mCat;
};
