#include <string>
#include "SFML/Graphics.hpp"
#include "cat.h"
#include "sprite.h"
#include "button.h"
#include "renderer.h"
#include "audiomanager.h"

Cat::Cat()
{
	// Hacky hack so when we first fade in
	// the scene dont pops (hearts and foods)
	Reset();

	// Initialize sounds and bg music
	auto audioMan = AudioManager::GetInstance();
	audioMan->AddTrack(mTrack1);
	audioMan->AddTrack(mTrack2);
	audioMan->SetVolume(mIsMuted == true ? 0.0f : 10.0f);
	audioMan->PlayMusic();

	// Set initial state
	mState = kMainMenu;

	// Set initial stats
	mStats.Health = 1.0f;
	mStats.Age = 0.0f;
	mStats.Weight = 0.0f;
	mStats.Hunger = 0.0f;
	mStats.Misses = 0.0f;
	mStats.Happiness = 0.5f;

	// Main font
	if (!mMainFont.loadFromFile(mFontPath))
	{
		printf("ERROR:Could not load the main font.\n");
	}

	// Window size
	auto r = Renderer::GetInstance();
	auto wSize = r->GetWindow()->getSize();

	// Hearts
	for (int i = 0; i < mHearts; i++)
	{
		mHeartsSprites.push_back(Sprite(mHeartPath));
		mHeartsSprites[mHeartsSprites.size() - 1].mSfSprite.setPosition
		(
			i * 64.0f,
			5.0f
		);
	}
	for (int i = 0; i < mHearts; i++)
	{
		mEmptHeartsSprites.push_back(Sprite(mEmptHeartPath));
		mEmptHeartsSprites[mEmptHeartsSprites.size() - 1].mSfSprite.setPosition
		(
			i * 64.0f,
			5.0f
		);
	}
	// Foods
	for (int i = 0; i < mFoods; i++)
	{
		mFoodSprites.push_back(Sprite(mFodPath));
		mFoodSprites[mFoodSprites.size() - 1].mSfSprite.setPosition
		(
			i * 64.0f + 256.0f,
			5.0f
		);
	}
	for (int i = 0; i < mFoods; i++)
	{
		mEmpFoodSprites.push_back(Sprite(mEmpFoodPath));
		mEmpFoodSprites[mEmpFoodSprites.size() - 1].mSfSprite.setPosition
		(
			i * 64.0f + 256.0f,
			5.0f
		);
	}

	// Init buttons
	// start menu
	mStartBtn = std::shared_ptr<Button>(new Button(mStartBtnPath, mStartBtnPath));
	mStartBtn->SetPosition(192.0f, 360.0f);
	// menu
	mGoToStatsBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mGoToStatsBtn->SetPosition(384.0f, 656.0f);
	mBackToIdleStatsBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mBackToIdleStatsBtn->SetPosition(384.0f, 656.0f);
	// actions
	mFeedBtn = std::shared_ptr<Button>(new Button(mStdButton,mStdButtonPressed));
	mFeedBtn->SetPosition(0.0f, 656.0f);
	mSnackBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mSnackBtn->SetPosition(0.0f, 592.0f);
	mMealBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mMealBtn->SetPosition(0.0f, 528.0f);
	mPetBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mPetBtn->SetPosition(128.0f, 656.0f);
	mCleanBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mCleanBtn->SetPosition(256.0f, 656.0f);

	// Dead btn
	mDeadBtn = std::shared_ptr<Button>(new Button(mStdButton, mStdButtonPressed));
	mDeadBtn->SetPosition(192.0f, 460.0f);

	// Init cat
	mCat = std::shared_ptr<Sprite>(new Sprite(mCatPath));

	// Poop
	mPoopSprite = std::shared_ptr<Sprite>(new Sprite(mPoopPath));
	mPoopSprite.get()->mSfSprite.setPosition(256.0f, 400.0f);

	// Main background
	mBackground = std::shared_ptr<Sprite>(new Sprite(mBgPath));

	// Title
	mTitle = std::shared_ptr<Sprite>(new Sprite(mTitlePath));
	mTitle->mSfSprite.setPosition(0.0f, 100.0f);

	// Audio
	mAudio = std::shared_ptr<Sprite>(new Sprite(mAudioPath));
	mAudio->mSfSprite.setPosition(448.0f, 0.0f);
	mAudioMute = std::shared_ptr<Sprite>(new Sprite(mAudioMutePath));
	mAudioMute->mSfSprite.setPosition(448.0f, 0.0f);

	// Fader
	mFader = std::shared_ptr<Sprite>(new Sprite(mFadeTPath));
	mFader->mSfSprite.setColor(sf::Color(255, 255, 255, 0));

	// Background sprites
	mSunrise = std::shared_ptr<Sprite>(new Sprite(mSunrisePath));
	mMidday = std::shared_ptr<Sprite>(new Sprite(mMiddayPath));
	mSunset = std::shared_ptr<Sprite>(new Sprite(mSunsetPath));
	mNight = std::shared_ptr<Sprite>(new Sprite(mNightPath));
}

