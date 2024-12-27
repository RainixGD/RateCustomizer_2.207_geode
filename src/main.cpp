#include "./includes.h"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelCell.hpp>

#include "./RateCustomizerManager.h"

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		RateCustomizerManager::getInstance()->onMenuLayer(this);
		return true;
	}
};

class $modify(LevelInfoLayer) {
	void setupLevelInfo() {
		LevelInfoLayer::setupLevelInfo();
		RateCustomizerManager::getInstance()->onLevelInfoLayer_setupLevelInfo(this);
	}
};

class $modify(LevelCell) {
	void loadCustomLevelCell() {
		LevelCell::loadCustomLevelCell();
		RateCustomizerManager::getInstance()->onLevelCell_loadCustomLevelCell(this);
	}
};