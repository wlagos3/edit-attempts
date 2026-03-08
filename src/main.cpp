#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/Geode.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class ModifyAttemptCountPopup : public geode::Popup {
    geode::SeedValueRSV* attempts;
    TextInput* inputNode;
    protected:
        bool boundsCheck(int num){
            if ((*attempts > 0 && num > std::numeric_limits<int>::max() - *attempts) ||
                (*attempts < 0 && num < std::numeric_limits<int>::min() - *attempts)) {
                log::warn("Operation would result in overflow or underflow");
                return false;
            }
            return true;
        }

        void addAttempts(int num){
            if(boundsCheck(num)){
                *attempts = *attempts + num;
            }
        }
        void subtractAttempts(int num){
            if(boundsCheck(num)){
                *attempts = *attempts - num;
            }
        }
        void setAttempts(int num){
            *attempts = num;
        }
        void onSetButtonClick(CCObject* sender){
            if(inputNode->getString().empty()){return;}
            int val = numFromString<int>(inputNode->getString()).unwrapOr(-1);
            if (val > -1) {
                setAttempts(val);
                this->onClose(nullptr);
            } else {
                geode::Notification::create("Input is not a valid number", NotificationIcon::Error, 2.f)->show();
            }
        }
        void onPlusButtonClick(CCObject* sender){
            if(inputNode->getString().empty()){return;}
            int val = numFromString<int>(inputNode->getString()).unwrapOr(-1);
            if (val > -1) {
                addAttempts(val);
                this->onClose(nullptr);
            } else {
                geode::Notification::create("Input is not a valid number", NotificationIcon::Error, 2.f)->show();
            }
        }
        void onMinusButtonClick(CCObject* sender){
            if(inputNode->getString().empty()){return;}
            int val = numFromString<int>(inputNode->getString()).unwrapOr(-1);
            if (val > -1) {
                subtractAttempts(val);
                this->onClose(nullptr);
            } else {
                geode::Notification::create("Input is not a valid number", NotificationIcon::Error, 2.f)->show();
            }
        }
        bool init(GJGameLevel* level) {
            if (!Popup::init(240.f, 160.f)) return false;
            attempts = &level->m_attempts;

            this->setTitle("Modify Attempt Count");

            auto menu = CCMenu::create();

            auto input = TextInput::create(160.f, "Attempts");
            input->setFilter("0123456789");
            inputNode = input;

            auto setSprite = ButtonSprite::create("Set");
            setSprite->setScale(0.80);
            auto setButton = CCMenuItemSpriteExtra::create(
                setSprite, this, menu_selector(ModifyAttemptCountPopup::onSetButtonClick)
            );

            auto addSprite = ButtonSprite::create("+");
            addSprite->setScale(0.80);
            auto addButton = CCMenuItemSpriteExtra::create(
                addSprite, this, menu_selector(ModifyAttemptCountPopup::onPlusButtonClick)
            );

            auto removeSprite = ButtonSprite::create("-");
            removeSprite->setScale(0.80);
            auto removeButton = CCMenuItemSpriteExtra::create(
                removeSprite, this, menu_selector(ModifyAttemptCountPopup::onMinusButtonClick)
            );
            
            
            menu->addChildAtPosition(setButton, Anchor::Center, CCPoint(0, -50));
            menu->addChildAtPosition(addButton, Anchor::Center, CCPoint(40, -50));
            menu->addChildAtPosition(removeButton, Anchor::Center, CCPoint(-39, -50));
            menu->updateLayout();
            this->m_mainLayer->addChildAtPosition(input, Anchor::Center, CCPoint(0.f, 15.f));
            this->m_mainLayer->addChildAtPosition(menu, Anchor::Center, CCPoint(0.f, 15.f));

            return true;
        }
        void onClose(CCObject* sender) override {
            inputNode->getInputNode()->detachWithIME();
            Popup::onClose(sender);
        }

    public:
        static ModifyAttemptCountPopup* create(GJGameLevel* level) {
            auto ret = new ModifyAttemptCountPopup();
            if (ret->init(level)) {
                ret->autorelease();
                return ret;
            }

            delete ret;
            return nullptr;
        }
};

class $modify (AttemptSetterEditLevelLayer, EditLevelLayer){
	void onAttemptSetButtonClick(CCObject* sender){
		auto modifyAttemptCountPopup = ModifyAttemptCountPopup::create(m_level);
		
		modifyAttemptCountPopup->show();
	}
	bool init(GJGameLevel* level){
		if(!EditLevelLayer::init(level)) return false;

		auto spr = CCSprite::create("editAttemptsButton.png"_spr);

		auto folderMenu = getChildByID("folder-menu");
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(AttemptSetterEditLevelLayer::onAttemptSetButtonClick)
		);

		folderMenu->addChild(btn);
		folderMenu->updateLayout();
		
		return true;
	}
};

class $modify (AttemptSetterLevelInfoLayer, LevelInfoLayer){
	void onAttemptSetButtonClick(CCObject* sender){
		auto modifyAttemptCountPopup = ModifyAttemptCountPopup::create(m_level);
		
		modifyAttemptCountPopup->show();
	}
	bool init(GJGameLevel* level, bool challenge){
		if(!LevelInfoLayer::init(level, challenge)) return false;

		auto spr = CCSprite::create("editAttemptsButton.png"_spr);


		auto leftSideMenu = getChildByID("left-side-menu");
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(AttemptSetterLevelInfoLayer::onAttemptSetButtonClick)
		);

		leftSideMenu->addChild(btn);
		leftSideMenu->updateLayout();
		
		return true;
	}
};




