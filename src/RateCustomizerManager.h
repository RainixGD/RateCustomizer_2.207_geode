#pragma once
#include "./includes.h"

#define public_cast(value, member) [](auto* v) { \
	class FriendClass__; \
	using T = std::remove_pointer<decltype(v)>::type; \
	class FriendeeClass__: public T { \
	protected: \
		friend FriendClass__; \
	}; \
	class FriendClass__ { \
	public: \
		auto& get(FriendeeClass__* v) { return v->member; } \
	} c; \
	return c.get(reinterpret_cast<FriendeeClass__*>(v)); \
}(value)

class RateCustomizerManager {
	enum DataLoadingResult {
		OK,
		FileNotFound,
		ParsingError
	};
	std::unordered_map<unsigned int, std::string> badges;
	DataLoadingResult loadingStatus;
	static RateCustomizerManager* instance;

	void init();

	bool can_convert_to_int(const std::string& str);

	DataLoadingResult loadData();
	void createErrorLabel(CCLayer* layer);

	RateCustomizerManager() {};
public:

	void onMenuLayer(CCLayer* layer);
	void onLevelInfoLayer_setupLevelInfo(LevelInfoLayer* layer);
	void onLevelCell_loadCustomLevelCell(LevelCell* cell);

	static RateCustomizerManager* getInstance();
};