#include "examples2.hh"

#include "util/util.hh"
#include "util/Invoice.hpp"

#include <ods/ods>

void
CreateCurrency()
{
	auto *book = ods::Book::New();
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell = row->NewCellAt(0);
	
	// currency value:
	cell->SetCurrency(49.2, ods::currency::str::USD);
	// using ods::currency::str::USD for "USD" to avoid typos.
	
	// other currency info and formatting rules:
	ods::inst::StyleStyle *style = cell->GetStyle();
	
	if (style == nullptr)
		style = cell->NewStyle();
	
	auto *ncs = (ods::inst::NumberCurrencyStyle*)
		style->Get(ods::Id::NumberCurrencyStyle);
	
	if (ncs == nullptr)
		ncs = style->NewCurrencyStyle();
	
	auto *cs = (ods::inst::NumberCurrencySymbol*)
		ncs->Get(ods::Id::NumberCurrencySymbol);
	
	if (cs == nullptr)
		cs = ncs->NewCurrencySymbol();
	
	cs->language("en");
	cs->country(ods::country::USA);
	cs->SetSymbol(ods::currency::symbol::USD);
	
	auto *nn = (ods::inst::NumberNumber*) ncs->Get(ods::Id::NumberNumber);
	
	if (nn == nullptr)
		nn = ncs->NewNumber();
	
	{
		nn->grouping(1);
		nn->min_integer_digits(5);
		nn->decimal_places(4);
		nn->min_decimal_places(3);
		// This makes 49.2 be displayed as "$00,049.200"
		// If nn->grouping(0) then "$00049.200" will be displayed.
		// -1 means value not set.
		// Note: Calligra Sheets doesn't display currency formatting properly
	}
	
	util::Save(book);
}

void
ReadCurrency()
{
	auto full_path = util::FindFile("Currency.ods");
	
	if (full_path.isEmpty())
		return;
	
	auto *book = ods::Book::FromFile(full_path);
	util::AutoDelete<ods::Book*> ad(book);
	auto err = book->error_msg();
	
	if (!err.isEmpty())
	{
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(2);
	
	if (row == nullptr)
	{
		mtl_warn();
		return;
	}
	
	auto *cell = row->GetCell(3);
	
	if (cell == nullptr)
	{
		mtl_warn();
		return;
	}
	
	if (!cell->is_currency())
	{
		mtl_warn("Not currency");
		return;
	}
	
	mtl_line("currency value: %f", *cell->as_currency());
	
	auto *style = cell->GetStyle();
	
	if (style == nullptr)
	{
		mtl_warn("no style");
		return;
	}
	
	auto *ncs = style->GetCurrencyStyle();
	
	if (ncs == nullptr)
	{
		mtl_warn("no ncs");
		return;
	}
	
	auto *cs = (ods::inst::NumberCurrencySymbol*)
		ncs->Get(ods::Id::NumberCurrencySymbol);
	
	if (cs != nullptr)
	{
		auto lang = cs->language().toLocal8Bit();
		mtl_line("language: %s", lang.data());
		
		auto country = cs->country().toLocal8Bit();
		mtl_line("country: %s", country.data());
		
		auto symbol = cs->GetSymbol().toLocal8Bit();
		mtl_line("symbol: %s", symbol.data());
	} else {
		mtl_warn("no currency symbol");
	}
	
	auto *nn = (ods::inst::NumberNumber*) ncs->Get(ods::Id::NumberNumber);
	
	if (nn != nullptr)
	{
		mtl_line("grouping: %d, min_integer_digits: %d, \
decimal_places: %d, min_decimal_places: %d",
		nn->grouping(), nn->min_integer_digits(),
		nn->decimal_places(), nn->min_decimal_places());
	} else {
		mtl_line("no nn");
	}
	
	util::Save(book);
}

void
CreateFormula()
{
	auto *book = ods::Book::New();
	util::AutoDelete<ods::Book*> ad(book);
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->NewSheet("Sheet name");
	auto *row = sheet->NewRowAt(0);
	auto *cell0 = row->NewCellAt(0);
	cell0->SetDouble(5.0);
	
	auto *cell1 = row->NewCellAt(1);
	cell1->SetDouble(4.0);
	
	auto *cell2 = row->NewCellAt(2);
	ods::Formula *formula = cell2->NewFormula();
	formula->Add(ods::Grouping::Open);
	formula->Add(ods::Grouping::Open);
	formula->Add(3.0);
	formula->Add(ods::Op::Subtract);
	formula->Add(2.0);
	formula->Add(ods::Grouping::Close);
	formula->Add(ods::Grouping::Close);
	formula->Add(ods::Op::Multiply);
	formula->Add(1.5);
	
	ods::formula::Value result;
	formula->Eval(result);
	
	util::Save(book);
}

void
ReadFormula()
{
	auto full_path = util::FindFile("Formula.ods");
	
	if (full_path.isEmpty())
		return;
	
	auto *book = ods::Book::FromFile(full_path);
	util::AutoDelete<ods::Book*> ad(book);
	auto err = book->error_msg();
	
	if (!err.isEmpty())
	{
		auto ba = err.toLocal8Bit();
		mtl_warn("%s", ba.data());
		return;
	}
	
	auto *spreadsheet = book->spreadsheet();
	auto *sheet = spreadsheet->GetSheet(0);
	auto *row = sheet->GetRow(0);
	
	if (row == nullptr)
	{
		mtl_warn();
		return;
	}
	
	auto *cell = row->GetCell(2);
	
	if (cell == nullptr)
	{
		mtl_warn("No such cell");
		return;
	}
	
	if (!cell->has_formula())
	{
		mtl_warn("Cell has no formula");
		return;
	}
	
	ods::Formula *formula = cell->formula();
	ods::formula::Value result;
	formula->Eval(result);
	
	if (result.has_error())
	{
		mtl_warn("Got an error when evaluating formula");
		return;
	}
	
	if (result.is_double())
	{
		double num = *result.as_double();
		mtl_line("Formula value: %.2f", num);
	} else {
		mtl_warn("Other types not supported yet");
	}
}

void
CreateInvoice()
{
	util::Invoice invoice;
}

