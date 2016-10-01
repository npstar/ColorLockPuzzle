#include "wTextField.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		TextFieldWidget::TextFieldWidget()
		{
			p_typeUI = UI_TYPE::UI_TEXT_FIELD;
			p_border_width = 0;
			p_border_color = Color4B::WHITE;
			p_current_text = "";
			p_default_place_holder = "";
			p_min_size_scale = -1;

		}
		TextFieldWidget::~TextFieldWidget()
		{
			p_current_text = "";
			p_default_place_holder = "";
		}

		WidgetEntity * TextFieldWidget::Clone()
		{
			TextFieldWidget * p_clone = new TextFieldWidget();
			//copy all value (excep pointer)
#ifdef OS_IOS
			memcpy(p_clone, this, sizeof(p_clone));
#else
			memcpy_s(p_clone, sizeof(p_clone), this, sizeof(this));
#endif
			//copy resource
			p_clone->p_Resource = p_Resource->clone();
			//remove child remain
			p_clone->p_Resource->removeAllChildrenWithCleanup(true);
			//
			p_clone->p_border_color = p_border_color;
			p_clone->p_border_width = p_border_width;

			if (p_clone->p_border_width > 0)
			{
				static_cast<Text*>(p_clone->p_Resource)->enableOutline(p_clone->p_border_color, p_clone->p_border_width);
			}

			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;
		}

		/////////////////////////////////////////////////////////////
		void TextFieldWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
		}

		void TextFieldWidget::SetSize(cocos2d::Vec2 s)
		{
			cocos2d::Vec2 p_Size(1, 1);

			float p_size_font = (static_cast<TextField*>(p_Resource))->getFontSize();

			float add_ = 1;
			if (GetGameSize().width <= p_min_size_scale && p_min_size_scale > 0)
			{
				add_ *= (p_min_size_scale / GetGameSize().width);
			}

			p_Size.x = (s.x * add_ )/ p_size_font;
			p_Size.y = (s.y * add_ )/ p_size_font;

			//GetResource()->setScaleX(p_Size.x);
			//GetResource()->setScaleY(p_Size.y);

			static_cast<TextField*>(GetResource())->setFontSize((int)(s.x * add_));
		}

		void TextFieldWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{
			p_current_text = xml_value->GeneralValue->GetDataChar("source");
			p_default_place_holder = p_current_text;

			p_min_size_scale = xml_value->GeneralValue->GetDataInt("min_size_scale");

			Vec2 origin_ScaleRatio = xml_value->OriginValue->GetDataVector2("scale_ratio");

			Vec2 size_origin = Vec2(origin_ScaleRatio.x * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x),
				origin_ScaleRatio.y * (GetGameSize().width / xml_value->p_menu_widget->DesignSize.x));

			int font_idx = xml_value->GeneralValue->GetDataInt("font_index");

			Label * label = TextureMgr->GetFontByIdx(font_idx);
			float size_text = XMLMgr->GetFontDecByIdx(font_idx)->d_size;

			TextField* text = TextField::create(p_current_text.GetString(), label->getTTFConfig().fontFilePath.c_str(), size_text);

			text->setPlaceHolderColor(xml_value->GeneralValue->GetDataColor("tint_place_holder"));
			text->setPasswordEnabled(xml_value->GeneralValue->GetDataInt("pass_input") == 1);

			Vec2 anchor_point = xml_value->GeneralValue->GetDataVector2("anchor_point");
			text->setAnchorPoint(anchor_point);
			text->setTextColor(xml_value->GeneralValue->GetDataColor("tint"));

			int border_width = xml_value->GeneralValue->GetDataInt("border_width");
			if (border_width > 0)
			{
				//Vec2 design_size = xml_value->p_parent_layer->p_menu_widget->DesignSize;
				//border_width = (int)(((float)border_width) * GetGameSize().width / design_size.x);
				//text->enableOutline(xml_value->GeneralValue->GetDataColor("border_corlor"), border_width);

				//p_border_width = border_width;
				//p_border_color = xml_value->GeneralValue->GetDataColor("border_corlor");
			}

			p_Resource = text;
		}

		RKString TextFieldWidget::GetText()
		{
			return static_cast<TextField*>(p_Resource)->getString();
		}

		void TextFieldWidget::SetText(RKString text)
		{
			p_current_text = text;
			static_cast<TextField*>(p_Resource)->setString(text.GetString());
		}

		void TextFieldWidget::SetColor(Color3B p)
		{
			static_cast<TextField*>(p_Resource)->setTextColor(Color4B(p.r, p.g, p.b, 255));
		}

		RKString TextFieldWidget::GetDefaultPlaceHolder()
		{
			return p_default_place_holder;
		}
	}
}