Cat::~Cat()
{
}

void Cat::Update(float dt)
{
	AudioManager::GetInstance()->Update(dt);

	switch (mState)
	{
	case kMainMenu:
		UpdateMainMenu(dt);
		break;
	case kIdle:
		UpdateIdle(dt);
		break;
	case kPlaying:
		UpdatePlaying(dt);
		break;
	case kSleeping:
		UpdateSleeping(dt);
		break;
	case kStats:
		UpdateStats(dt);
		break;
	case kFade:
		UpdateFade(dt);
	case kDead:
		UpdateDead(dt);
		break;
	default:
		break;
	}
}

void Cat::Render(sf::RenderWindow* renderWindow)
{
	switch (mState)
	{
	case kMainMenu:
		RenderMainMenu(renderWindow);
		break;
	case kIdle:
		RenderIdle(renderWindow);
		break;
	case kPlaying:
		RenderPlaying(renderWindow);
		break;
	case kSleeping:
		RenderSleeping(renderWindow);
		break;
	case kStats:
		RenderStats(renderWindow);
		break;
	case kFade:
		RenderFade(renderWindow);
		break;
	case kDead:
		RenderDead(renderWindow);
		break;
	default:
		break;
	}
}

void Cat::UpdateMainMenu(float dt)
{
	auto r = Renderer::GetInstance();

	// Mute
	mAudioCurCd += dt;
	sf::Vector2i mousePos = sf::Mouse::getPosition(*r->GetWindow());
	if (mAudio->mSfSprite.getGlobalBounds().contains(mousePos.x,mousePos.y)&&
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (mAudioCurCd >= mAudioCd)
		{
			mAudioCurCd = 0.0f;
			mIsMuted = !mIsMuted;
			if (mIsMuted)AudioManager::GetInstance()->SetVolume(0.0f);
			else AudioManager::GetInstance()->SetVolume(10.0f);
		}
	}
	
	// Start btn
	if (mStartBtn->IsPressed())
	{
		mHasStarted = true;
		mState = kFade;
		mFadeFrom = kMainMenu;
		mFadeTo = kIdle;

		return;
	}
}

