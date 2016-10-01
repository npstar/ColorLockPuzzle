#include "wPageView.h"
#include "wLayout.h"
#include "LayerEntity.h"
namespace Utility
{
	namespace UI_Widget
	{
		/////////////////////////////////////////////////////////////
		PageViewWidget::PageViewWidget()
		{
			p_typeUI = UI_TYPE::UI_PAGEVIEW;
			p_refer_child = nullptr;

		}
		PageViewWidget::~PageViewWidget()
		{
			p_refer_child = nullptr;
		}

		WidgetEntity * PageViewWidget::Clone()
		{
			PageViewWidget * p_clone = new PageViewWidget();
			//copy all value (excep pointer)
#ifdef OS_IOS
            memcpy(p_clone,this,sizeof(p_clone));
#else
            memcpy_s(p_clone, sizeof(p_clone), this, sizeof(this));
#endif

			//copy resource
			p_clone->p_Resource = p_Resource->clone();
			//remove child remain
			p_clone->p_Resource->removeAllChildrenWithCleanup(true);
			//clone refer;
			p_clone->p_refer_child = static_cast<LayoutWidget*>(p_refer_child->Clone());

			//copy child
			CloneChild(p_clone);
			CloneThis(p_clone);

			return p_clone;
		}

		/////////////////////////////////////////////////////////////
		void PageViewWidget::InitParam(RKString name, xml::UIWidgetDec * xml_value)
		{
			InitTexture(xml_value);
			WidgetEntity::InitParam(name, xml_value);
		}

		void PageViewWidget::InitTexture(xml::UIWidgetDec * xml_value)
		{
			Vec2 inner_size = xml_value->GeneralValue->GetDataVector2("inner_size");

			inner_size.x = inner_size.x * GetGameSize().width / xml_value->p_menu_widget->DesignSize.x;
			inner_size.y = inner_size.y * GetGameSize().height / xml_value->p_menu_widget->DesignSize.y;

			PageView* panel = PageView::create();

			ui::PageView::Direction direction = (ui::PageView::Direction)xml_value->GeneralValue->GetDataInt("direction");
			panel->setDirection(direction);
			panel->setInnerContainerSize(cocos2d::Size(inner_size.x,inner_size.y));

			if (xml_value->GeneralValue->GetDataInt("clipping_enable") == 1)
			{
				panel->setClippingEnabled(true);
			}
			else
			{
				panel->setClippingEnabled(false);
			}
			int background_type = xml_value->GeneralValue->GetDataInt("background_type");

			if (background_type >= 0 && background_type <= 2)
			{
				panel->setBackGroundColorType((ui::Layout::BackGroundColorType)background_type);

				if (background_type == 1)
				{
					Color4B color_layout = xml_value->GeneralValue->GetDataColor("tint");
					panel->setBackGroundColor(Color3B(color_layout.r, color_layout.g, color_layout.b));
					RKUtils::BYTE opacity_layout = xml_value->GeneralValue->GetDataInt("opacity");
					panel->setBackGroundColorOpacity(opacity_layout);
				}
				else if (background_type == 2)
				{
					Color4B color_layout_from = xml_value->GeneralValue->GetDataColor("tint");
					Color4B color_layout_to = xml_value->GeneralValue->GetDataColor("tint_to");
					panel->setBackGroundColor(Color3B(color_layout_from.r, color_layout_from.g, color_layout_from.b),
						Color3B(color_layout_to.r, color_layout_to.g, color_layout_to.b));
					RKUtils::BYTE opacity_layout = xml_value->GeneralValue->GetDataInt("opacity");
					panel->setBackGroundColorOpacity(opacity_layout);
				}
			}
			else if (background_type == 3)
			{
				RKString name_image = xml_value->GeneralValue->GetDataChar("source");
				bool IsUseAtlast = xml_value->GeneralValue->GetDataInt("use_atlas") == 1;

				auto t_sprite = TextureMgr->GetSpriteByName(name_image.GetString());
				Vec2 size_srpite = t_sprite->getContentSize();
				//p_Resource->setContentSize(Size(size_srpite.x, size_srpite.y));

				panel->setBackGroundImage(name_image.GetString(), IsUseAtlast ? Widget::TextureResType::PLIST : Widget::TextureResType::LOCAL);
				bool IsScale9Enable = (xml_value->GeneralValue->GetDataInt("scale_9") == 1);
				p_isScale9Enable = IsScale9Enable;
				panel->setBackGroundImageScale9Enabled(IsScale9Enable);
			}
			else
			{
				PWARN1("Wrong input !");
			}
			//panel->setInnerContainerPosition(panel->getSize());
			p_Resource = panel;
		}
		
