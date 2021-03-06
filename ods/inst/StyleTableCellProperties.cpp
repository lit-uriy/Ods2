#include "StyleTableCellProperties.hpp"

#include "../attr/Border.hpp"
#include "../attr/StyleBorderLineWidth.hpp"
#include "../Ns.hpp"
#include "../ns.hxx"
#include "../Tag.hpp"
#include "../VAlign.hpp"

namespace ods { // ods::
namespace inst { // ods::inst::

StyleTableCellProperties::StyleTableCellProperties(Abstract *parent, Tag *tag)
: Abstract(parent, parent->ns(), id::StyleTableCellProperties)
{
	if (tag != nullptr)
		Init(tag);
}

StyleTableCellProperties::StyleTableCellProperties(const StyleTableCellProperties &cloner)
: Abstract(cloner)
{}

StyleTableCellProperties::~StyleTableCellProperties()
{
	delete fo_border_;
	delete fo_border_left_;
	delete fo_border_top_;
	delete fo_border_right_;
	delete fo_border_bottom_;
	delete fo_background_color_;
	delete style_vertical_align_;
	delete style_border_line_width_;
}

void
StyleTableCellProperties::border(ods::attr::Border *border)
{
	delete fo_border_;
	
	if (border == nullptr)
		fo_border_ = nullptr;
	else
		fo_border_ = border->Clone();
}

void
StyleTableCellProperties::border_left(ods::attr::Border *border)
{
	delete fo_border_left_;
	
	if (border == nullptr)
		fo_border_left_ = nullptr;
	else
		fo_border_left_ = border->Clone();
}

void
StyleTableCellProperties::border_top(ods::attr::Border *border)
{
	delete fo_border_top_;
	
	if (border == nullptr)
		fo_border_top_ = nullptr;
	else
		fo_border_top_ = border->Clone();
}

void
StyleTableCellProperties::border_right(ods::attr::Border *border)
{
	delete fo_border_right_;
	
	if (border == nullptr)
		fo_border_right_ = nullptr;
	else
		fo_border_right_ = border->Clone();
}

void
StyleTableCellProperties::border_bottom(ods::attr::Border *border)
{
	delete fo_border_bottom_;
	
	if (border == nullptr)
		fo_border_bottom_ = nullptr;
	else
		fo_border_bottom_ = border->Clone();
}

Abstract*
StyleTableCellProperties::Clone(Abstract *parent) const
{
	auto *p = new StyleTableCellProperties(*this);
	
	if (parent != nullptr)
		p->parent(parent);
	
	if (fo_background_color_ != nullptr)
		p->fo_background_color_ = new QColor(*fo_background_color_);
	
	if (fo_border_ != nullptr)
		p->fo_border_ = fo_border_->Clone();
	
	if (fo_border_left_ != nullptr)
		p->fo_border_left_ = fo_border_left_->Clone();
	
	if (fo_border_top_ != nullptr)
		p->fo_border_top_ = fo_border_top_->Clone();
	
	if (fo_border_right_ != nullptr)
		p->fo_border_right_ = fo_border_right_->Clone();
	
	if (fo_border_bottom_ != nullptr)
		p->fo_border_bottom_ = fo_border_bottom_->Clone();
	
	if (style_border_line_width_ != nullptr)
		p->style_border_line_width_ = style_border_line_width_->Clone();
	
	p->fo_wrap_option_ = fo_wrap_option_;
	p->style_diagonal_bl_tr_ = style_diagonal_bl_tr_;
	p->style_diagonal_tl_br_ = style_diagonal_tl_br_;
	
	if (style_vertical_align_ != nullptr)
		p->style_vertical_align_ = style_vertical_align_->Clone();
	
	return p;
}

void
StyleTableCellProperties::Init(ods::Tag *tag)
{
	QString str;
	tag->Copy(ns_->fo(), ods::ns::kBackgroundColor, str);
	
	if (QColor::isValidColor(str))
	{
		delete fo_background_color_;
		fo_background_color_ = new QColor(str);
	}
	
	tag->Copy(ns_->style(), ods::ns::kDiagonalBlTr, style_diagonal_bl_tr_);
	tag->Copy(ns_->style(), ods::ns::kDiagonalTlBr, style_diagonal_tl_br_);
	
	tag->Copy(ns_->fo(), ods::ns::kBorder, str);
	fo_border_ = ods::attr::Border::FromString(str);
	
	tag->Copy(ns_->fo(), ods::ns::kBorderLeft, str);
	fo_border_left_ = ods::attr::Border::FromString(str);
	
	tag->Copy(ns_->fo(), ods::ns::kBorderTop, str);
	fo_border_top_ = ods::attr::Border::FromString(str);
	
	tag->Copy(ns_->fo(), ods::ns::kBorderRight, str);
	fo_border_right_ = ods::attr::Border::FromString(str);
	
	tag->Copy(ns_->fo(), ods::ns::kBorderBottom, str);
	fo_border_bottom_ = ods::attr::Border::FromString(str);
	
	tag->Copy(ns_->style(), ods::ns::kBorderLineWidth, str);
	style_border_line_width_ = ods::attr::StyleBorderLineWidth::FromString(str);
	
	tag->Copy(ns_->fo(), ods::ns::kWrapOption, fo_wrap_option_);
	
	tag->Copy(ns_->style(), ods::ns::kVerticalAlign, str);
	style_vertical_align_ = VAlign::FromString(str);
	
	ScanString(tag);
}

void
StyleTableCellProperties::SetBackgroundColor(const QColor &c)
{
	delete fo_background_color_;
	fo_background_color_ = new QColor(c);
}

void
StyleTableCellProperties::SetWrapOption(const bool flag)
{
	if (flag)
		fo_wrap_option_ = QLatin1String("wrap");
	else
		fo_wrap_option_.clear();
}

void
StyleTableCellProperties::style_border_line_width
	(const ods::attr::StyleBorderLineWidth *p)
{
	delete style_border_line_width_;
	
	if (p == nullptr)
		style_border_line_width_ = nullptr;
	else
		style_border_line_width_ = p->Clone();
}

void
StyleTableCellProperties::vertical_align(VAlign *p)
{
	delete style_vertical_align_;
	
	if (p == nullptr)
	{
		style_vertical_align_ = nullptr;
	} else {
		style_vertical_align_ = p->Clone();
	}
}

void
StyleTableCellProperties::WriteData(QXmlStreamWriter &xml)
{
	if (fo_background_color_ != nullptr)
	{
		Write(xml, ns_->fo(), ods::ns::kBackgroundColor,
			fo_background_color_->name());
	}
	
	if (style_vertical_align_ != nullptr)
	{
		Write(xml, ns_->style(), ods::ns::kVerticalAlign,
			style_vertical_align_->toString());
	}
	
	Write(xml, ns_->style(), ods::ns::kDiagonalBlTr, style_diagonal_bl_tr_);
	Write(xml, ns_->style(), ods::ns::kDiagonalTlBr, style_diagonal_tl_br_);
	
	if (fo_border_ != nullptr)
		Write(xml, ns_->fo(), ods::ns::kBorder, fo_border_->toString());
	
	if (fo_border_left_ != nullptr)
		Write(xml, ns_->fo(), ods::ns::kBorderLeft, fo_border_left_->toString());
	
	if (fo_border_top_ != nullptr)
		Write(xml, ns_->fo(), ods::ns::kBorderTop, fo_border_top_->toString());
	
	if (fo_border_right_ != nullptr)
		Write(xml, ns_->fo(), ods::ns::kBorderRight, fo_border_right_->toString());
	
	if (fo_border_bottom_ != nullptr)
		Write(xml, ns_->fo(), ods::ns::kBorderBottom, fo_border_bottom_->toString());
	
	if (style_border_line_width_ != nullptr)
	{
		Write(xml, ns_->style(), ods::ns::kBorderLineWidth,
			style_border_line_width_->toString());
	}
	
	Write(xml, ns_->fo(), ods::ns::kWrapOption, fo_wrap_option_);
	
	WriteNodes(xml);
}

} // ods::inst::
} // ods::