void Cat::UpdateIdle(float dt)
{
	// Acumulated play time (age)
	mCurTime += dt;
	mTimeDay += dt * mTimeMod;
	if (mTimeDay > 1.0f)
	{
		mTimeDay = 0.0f;
		mTotalDays++;
	}
	// Select day time
	if (mTimeDay >= 0.0f && mTimeDay < 0.25f)
	{
		mCurDayTime = kSunRise;
	}
	else if (mTimeDay >= 0.25f && mTimeDay < 0.50f)
	{
		mCurDayTime = kMidday;
	}
	else if (mTimeDay >= 0.5f && mTimeDay < 0.75f)
	{
		mCurDayTime = kSunset;
	}
	else
	{
		mCurDayTime = kNight;
	}

	// Check debug
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
	{
		mShowDebug = !mShowDebug;
	}

	// Feed btn
	mFedCurTimer += dt;
	if (mFeedBtn->IsPressed())
	{
		if (mFedCurTimer >= mFeedBtnCd)
		{
			mShowFeedBtns = !mShowFeedBtns;
			mFedCurTimer = 0.0f;
		}
	}
	// Snack btn
	mSnackCurTimer += dt;
	if (mSnackBtn->IsPressed())
	{
		if (mSnackCurTimer >= mSnackCd)
		{
			mSnackCurTimer = 0.0f;
			// Reduce hunger
			mStats.Hunger -= mSnackHunger;
			mStats.Hunger = std::fmax(0.0f, mStats.Hunger);
			// Increase weight
			mStats.Weight += mSnackWeight;
		}
	}
	// Meal btn
	mMealCurTimer += dt;
	if (mMealBtn->IsPressed())
	{
		if (mMealCurTimer >= mMealCd)
		{
			mMealCurTimer = 0.0f;
			// Reduce hunger
			mStats.Hunger -= mMealHunger;
			mStats.Hunger = std::fmax(0.0f, mStats.Hunger);
			// Increase weight
			mStats.Weight += mMealWeight;
		}
	}
	// Pet btn
	// Clean btn
	if (mCleanBtn->IsPressed())
	{
		if (mHasPoop)
		{
			mHasPoop = false;
			mCurPoopTimer = 0.0f;
		}
	}
	// Stats btn
	if (mGoToStatsBtn->IsPressed())
	{
		mState = kFade;
		mFadeFrom = kIdle;
		mFadeTo = kStats;
		return;
	}

	mStats.Age += dt;
	if (mStats.Hunger < 1.0f)
	{
		mStats.Hunger += mHungerRatio * dt;
	}

	// Calc cur food
	mCurFoods = 1.0f + (mFoods * (1.0f - mStats.Hunger));
	if (mCurFoods > mFoods)mCurFoods = mFoods;

	// Update poop status
	mCurPoopTimer += dt;
	if (mCurPoopTimer >= mPoopCd)
	{
		mHasPoop = true;
	}
	// Get sick if poop is there for a long time
	if (mCurPoopTimer > (mPoopStartSick + mPoopCd) && mHasPoop)
	{
		mStats.Health -= mPoopRatio * dt;
		mStats.Health = std::max(0.0f, mStats.Health);
	}

	// Update health and hearts
	mCurHears = 1.0f + (mHearts * (mStats.Health));
	if (mCurHears > mHearts)mCurHears = mHearts;

	// Check dead
	if (mStats.Health == 0.0f)
	{
		Reset();
		mState = kFade;
		mFadeFrom = kIdle;
		mFadeTo = kDead;
		return;
	}
}

void Cat::UpdatePlaying(float dt)
{
}

void Cat::UpdateSleeping(float dt)
{
}

void Cat::UpdateStats(float dt)
{
	auto r = Renderer::GetInstance();

	// Mute
	mAudioCurCd += dt;
	sf::Vector2i mousePos = sf::Mouse::getPosition(*r->GetWindow());
	if (mAudio->mSfSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) &&
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		if (mAudioCurCd >= mAudioCd)
		{
			mAudioCurCd = 0.0f;
			mIsMuted = !mIsMuted;
			if (mIsMuted)AudioManager::GetInstance()->SetVolume(0.0f);
			else AudioManager::GetInstance()->SetVolume(10.0f);
		}
	}
	// Back to idle
	if (mBackToIdleStatsBtn->IsPressed())
	{
		mState = kFade;
		mFadeFrom = kStats;
		mFadeTo = kIdle;
		return;
	}
}

