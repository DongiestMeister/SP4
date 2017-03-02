#include "PartySelectScene.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "RenderHelper.h"
#include "MyMath.h"


#include <iostream>
using namespace std;


PartySelectScene::PartySelectScene()
{
}


PartySelectScene::~PartySelectScene()
{
}

void PartySelectScene::Init()
{
	Math::InitRNG();

	// Create and attach the camera to the scene
	camera.Init(Vector3(100, -10, 100), Vector3(100, 0, 100), Vector3(0, 0, 1));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Load all the meshes for this scene
	MeshBuilder::GetInstance()->GenerateQuad("PartySelectBG", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("PartySelectBG")->textureID = LoadTGA("Image//PartySelect//PartySelectBackground.tga");
	MeshBuilder::GetInstance()->GenerateQuad("test", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("test")->textureID = LoadTGA("Image//test.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selected", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("selected")->textureID = LoadTGA("Image//selectarrow.tga");
	MeshBuilder::GetInstance()->GenerateQuad("statusScreen", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("statusScreen")->textureID = LoadTGA("Image//PartySelect//statusScreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("rightFacingArrow", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("rightFacingArrow")->textureID = LoadTGA("Image//PartySelect//rightFacingArrow.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selectOption", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("selectOption")->textureID = LoadTGA("Image//PartySelect//selectOption.tga");
	MeshBuilder::GetInstance()->GenerateQuad("changeWhoseEQ", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("changeWhoseEQ")->textureID = LoadTGA("Image//PartySelect//changeWhoseEQ.tga");
	MeshBuilder::GetInstance()->GenerateQuad("weaponStats", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("weaponStats")->textureID = LoadTGA("Image//PartySelect//weaponStats.tga");
	MeshBuilder::GetInstance()->GenerateQuad("armorStats", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("armorStats")->textureID = LoadTGA("Image//PartySelect//armorStats.tga");
	MeshBuilder::GetInstance()->GenerateQuad("swordPortrait", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("swordPortrait")->textureID = LoadTGA("Image//PartySelect//swordPortrait.tga");
	MeshBuilder::GetInstance()->GenerateQuad("armorPortrait", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("armorPortrait")->textureID = LoadTGA("Image//PartySelect//armorPortrait.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selectEquip", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("selectEquip")->textureID = LoadTGA("Image//PartySelect//selectEquip.tga");
	MeshBuilder::GetInstance()->GenerateQuad("shop", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("shop")->textureID = LoadTGA("Image//PartySelect//shop.tga");
	MeshBuilder::GetInstance()->GenerateQuad("shopWeaponStats", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("shopWeaponStats")->textureID = LoadTGA("Image//PartySelect//shopWeaponStats.tga");
	MeshBuilder::GetInstance()->GenerateQuad("shopArmorStats", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("shopArmorStats")->textureID = LoadTGA("Image//PartySelect//shopArmorStats.tga");

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	
	selectedPos.Set(-47, 52.5f, 0);
	currentScreen = CURR_SCREEN_SELECT_OPTION;
	i_selectedOptionCounter = 0;

	i_selectedUnitsCounter = 0;
	b_statusCursor = 0;
	statusCursorPos.Set(-10, -40, 0);

	i_selectedEquipmentCounter = 0;
	b_equipmentCursor = 0;
	equipmentCursorPos.Set(30, -40, 0);
	b_showArrowAtEQs = false;

	i_shopCursor = 0;
	i_selectedShopCounter = 0;
	i_eqToShow = 0; // (0 to 15) or (16 - 31) , this number should only be 0 or 16

	b_showShopItems = false;
	b_showPlayerItems = false;
	b_showItemStatsAtShop = false;
	b_equipmentPageNumber = 0;

	message = "";
}

void PartySelectScene::Update(double dt)
{
	// Update our entities
	EntityManager::GetInstance()->Update(dt);
	if (KeyboardController::GetInstance()->IsKeyReleased('C'))
	{
		if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (PlayerInfo::GetInstance()->inventory.size() > 16)
			{
				if (b_equipmentPageNumber == 0)
				{
					b_equipmentPageNumber = 1;
					i_eqToShow = 16;
				}
				else
				{
					b_equipmentPageNumber = 0;
					i_eqToShow = 0;
				}
			}
		}
		else if (currentScreen == CURR_SCREEN_SHOP)
		{
			if( (b_showShopItems == true && b_showPlayerItems == false) || (b_showShopItems == false && b_showPlayerItems == true) )
			{
				b_showItemStatsAtShop = true;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('Z'))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			if (i_selectedOptionCounter == 0) // Change party
			{
				currentScreen = CURR_SCREEN_SELECT_UNITS;
				selectedPos.Set(-60, -10, 0);
				i_selectedUnitsCounter = 0;
			}
			else if (i_selectedOptionCounter == 1) // Change equipment
			{
				currentScreen = CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER;
				selectedPos.Set(-60, 40, 0);
				i_selectedUnitsCounter = 0;
			}
			else if (i_selectedOptionCounter == 2) // Shop
			{
				currentScreen = CURR_SCREEN_SHOP;
				selectedPos.Set(-63.5f, 62, 0);
				i_selectedShopCounter = 0;
				i_shopCursor = 0;
			}
			else if (i_selectedOptionCounter == 3)
			{
				if (PlayerInfo::GetInstance()->party.size() > 0)
					SceneManager::GetInstance()->SetActiveScene("GameState");
				else
					message = "Your party is empty!";
			}
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			if (i_selectedUnitsCounter <= PlayerInfo::GetInstance()->availableUnits.size() - 1)
				currentScreen = CURR_SCREEN_VIEW_STATUS;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			if (b_statusCursor == 0)
			{
				if (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->b_inParty == false)
				{
					for (int i = 0; i < 4; i++)
					{
						if (PlayerInfo::GetInstance()->addCharacterToParty(Vector2(0, 0), PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter), i))
							break;
					}
				}
				else
					PlayerInfo::GetInstance()->removeCharacterFromParty(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->i_idInParty);
			}
			else
			{
				currentScreen = CURR_SCREEN_SELECT_UNITS;
				selectedPos.Set(-60, -10, 0);
				i_selectedUnitsCounter = 0;
			}
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			if (i_selectedUnitsCounter <= PlayerInfo::GetInstance()->availableUnits.size() - 1)
			{
				currentScreen = CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ;
				selectedPos.Set(-80, 41, 0);
				i_selectedEquipmentCounter = 0;
			}
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (b_showArrowAtEQs == true)
			{
				if (b_equipmentCursor == 0)
				{
					if (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->b_isArmor)
					{
						PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->equipArmor(dynamic_cast<Armor*>(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)));
					}
					else if (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter+ i_eqToShow)->b_isWeapon)
					{
						PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->equipWeapon(dynamic_cast<Weapon*>(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)));
					}
				}
				else
				{
					b_showArrowAtEQs = false;
				}
			}
			else
			{
				if (i_selectedEquipmentCounter + i_eqToShow < PlayerInfo::GetInstance()->inventory.size())
				{
					b_showArrowAtEQs = true;
					equipmentCursorPos.Set(30, -40, 10);
					b_equipmentCursor = 0;
				}
			}
		}
		else if (currentScreen == CURR_SCREEN_SHOP)
		{
			if (b_showShopItems == false && b_showPlayerItems == false)
			{
				if (i_selectedShopCounter == 0)
					b_showShopItems = true;
				else if (i_selectedShopCounter == 1)
					b_showPlayerItems = true;
				else if (i_selectedShopCounter == 2)
				{
					// return to pre-battle preparations
					currentScreen = CURR_SCREEN_SELECT_OPTION;
					selectedPos.Set(-47, 52.5f, 0);
					i_selectedOptionCounter = 0;
				}
				message = "";
			}
			else if (b_showShopItems == true && b_showPlayerItems == false) // buying items
			{
				if (b_showItemStatsAtShop == false)
				{
					if (PlayerInfo::GetInstance()->shop.size() > 0)
					{
						// only can buy if the player has enough gold
						if (PlayerInfo::GetInstance()->gold >= PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_Price)
						{
							PlayerInfo::GetInstance()->gold -= PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_Price;
							PlayerInfo::GetInstance()->addItemToInventory(PlayerInfo::GetInstance()->shop.at(i_shopCursor));
							PlayerInfo::GetInstance()->shop.erase(PlayerInfo::GetInstance()->shop.begin() + i_shopCursor);
							if (i_shopCursor > PlayerInfo::GetInstance()->shop.size() - 1)
								i_shopCursor = PlayerInfo::GetInstance()->shop.size() - 1;
						}
					}
				}
			}
			else if (b_showShopItems == false && b_showPlayerItems == true) // selling items
			{
				if (b_showItemStatsAtShop == false)
				{
					if (PlayerInfo::GetInstance()->inventory.size() > 0)
					{
						if (PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->b_isEquippedToSomeone == false)
						{
							// resell back to shop at half price
							PlayerInfo::GetInstance()->gold += (PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_Price >> 1);
							PlayerInfo::GetInstance()->inventory.erase(PlayerInfo::GetInstance()->inventory.begin() + i_shopCursor);
							if (i_shopCursor > PlayerInfo::GetInstance()->inventory.size() - 1)
								i_shopCursor = PlayerInfo::GetInstance()->shop.size() - 1;
						}
						else
						{
							message = "Unequip the weapon before selling!";
						}
					}
				}
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased('X'))
	{
		message = "";
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			// go back to level select
			SceneManager::GetInstance()->SetActiveScene("WarMap");
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			// return to pre-battle preparations
			currentScreen = CURR_SCREEN_SELECT_OPTION;
			selectedPos.Set(-47, 52.5f, 0);
			i_selectedOptionCounter = 0;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			// return to party select
			currentScreen = CURR_SCREEN_SELECT_UNITS;
			selectedPos.Set(-60, -10, 0);
			i_selectedUnitsCounter = 0;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			// return to pre-battle preparations
			currentScreen = CURR_SCREEN_SELECT_OPTION;
			selectedPos.Set(-47, 52.5f, 0);
			i_selectedOptionCounter = 0;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (b_showArrowAtEQs == true)
				b_showArrowAtEQs = false;
			else
			{
				currentScreen = CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER;
				selectedPos.Set(-60, 40, 0);
				i_selectedUnitsCounter = 0;
				b_showArrowAtEQs = false;
			}
		}
		else if (currentScreen == CURR_SCREEN_SHOP)
		{
			if (b_showPlayerItems == false && b_showShopItems == false)
			{
				// return to pre-battle preparations
				currentScreen = CURR_SCREEN_SELECT_OPTION;
				selectedPos.Set(-47, 52.5f, 0);
				i_selectedOptionCounter = 0;
				b_showShopItems = false;
				b_showPlayerItems = false;
			}
			else if (b_showShopItems == true && b_showPlayerItems == false)
			{
				if (b_showItemStatsAtShop == false)
				{
					b_showShopItems = false;
					i_shopCursor = 0;
				}
				else
					b_showItemStatsAtShop = false;
			}
			else if (b_showPlayerItems == true && b_showShopItems == false)
			{
				if (b_showItemStatsAtShop == false)
				{
					b_showPlayerItems = false;
					i_shopCursor = 0;
				}
				else
					b_showItemStatsAtShop = false;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_RIGHT))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.x += 94;
			if (selectedPos.x > 47)
				selectedPos.x = -47;

			i_selectedOptionCounter += 1;
			if (i_selectedOptionCounter == 2)
				i_selectedOptionCounter = 0;
			else if (i_selectedOptionCounter == 4)
				i_selectedOptionCounter = 2;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.x += 30;
			if (selectedPos.x > 60)
				selectedPos.x = -60;

			i_selectedUnitsCounter += 1;
			if (i_selectedUnitsCounter == 5)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 10)
				i_selectedUnitsCounter = 5;
			else if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 10;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			i_selectedUnitsCounter+=1;
			if (i_selectedUnitsCounter >= PlayerInfo::GetInstance()->availableUnits.size())
				i_selectedUnitsCounter = 0;
			selectedPos.x += 30;
			if (selectedPos.x > 60)
				selectedPos.x = -60;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.x += 30;
			if (selectedPos.x > 60)
				selectedPos.x = -60;

			i_selectedUnitsCounter += 1;
			if (i_selectedUnitsCounter == 5)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 10)
				i_selectedUnitsCounter = 5;
			else if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 10;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (b_showArrowAtEQs == false)
			{
				selectedPos.x += 20;
				if (selectedPos.x > -20)
					selectedPos.x = -80;

				i_selectedEquipmentCounter += 1;
				if (i_selectedEquipmentCounter == 4)
					i_selectedEquipmentCounter = 0;
				else if (i_selectedEquipmentCounter == 8)
					i_selectedEquipmentCounter = 4;
				else if (i_selectedEquipmentCounter == 12)
					i_selectedEquipmentCounter = 8;
				else if (i_selectedEquipmentCounter == 16)
					i_selectedEquipmentCounter = 12;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_LEFT))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.x -= 94;
			if (selectedPos.x < -47)
				selectedPos.x = 47;

			i_selectedOptionCounter -= 1;
			if (i_selectedOptionCounter == -1)
				i_selectedOptionCounter = 1;
			else if (i_selectedOptionCounter == 1)
				i_selectedOptionCounter = 3;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.x -= 30;
			if (selectedPos.x < -60)
				selectedPos.x = 60;

			i_selectedUnitsCounter -= 1;
			if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 4;
			else if (i_selectedUnitsCounter == 4)
				i_selectedUnitsCounter = 9;
			else if (i_selectedUnitsCounter == 9)
				i_selectedUnitsCounter = 14;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			i_selectedUnitsCounter -= 1;
			if (i_selectedUnitsCounter < 0)
				i_selectedUnitsCounter = PlayerInfo::GetInstance()->availableUnits.size() - 1;

			selectedPos.x -= 30;
			if (selectedPos.x < -60)
				selectedPos.x = 60;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.x -= 30;
			if (selectedPos.x < -60)
				selectedPos.x = 60;

			i_selectedUnitsCounter -= 1;
			if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 4;
			else if (i_selectedUnitsCounter == 4)
				i_selectedUnitsCounter = 9;
			else if (i_selectedUnitsCounter == 9)
				i_selectedUnitsCounter = 14;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (b_showArrowAtEQs == false)
			{
				selectedPos.x -= 20;
				if (selectedPos.x < -80)
					selectedPos.x = -20;

				i_selectedEquipmentCounter -= 1;
				if (i_selectedEquipmentCounter == -1)
					i_selectedEquipmentCounter = 3;
				else if (i_selectedEquipmentCounter == 3)
					i_selectedEquipmentCounter = 7;
				else if (i_selectedEquipmentCounter == 7)
					i_selectedEquipmentCounter = 11;
				else if (i_selectedEquipmentCounter == 11)
					i_selectedEquipmentCounter = 15;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_DOWN))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.y -= 74.5f;
			if (selectedPos.y < -22.f)
				selectedPos.y = 52.5f;

			i_selectedOptionCounter += 2;
			if (i_selectedOptionCounter == 4)
				i_selectedOptionCounter = 0;
			else if (i_selectedOptionCounter == 5)
				i_selectedOptionCounter = 1;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.y -= 30;
			if (selectedPos.y < -70)
				selectedPos.y = -10;

			i_selectedUnitsCounter += 5;
			if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 16)
				i_selectedUnitsCounter = 1;
			else if (i_selectedUnitsCounter == 17)
				i_selectedUnitsCounter = 2;
			else if (i_selectedUnitsCounter == 18)
				i_selectedUnitsCounter = 3;
			else if (i_selectedUnitsCounter == 19)
				i_selectedUnitsCounter = 4;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			if (b_statusCursor == 0)
				b_statusCursor = 1;
			else
				b_statusCursor = 0;

			statusCursorPos.y -= 30;
			if (statusCursorPos.y < -70)
				statusCursorPos.y = -40;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.y -= 40;
			if (selectedPos.y < -40)
				selectedPos.y = 40;

			i_selectedUnitsCounter += 5;
			if (i_selectedUnitsCounter == 15)
				i_selectedUnitsCounter = 0;
			else if (i_selectedUnitsCounter == 16)
				i_selectedUnitsCounter = 1;
			else if (i_selectedUnitsCounter == 17)
				i_selectedUnitsCounter = 2;
			else if (i_selectedUnitsCounter == 18)
				i_selectedUnitsCounter = 3;
			else if (i_selectedUnitsCounter == 19)
				i_selectedUnitsCounter = 4;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (b_showArrowAtEQs == false)
			{
				selectedPos.y -= 31;
				if (selectedPos.y < -52)
					selectedPos.y = 41;

				i_selectedEquipmentCounter += 4;
				if (i_selectedEquipmentCounter == 16)
					i_selectedEquipmentCounter = 0;
				else if (i_selectedEquipmentCounter == 17)
					i_selectedEquipmentCounter = 1;
				else if (i_selectedEquipmentCounter == 18)
					i_selectedEquipmentCounter = 2;
				else if (i_selectedEquipmentCounter == 19)
					i_selectedEquipmentCounter = 3;
			}
			else
			{
				if (b_equipmentCursor == 0)
					b_equipmentCursor = 1;
				else
					b_equipmentCursor = 0;

				equipmentCursorPos.y -= 30;
				if (equipmentCursorPos.y < -70)
					equipmentCursorPos.y = -40;
			}
		}
		else if (currentScreen == CURR_SCREEN_SHOP)
		{
			if (b_showShopItems == false && b_showPlayerItems == false)
			{
				selectedPos.y -= 61;
				if (selectedPos.y < -60)
					selectedPos.y = 62;

				i_selectedShopCounter += 1;
				if (i_selectedShopCounter == 3)
					i_selectedShopCounter = 0;
			}
			else if (b_showShopItems == true)
			{
				if (i_shopCursor < PlayerInfo::GetInstance()->shop.size() - 1)
					i_shopCursor += 1;
			}
			else if (b_showPlayerItems == true)
			{
				if (i_shopCursor < PlayerInfo::GetInstance()->inventory.size() - 1)
					i_shopCursor += 1;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyReleased(VK_UP))
	{
		if (currentScreen == CURR_SCREEN_SELECT_OPTION)
		{
			selectedPos.y += 74.5f;
			if (selectedPos.y > 52.5f)
				selectedPos.y = -22.f;

			i_selectedOptionCounter -= 2;
			if (i_selectedOptionCounter == -2)
				i_selectedOptionCounter = 2;
			else if (i_selectedOptionCounter == -1)
				i_selectedOptionCounter = 3;
		}
		else if (currentScreen == CURR_SCREEN_SELECT_UNITS)
		{
			selectedPos.y += 30;
			if (selectedPos.y > -10)
				selectedPos.y = -70;

			i_selectedUnitsCounter -= 5;
			if (i_selectedUnitsCounter == -5)
				i_selectedUnitsCounter = 10;
			else if (i_selectedUnitsCounter == -4)
				i_selectedUnitsCounter = 11;
			else if (i_selectedUnitsCounter == -3)
				i_selectedUnitsCounter = 12;
			else if (i_selectedUnitsCounter == -2)
				i_selectedUnitsCounter = 13;
			else if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 14;
		}
		else if (currentScreen == CURR_SCREEN_VIEW_STATUS)
		{
			if (b_statusCursor == 0)
				b_statusCursor = 1;
			else
				b_statusCursor = 0;

			statusCursorPos.y += 30;
			if (statusCursorPos.y > -40)
				statusCursorPos.y = -70;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
		{
			selectedPos.y += 40;
			if (selectedPos.y > 40)
				selectedPos.y = -40;

			i_selectedUnitsCounter -= 5;
			if (i_selectedUnitsCounter == -5)
				i_selectedUnitsCounter = 10;
			else if (i_selectedUnitsCounter == -4)
				i_selectedUnitsCounter = 11;
			else if (i_selectedUnitsCounter == -3)
				i_selectedUnitsCounter = 12;
			else if (i_selectedUnitsCounter == -2)
				i_selectedUnitsCounter = 13;
			else if (i_selectedUnitsCounter == -1)
				i_selectedUnitsCounter = 14;
		}
		else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
		{
			if (b_showArrowAtEQs == false)
			{
				selectedPos.y += 31;
				if (selectedPos.y > 41)
					selectedPos.y = -52;

				i_selectedEquipmentCounter -= 4;
				if (i_selectedEquipmentCounter == -4)
					i_selectedEquipmentCounter = 12;
				else if (i_selectedEquipmentCounter == -3)
					i_selectedEquipmentCounter = 13;
				else if (i_selectedEquipmentCounter == -2)
					i_selectedEquipmentCounter = 14;
				else if (i_selectedEquipmentCounter == -1)
					i_selectedEquipmentCounter = 15;
			}
			else
			{
				if (b_equipmentCursor == 0)
					b_equipmentCursor = 1;
				else
					b_equipmentCursor = 0;

				equipmentCursorPos.y += 30;
				if (equipmentCursorPos.y > -40)
					equipmentCursorPos.y = -70;
			}
		}
		else if (currentScreen == CURR_SCREEN_SHOP)
		{
			if (b_showShopItems == false && b_showPlayerItems == false)
			{
				selectedPos.y += 61;
				if (selectedPos.y > 62)
					selectedPos.y = -60;

				i_selectedShopCounter -= 1;
				if (i_selectedShopCounter == -1)
					i_selectedShopCounter = 2;
			} 
			else if (b_showShopItems == true || b_showPlayerItems == true)
			{
				if (i_shopCursor > 0)
					i_shopCursor -= 1;
			}
		}
	}
}

void PartySelectScene::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->DetachCamera();

	EntityManager::GetInstance()->RenderUI();

	if (currentScreen == CURR_SCREEN_SELECT_OPTION)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selectOption"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(87, 40 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();

		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), message, Vector3(-30, 15, 10), 15.f, Color(0, 0, 1));
	}
	else if (currentScreen == CURR_SCREEN_SELECT_UNITS) // In the select units screen
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("PartySelectBG"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(15, 15 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();

		for (int y = 0, counter = 0; y >= -3; y--)
		{
			for (int x = 0; x < 5; x++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(-60 + (x * 30), (y * 30) - 10, 0);
				modelStack.Scale(15, 15 * 16 / 9, 1);
				RenderHelper::RenderMesh(PlayerInfo::GetInstance()->availableUnits.at(counter)->getPortrait());
				modelStack.PopMatrix();
				counter++;
				if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
					break;
			}
			if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
				break;
		}


		for (int i = 0, counter = 0; i < 5; i++)
		{
			if (counter >= PlayerInfo::GetInstance()->party.size())
				break;
			modelStack.PushMatrix();
			modelStack.Translate(-70.3f + (i * 47), 50, 2);
			modelStack.Scale(15, 15 * 16 / 9, 1);
			RenderHelper::RenderMesh(PlayerInfo::GetInstance()->party.at(i)->getPortrait());
			modelStack.PopMatrix();
			counter++;
		}
	}
	else if (currentScreen == CURR_SCREEN_VIEW_STATUS) // Viewing status screen
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("statusScreen"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(statusCursorPos.x, statusCursorPos.y, 10);
		modelStack.Scale(20, 10, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rightFacingArrow"));
		modelStack.PopMatrix();

		if (PlayerInfo::GetInstance()->party.size() >= 4 && PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->b_inParty == false)
		{
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Party is full!", Vector3(12.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
		}
		else
		{
			if (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->b_inParty == false)
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Add to Party", Vector3(12.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
			else
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Remove from Party", Vector3(12.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
		}
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Back", Vector3(12.f, -70.f, 1.f), 17.f, Color(1, 1, 1));
		// This character's portrait
		modelStack.PushMatrix();
		modelStack.Translate(-camera.f_OrthoSize + 37.5f, 0, 1);
		modelStack.Scale(50, 50 * 16 / 9, 1);
		RenderHelper::RenderMesh(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getPortrait());
		modelStack.PopMatrix();

		// This character's STR
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getSTR()), Vector3(3, 54, 1), 10.f, Color(1, 1, 1));
		// This character's DEX
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getDEX()), Vector3(3, 37, 1), 10.f, Color(1, 1, 1));
		// This character's LUK
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getLUK()), Vector3(3, 20, 1), 10.f, Color(1, 1, 1));
		// This character's HP
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getMaxHP()), Vector3(3, 3, 1), 10.f, Color(1, 1, 1));

		// This character's Attack Range
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->i_attackRange), Vector3(60, 54, 1), 10.f, Color(1, 1, 1));
		// This character's Movement Range
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->i_movementCost), Vector3(60, 37, 1), 10.f, Color(1, 1, 1));
		// This character's weapon's name
		if (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getWeapon() == nullptr)
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), " ", Vector3(60, 20, 1), 10.f, Color(1, 1, 1));
		else
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getWeapon()->s_Name), Vector3(60.f, 20, 1), 10.f, Color(1, 1, 1));
		// This character's armor's name
		if (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getArmor() == nullptr)
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), " ", Vector3(60, 20, 1), 10.f, Color(1, 1, 1));
		else
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getArmor()->s_Name), Vector3(60.f, 3, 1), 10.f, Color(1, 1, 1));
		// This character's name
		RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->availableUnits.at(i_selectedUnitsCounter)->getName()), Vector3(-75, 69, 1), 15.f, Color(1, 1, 1));
	}
	else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_SELECT_CHARACTER)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, -1);
		modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("changeWhoseEQ"));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(15, 15 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();

		for (int y = 1, counter = 0; y >= -2; y--)
		{
			for (int x = 0; x < 5; x++)
			{
				modelStack.PushMatrix();
				modelStack.Translate(-60 + (x * 30), (y * 40), 0);
				modelStack.PushMatrix();
				modelStack.Scale(15, 15 * 16 / 9, 1);
				RenderHelper::RenderMesh(PlayerInfo::GetInstance()->availableUnits.at(counter)->getPortrait());
				modelStack.PopMatrix();
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), PlayerInfo::GetInstance()->availableUnits.at(counter)->getName(), Vector3(-4, -17, 0), 7.f, Color(1, 1, 1));
				modelStack.PopMatrix();
				counter++;
				if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
					break;
			}
			if (counter >= PlayerInfo::GetInstance()->availableUnits.size())
				break;
		}
	}
	else if (currentScreen == CURR_SCREEN_CHANGE_EQUIPMENT_CHANGE_EQ)
	{
		if (b_showArrowAtEQs == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0, -1);
			modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
			if (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->b_isWeapon)
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("weaponStats"));
			else if (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->b_isArmor)
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("armorStats"));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(equipmentCursorPos.x, equipmentCursorPos.y, 10);
			modelStack.Scale(20, 10, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rightFacingArrow"));
			modelStack.PopMatrix();

			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->s_Name), Vector3(30, 69, 1), 15.f, Color(1, 1, 1));
			if (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->b_isWeapon)
			{
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->s_ownerName), Vector3(50, 30.5f, 1), 12.f, Color(1, 1, 1));
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->i_damageValue), Vector3(50, 15.5f, 1), 12.f, Color(1, 1, 1));
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->i_weaponAccuracy) + "%", Vector3(50, 0.5f, 1), 12.f, Color(1, 1, 1));
			}
			else if (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->b_isArmor)
			{
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), (PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->s_ownerName), Vector3(50, 52, 1), 12.f, Color(1, 1, 1));
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->i_strBoost), Vector3(50, 39.5f, 1), 12.f, Color(1, 1, 1));
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->i_dexBoost), Vector3(50, 27, 1), 12.f, Color(1, 1, 1));
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->i_lukBoost), Vector3(50, 14.5f, 1), 12.f, Color(1, 1, 1));
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_selectedEquipmentCounter + i_eqToShow)->i_hpBoost), Vector3(50, 2, 1), 12.f, Color(1, 1, 1));
			}

			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Equip", Vector3(50.f, -40.f, 1.f), 17.f, Color(1, 1, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Close", Vector3(50.f, -70.f, 1.f), 17.f, Color(1, 1, 1));
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0, -1);
			modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selectEquip"));
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(selectedPos.x, selectedPos.y, 1);
		modelStack.Scale(15, 15 * 16 / 9, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
		modelStack.PopMatrix();

		if (PlayerInfo::GetInstance()->inventory.size() > 0)
		{
			for (int y = 1, counter = 0; y > -4; y--)
			{
				if ((counter + i_eqToShow) > PlayerInfo::GetInstance()->inventory.size() - 1 || counter > 15)
					break;
				for (int x = 0; x < 4; x++)
				{
					if ((counter + i_eqToShow) > PlayerInfo::GetInstance()->inventory.size() - 1 || counter > 15)
						break;
					modelStack.PushMatrix();
					modelStack.Translate(-80 + (x * 20), (y * 31) + 10, 0);
					modelStack.PushMatrix();
					modelStack.Scale(15, 15 * 16 / 9, 1);
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(PlayerInfo::GetInstance()->inventory.at(counter + i_eqToShow)->itemPortrait));
					modelStack.PopMatrix();
					modelStack.PushMatrix();
					modelStack.Translate(3, -6, 1);
					modelStack.Scale(7, 7 * 16 / 9, 1);
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(PlayerInfo::GetInstance()->inventory.at(counter + i_eqToShow)->s_ownerName));
					modelStack.PopMatrix();
					if (PlayerInfo::GetInstance()->inventory.at(counter + i_eqToShow)->b_isWeapon)
						RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "W", Vector3(-3, -6, 1), 7, Color(0, 0, 0));
					else if (PlayerInfo::GetInstance()->inventory.at(counter + i_eqToShow)->b_isArmor)
						RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "A", Vector3(-3, -6, 1), 7, Color(0, 0, 0));
					modelStack.PopMatrix();
					counter++;
				}
			}
		}
	}
	else if (currentScreen == CURR_SCREEN_SHOP)
	{
		if (b_showItemStatsAtShop == true)
		{
			if (b_showShopItems == true && PlayerInfo::GetInstance()->shop.size() > 0)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, -1);
				modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
				if (PlayerInfo::GetInstance()->shop.at(i_shopCursor)->b_isWeapon)
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shopWeaponStats"));
				else if (PlayerInfo::GetInstance()->shop.at(i_shopCursor)->b_isArmor)
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shopArmorStats"));
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-30, 47, 5);
				modelStack.Scale(60, 35 * 16 / 9, 5);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->itemPortrait));
				modelStack.PopMatrix();

				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), PlayerInfo::GetInstance()->shop.at(i_shopCursor)->s_Name, Vector3(-42, 7, 5), 15.f, Color(1, 1, 1));
				if (PlayerInfo::GetInstance()->shop.at(i_shopCursor)->b_isWeapon)
				{
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_damageValue), Vector3(-35, -23, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_weaponAccuracy) + "%", Vector3(-35, -38, 5), 15.f, Color(1, 1, 1));
				}
				else if (PlayerInfo::GetInstance()->shop.at(i_shopCursor)->b_isArmor)
				{
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_strBoost), Vector3(-45, -16, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_dexBoost), Vector3(-45, -31, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_lukBoost), Vector3(-45, -46, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->shop.at(i_shopCursor)->i_hpBoost), Vector3(-45, -62, 5), 15.f, Color(1, 1, 1));
				}
			}
			else if (b_showPlayerItems == true && PlayerInfo::GetInstance()->inventory.size() > 0)
			{
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, -1);
				modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
				if (PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->b_isWeapon)
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shopWeaponStats"));
				else if (PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->b_isArmor)
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shopArmorStats"));
				modelStack.PopMatrix();

				modelStack.PushMatrix();
				modelStack.Translate(-30, 47, 5);
				modelStack.Scale(60, 35 * 16 / 9, 5);
				RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->itemPortrait));
				modelStack.PopMatrix();

				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->s_Name, Vector3(-42, 7, 5), 15.f, Color(1, 1, 1));
				if (PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->b_isWeapon)
				{
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_damageValue), Vector3(-35, -23, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_weaponAccuracy), Vector3(-35, -38, 5), 15.f, Color(1, 1, 1));
				}
				else if (PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->b_isArmor)
				{
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_strBoost), Vector3(-45, -16, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_dexBoost), Vector3(-45, -31, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_lukBoost), Vector3(-45, -46, 5), 15.f, Color(1, 1, 1));
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "+" + std::to_string(PlayerInfo::GetInstance()->inventory.at(i_shopCursor)->i_hpBoost), Vector3(-45, -62, 5), 15.f, Color(1, 1, 1));
				}
			}
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(0, 0, -1);
			modelStack.Scale(2 * camera.f_OrthoSize, 2 * camera.f_OrthoSize, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shop"));
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(selectedPos.x, selectedPos.y, 1);
			modelStack.Scale(43, 26.5f * 16 / 9, 1);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("selected"));
			modelStack.PopMatrix();

			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->gold), Vector3(3, 72, 3), 15.f, Color(1, 1, 1));
			RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->inventory.size()), Vector3(82.5f, 72, 3), 15.f, Color(1, 1, 1));
			if (b_showShopItems == true)
			{
				if (PlayerInfo::GetInstance()->shop.size() > 0)
				{
					modelStack.PushMatrix();
					modelStack.Translate(-18, 10, 10);
					modelStack.Scale(20, 10, 1);
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rightFacingArrow"));
					modelStack.PopMatrix();

					for (int i = i_shopCursor, counter = 0; i < i_shopCursor + 4; i++, counter++)
					{
						if (i > PlayerInfo::GetInstance()->shop.size() - 1)
							break;
						modelStack.PushMatrix();
						modelStack.Translate(0, 10 - (counter * 25), 1);
						modelStack.PushMatrix();
						modelStack.Scale(15, 15, 1);
						if (PlayerInfo::GetInstance()->shop.at(i)->b_isWeapon)
							RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("swordPortrait"));
						else if (PlayerInfo::GetInstance()->shop.at(i)->b_isArmor)
							RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("armorPortrait"));
						modelStack.PopMatrix();
						RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), PlayerInfo::GetInstance()->shop.at(i)->s_Name, Vector3(19, 0, 1), 13.f, Color(1, 1, 1));
						RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->shop.at(i)->i_Price), Vector3(65, 0, 1), 13.f, Color(1, 1, 1));
						modelStack.PopMatrix();
					}
				}
				else
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Nothing to see here", Vector3(-5, -22, 3), 15.f, Color(1, 1, 1));
			}
			else if (b_showPlayerItems == true)
			{
				RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), message, Vector3(-35, 60, 6), 15.f, Color(1, 1, 0));
				if (PlayerInfo::GetInstance()->inventory.size() > 0)
				{
					modelStack.PushMatrix();
					modelStack.Translate(-18, 10, 10);
					modelStack.Scale(20, 10, 1);
					RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("rightFacingArrow"));
					modelStack.PopMatrix();
					for (int i = i_shopCursor, counter = 0; i < i_shopCursor + 4; i++, counter++)
					{
						if (i > PlayerInfo::GetInstance()->inventory.size() - 1)
							break;
						modelStack.PushMatrix();
						modelStack.Translate(0, 10 - (counter * 25), 1);
						modelStack.PushMatrix();
						modelStack.Scale(15, 15, 1);
						if (PlayerInfo::GetInstance()->inventory.at(i)->b_isWeapon)
							RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("swordPortrait"));
						else if (PlayerInfo::GetInstance()->inventory.at(i)->b_isArmor)
							RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("armorPortrait"));
						modelStack.PopMatrix();
						RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), PlayerInfo::GetInstance()->inventory.at(i)->s_Name, Vector3(19, 0, 1), 13.f, Color(1, 1, 1));
						RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), std::to_string(PlayerInfo::GetInstance()->inventory.at(i)->i_Price >> 1), Vector3(65, 0, 1), 13.f, Color(1, 1, 1));
						modelStack.PopMatrix();
					}
				}
				else
					RenderHelper::RenderTextOnScreen(MeshBuilder::GetInstance()->GetMesh("text"), "Nothing to see here", Vector3(-5, -22, 3), 15.f, Color(1, 1, 1));
			}
		}
	}
}

void PartySelectScene::Exit()
{
	GraphicsManager::GetInstance()->DetachCamera();
}
