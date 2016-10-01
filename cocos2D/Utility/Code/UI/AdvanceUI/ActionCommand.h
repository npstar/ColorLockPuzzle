#ifndef __ACTION_COMMAND_H__
#define __ACTION_COMMAND_H__

#include "cocos2d.h"

#include "../../LoadDataXML.h"
#include "../../json/include/json/json.h"
#include "RKTool_Code/RKList.h"
#include "RKBaseLog/Debug.h"

using namespace RKUtils;
using namespace ui;
namespace Utility
{
	namespace UI_Widget
	{	
		class ActionCommand
		{
		private:

			struct ActionDetail
			{

				xml::BasicDec * value_action;
				RKString		name_action;
				float			time_action;

				xml::BasicDec *	_hook_value;
			public:
				ActionDetail()
				{
					value_action = new xml::BasicDec();
					_hook_value = nullptr;
					name_action = "";
					time_action = 1.0f;
				}

				virtual ~ActionDetail()
				{
					if (value_action)
					{
						delete value_action;
						value_action = nullptr;
					}
					if (_hook_value)
					{
						delete _hook_value;
						_hook_value = nullptr;
					}

					name_action = "";
					time_action = 1.0f;
				}

				FiniteTimeAction * GetAction(unsigned int & action, cocos2d::Node * it_res, cocos2d::Node * parent, bool IsLayer = false);

				void ParseActionDetail(const Json::Value value, RKString name);

				void SetValueHook(xml::BasicDec value, RKString name_value);

				void ClearHookCommand(RKString name_value = "");

				void SetTimeAction(float time) { time_action = time; }
				ActionDetail * Clone();
			};

			struct ActionRepeat
			{
				std::vector<std::vector<ActionDetail*>> p_list_action_;
				int number_repeat;

				ActionRepeat()
				{
					p_list_action_.clear();
					number_repeat = 1;
				}

				virtual ~ActionRepeat()
				{
					number_repeat = 0;
					for (unsigned int i = 0; i < p_list_action_.size(); i++)
					{
						for (unsigned int j = 0; j < p_list_action_.at(i).size(); j++)
						{
							delete p_list_action_.at(i).at(j);
						}
						p_list_action_.at(i).clear();
					}
					p_list_action_.clear();
				}

			};

			std::map<RKString, ActionRepeat*> p_list_actions;

			Json::Value JRootValue;

			ActionRepeat * ReadJsonAction(const  Json::Value value);

			ActionDetail* ParseDetailJsonAction(const Json::Value value);

			Sequence * GetSquenceActionDetail(RKString name, int idx, unsigned int & action_tag, cocos2d::Node * it_res, cocos2d::Node * parent, bool IsLayer = false) const;

		public:
			ActionCommand();

			virtual ~ActionCommand();

			bool  ReadJsonString(const char * jstr);

			ActionInterval * GetSequenceActionData(unsigned int & action_tag, cocos2d::Node * it_res, cocos2d::Node * parent,int & flag, bool IsLayer = false) const;

			ActionCommand * Clone();

			ActionDetail * GetActionDetail(RKString name_act, int idx_squence, int idx_detail);
		};

	}
}

#endif //__ACTION_COMMAND_H__