void Cat::UpdateFade(float dt)
{
	// Is fadding out
	if (!mHasFadedOut)
	{
		mFadeCurState = mFadeFrom;

		mFadeCurTime += dt;
		int curAlpha = (int)((255.0f * mFadeCurTime) / mFadeTime);
		mFader->mSfSprite.setColor(sf::Color(255, 255, 255, curAlpha));
		if (mFadeCurTime >= mFadeTime)
		{
			mHasFadedOut = true;
			mFader->mSfSprite.setColor(sf::Color(255, 255, 255, 255));
			mFadeCurTime = mFadeTime;
		}
	}
	// Is fadding in
	else
	{
		mFadeCurState = mFadeTo;

		mFadeCurTime -= dt;
		float curAlpha = (255.0f * mFadeCurTime) / mFadeTime;
		mFader->mSfSprite.setColor(sf::Color(255, 255, 255, (float)curAlpha));
		if (mFadeCurTime <= 0.0f)
		{
			mHasFadedOut = false;
			mFader->mSfSprite.setColor(sf::Color(255, 255, 255, 0));
			mState = mFadeTo;
			return;
		}
	}
}

void Cat::UpdateDead(float dt)
{
	if (mDeadBtn->IsPressed())
	{
		mState = kFade;
		mFadeFrom = kDead;
		mFadeTo = kMainMenu;
		return;
	}
}

void Cat::RenderMainMenu(sf::RenderWindow * renderWindow)
{
	auto r = Renderer::GetInstance();
	r->Render(mStartBtn.get());
	r->Render(mTitle.get());
	if (mIsMuted)
	{
		r->Render(mAudioMute.get());
	}
	else
	{
		r->Render(mAudio.get());
	}
}

void Cat::RenderIdle(sf::RenderWindow * renderWindow)
{
	auto r = Renderer::GetInstance();

	switch (mCurDayTime)
	{
	case kSunRise:
		r->Render(mSunrise.get());
		break;
	case kMidday:
		r->Render(mMidday.get());
		break;
	case kSunset:
		r->Render(mSunset.get());
		break;
	case kNight:
		r->Render(mNight.get());
		break;
	default:
		break;
	}
	//r->Render(mBackground.get());
	r->Render(mFeedBtn.get());
	if (mShowFeedBtns)
	{
		r->Render(mSnackBtn.get());
		r->Render(mMealBtn.get());
	}
	r->Render(mPetBtn.get());
	r->Render(mCleanBtn.get());
	r->Render(mGoToStatsBtn.get());

	// Bg hearts
	for (int i = 0; i<mHearts; i++)
	{
		renderWindow->draw(mEmptHeartsSprites[i].mSfSprite);
	}
	// Current hearts
	for (int i = 0; i<mCurHears; i++)
	{
		renderWindow->draw(mHeartsSprites[i].mSfSprite);
	}

	// Bg foods
	for (int i = 0; i<mFoods; i++)
	{
		renderWindow->draw(mEmpFoodSprites[i].mSfSprite);
	}
	// Current foods
	for (int i = 0; i<mCurFoods; i++)
	{
		renderWindow->draw(mFoodSprites[i].mSfSprite);
	}

	// Poop!
	if (mHasPoop)
	{
		r->Render(mPoopSprite.get());
	}

	// Debug
	if (mShowDebug)
	{
		RenderStats(renderWindow);
	}
}

void Cat::RenderPlaying(sf::RenderWindow * renderWindow)
{
	auto r = Renderer::GetInstance();
}

void Cat::RenderSleeping(sf::RenderWindow * renderWindow)
{
	auto r = Renderer::GetInstance();
}

