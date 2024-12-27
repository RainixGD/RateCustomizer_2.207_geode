#include "./RateCustomizerManager.h"

void RateCustomizerManager::init() {
	loadingStatus = loadData();
}

bool RateCustomizerManager::can_convert_to_int(const std::string& str) {
	try {
		std::stoi(str);
		return true;
	}
	catch (const std::invalid_argument& e) {
		return false;
	}
	catch (const std::out_of_range& e) {
		return false;
	}
}

RateCustomizerManager::DataLoadingResult RateCustomizerManager::loadData() {
	std::ifstream file("Resources/RateCustomizer.json");
	if (!file) return FileNotFound;
	std::ostringstream buffer;
	buffer << file.rdbuf();
	std::string fileContent = buffer.str();

	file.close();
	try {
		auto root = nlohmann::json::parse(fileContent);

		if (!root.is_object()) return ParsingError;

		for (const auto& [key, value] : root.items()) {
			if (!can_convert_to_int(key)) return ParsingError;

			int badgeId = std::stoi(key);
			if (badgeId < 1 || !value.is_string()) return ParsingError;

			badges[badgeId] = value.get<std::string>();
		}
	}
	catch (const std::exception& e) {
		return ParsingError;
	}
	return OK;
}

void RateCustomizerManager::createErrorLabel(CCLayer* layer) {
	std::string errorText;
	switch (loadingStatus) {
	case RateCustomizerManager::FileNotFound:
		errorText = "Can't find 'RateCustomizer.json' in ./Resources";
		break;
	case RateCustomizerManager::ParsingError:
		errorText = "Can't parse 'RateCustomizer.json'";
		break;
	}

	auto size = CCDirector::sharedDirector()->getWinSize();

	auto errorLabel = CCLabelBMFont::create(errorText.c_str(), "bigFont.fnt");
	errorLabel->setColor({ 255, 0, 0 });
	errorLabel->setScale(0.6);
	errorLabel->setPosition({ size.width / 2, size.height - 10 });
	layer->addChild(errorLabel);
}

void RateCustomizerManager::onMenuLayer(CCLayer* layer) {
	if (loadingStatus != OK) {
		createErrorLabel(layer);
		return;
	}
}

void RateCustomizerManager::onLevelInfoLayer_setupLevelInfo(LevelInfoLayer* layer) {
	if (loadingStatus != OK) return;

	int rateValue = layer->m_level->m_featured;
	
	auto difficultySprite = layer->m_difficultySprite;
	if (difficultySprite->getChildrenCount() == 0) return;

	if (rateValue > 0) {
		if (badges.find(rateValue) == badges.end()) return;
		auto newRateIcon = CCSprite::create(badges[rateValue].c_str());
		newRateIcon->setZOrder(-1);
		if (newRateIcon == nullptr) {
			newRateIcon = CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png");
			newRateIcon->setZOrder(5);
		}
		auto position = static_cast<CCNode*>(difficultySprite->getChildren()->objectAtIndex(0))->getPosition();
		newRateIcon->setPosition({position.x, position.y + 8});

		difficultySprite->removeAllChildren();

		difficultySprite->addChild(newRateIcon);
	}
}

void RateCustomizerManager::onLevelCell_loadCustomLevelCell(LevelCell* cell) {
	if (loadingStatus != OK) return;
	
	int rateValue = cell->m_level->m_featured;

	auto difficultyContainer = cell->m_mainLayer->getChildByID("difficulty-container");
	if (!difficultyContainer) return;

	auto difficultySprite = difficultyContainer->getChildByID("difficulty-sprite");
	if (!difficultySprite) return;

	if (difficultySprite->getChildrenCount() == 0) return;

	if (rateValue > 0) {
		if (badges.find(rateValue) == badges.end()) return;
		auto newRateIcon = CCSprite::create(badges[rateValue].c_str());
		newRateIcon->setZOrder(-1);
		if (newRateIcon == nullptr) {
			newRateIcon = CCSprite::createWithSpriteFrameName("edit_delBtnSmall_001.png");
			newRateIcon->setZOrder(5);
		}
		auto position = static_cast<CCNode*>(difficultySprite->getChildren()->objectAtIndex(0))->getPosition();
		newRateIcon->setPosition({ position.x, position.y + 8 });

		difficultySprite->removeAllChildren();

		difficultySprite->addChild(newRateIcon);
	}
}

RateCustomizerManager* RateCustomizerManager::getInstance() {
	AllocConsole();
	freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
	if (!instance) {
		instance = new RateCustomizerManager();
		instance->init();
	}
	return instance;
}
RateCustomizerManager* RateCustomizerManager::instance = nullptr;