		void PageViewWidget::FindAndInsertChildWidget(xml::UILayerWidgetDec * xml_value)
		{
			for (int i = 0; i < xml_value->p_list_child_widget.size(); i++)
			{
				auto _widget_child = this->GetParentLayer()->InitWidgetEntity(xml_value->p_list_child_widget.at(i), this->GetParentLayer(), this);
				_widget_child->SetMenuParent(this->GetMenuParent());
				_widget_child->SetParentWidget(this);

				if (_widget_child->getUIWidgetType() != UI_TYPE::UI_LAYOUT
					||
					xml_value->p_list_child_widget.at(i)->GeneralValue->GetDataInt("not_default_layout") == 1
					)
				{
					int idx = static_cast<PageView*>(p_Resource)->getChildrenCount();

					static_cast<PageView*>(p_Resource)->insertPage(static_cast<Widget*>(_widget_child->GetResource()), idx);

					_widget_child->SetName(_widget_child->GetName() + std::to_string(idx));
					p_listChildWidget.push_back(_widget_child);
					_widget_child->GetResource()->setName((_widget_child->GetName() + std::to_string(idx)).GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
					_widget_child->GetResource()->setZOrder(p_listChildWidget.size());
					_widget_child->SetParentLayer(this->GetParentLayer());
				}
				else
				{
					p_refer_child = static_cast<LayoutWidget *>(_widget_child);
				}
			}
		}

		void PageViewWidget::SetSizeValueXML(xml::UIWidgetDec * xml_value)
		{
			ActionProcess::SetSizeValueXML(xml_value);

			int percent_ = xml_value->GeneralValue->GetDataInt("Threshold_scroll");

			static_cast<PageView*>(p_Resource)->setCustomScrollThreshold(percent_ * 0.01 * p_Resource->getContentSize().width);
		}

		WidgetEntity * PageViewWidget::CreateWidgetReferAtIdx(int idx)
		{
			auto page_view = static_cast<PageView*>(p_Resource);

			LayoutWidget * _layout_widget = static_cast<LayoutWidget*>(p_refer_child->Clone());
			page_view->insertPage(static_cast<Layout*>(_layout_widget->GetResource()), idx);

			_layout_widget->SetName(_layout_widget->GetName() + std::to_string(idx));
			p_listChildWidget.push_back(_layout_widget);
			_layout_widget->GetResource()->setName((_layout_widget->GetName() + std::to_string(idx)).GetString()); //this function is get the name define on xml and parse to the cocos2dx logic
			_layout_widget->GetResource()->setZOrder(p_listChildWidget.size());
			_layout_widget->SetParentLayer(this->GetParentLayer());

			return _layout_widget;
		}

		void PageViewWidget::PushBackItem(WidgetEntity * item)
		{
		}


		int PageViewWidget::GetCurrentPageIdx()
		{
			return static_cast<PageView*>(p_Resource)->getCurPageIndex();
		}

		void PageViewWidget::JumpToPageIdx(int idx)
		{
			static_cast<PageView*>(p_Resource)->setCurPageIndex(idx);
		}

	}
}