void Cat::RenderStats(sf::RenderWindow * renderWindow)
{
	auto r = Renderer::GetInstance();
	
	const float kTopOff = 50.0f;

	//Title
	mStatsTitle.setFont(mMainFont);
	mStatsTitle.setPosition(5.0f, -32.0f + kTopOff);
	mStatsTitle.setColor(sf::Color::Black);
	mStatsTitle.setString("Stats");
	renderWindow->draw(mStatsTitle);

	// Age
	mStatsAge.setFont(mMainFont);
	mStatsAge.setPosition(15.0f, 0.0f + kTopOff);
	mStatsAge.setColor(sf::Color::Black);
	std::string age = "Age:" + std::to_string(mStats.Age);
	mStatsAge.setString(age);
	renderWindow->draw(mStatsAge);

	// Weight
	mStatsWeight.setFont(mMainFont);
	mStatsWeight.setPosition(15.0f, 32.0f + kTopOff);
	mStatsWeight.setColor(sf::Color::Black);
	std::string wei = "Weight:" + std::to_string(mStats.Weight);
	mStatsWeight.setString(wei);
	renderWindow->draw(mStatsWeight);

	// Hunger
	mHungerText.setFont(mMainFont);
	mHungerText.setPosition(15.0f, 64.0f + kTopOff);
	mHungerText.setColor(sf::Color::Black);
	int hungerPer = (int)(mStats.Hunger * 100.0f);
	std::string hun = "Hunger:" + std::to_string(hungerPer) + "%";
	mHungerText.setString(hun);
	renderWindow->draw(mHungerText);

	// Happiness
	mHappinessText.setFont(mMainFont);
	mHappinessText.setPosition(15.0f, 96.0f + kTopOff);
	mHappinessText.setColor(sf::Color::Black);
	int hapPer = (int)(mStats.Happiness * 100.0f);
	std::string hap = "Happiness:" + std::to_string(hapPer) + "%";
	mHappinessText.setString(hap);
	renderWindow->draw(mHappinessText);

	// Health
	mHealthText.setFont(mMainFont);
	mHealthText.setPosition(15.0f, 129.0f + kTopOff);
	mHealthText.setColor(sf::Color::Black);
	int heaPer = (int)(mStats.Health * 100.0f);
	std::string hea = "Health:" + std::to_string(heaPer) + "%";
	mHealthText.setString(hea);
	renderWindow->draw(mHealthText);

	// Total days
	mTotalDaysText.setFont(mMainFont);
	mTotalDaysText.setPosition(15.0f, 161.0f + kTopOff);
	mTotalDaysText.setColor(sf::Color::Black);
	std::string day = "Days:" + std::to_string(mTotalDays);
	mTotalDaysText.setString(day);
	renderWindow->draw(mTotalDaysText);

	r->Render(mBackToIdleStatsBtn.get());

	if (mIsMuted)
	{
		r->Render(mAudioMute.get());
	}
	else
	{
		r->Render(mAudio.get());
	}
}

void Cat::RenderFade(sf::RenderWindow * renderWindow)
{
	switch (mFadeCurState)
	{
	case kMainMenu:
		RenderMainMenu(renderWindow);
		break;
	case kIdle:
		RenderIdle(renderWindow);
		break;
	case kPlaying:
		RenderPlaying(renderWindow);
		break;
	case kSleeping:
		RenderSleeping(renderWindow);
		break;
	case kStats:
		RenderStats(renderWindow);
		break;
	case kDead:
		RenderDead(renderWindow);
		break;
	default:
		break;
	}

	renderWindow->draw(mFader->mSfSprite);
}

void Cat::RenderDead(sf::RenderWindow * renderWindow)
{
	auto r = Renderer::GetInstance();
	r->Render(mDeadBtn.get());
}

void Cat::Reset()
{
	// Food and heats
	mCurHears = mHearts;
	mCurFoods = mFoods;

	// Set initial stats
	mStats.Health = 1.0f;
	mStats.Age = 0.0f;
	mStats.Weight = 0.0f;
	mStats.Hunger = 0.0f;
	mStats.Misses = 0.0f;
	mStats.Happiness = 0.5f;

	mCurPoopTimer = 0.0f;

	mShowFeedBtns = false;

	mCurDayTime = kSunRise;
	mCurTime = 0.0f;
	mTimeDay = 0.0f;
	mTotalDays = 0;
}
