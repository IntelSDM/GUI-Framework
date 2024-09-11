#include "pch.h"
#include "DropDownTextBox.h"
#include "Input.h"
#include "Drawing.h"
#include "Font.h"
#include "Graphics.h"
#include "Animation.h"

DropDownTextBox::DropDownTextBox(float x, float y, std::wstring text, int* items, std::list<std::wstring>names)
{
	DropDownTextBox::Pos = { x, y };
	DropDownTextBox::Size = { 120, 20 };
	DropDownTextBox::Name = text;
	DropDownTextBox::CalculateBuffer();
	DropDownTextBox::Names = names;
	DropDownTextBox::Index = items;
	DropDownTextBox::ConvertSelectedName();
	DropDownTextBox::VisiblePointerEnd = MainString.length();
	DropDownTextBox::SetStartIndex(); // this sets start value
	DropDownTextBox::VisibleString = MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd);
	DropDownTextBox::SelectedPoint = VisiblePointerEnd - DropDownTextBox::VisiblePointerStart;
	DropDownTextBox::SelectedPosition = GetTextSize(DropDownTextBox::MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::SelectedPoint), "Verdana").x;
	DropDownTextBox::ContextSize = { 80.0f, 20.0f * (int)DropDownTextBox::ContextNames.size() };
	DropDownTextBox::SetVisible(true);

}

void DropDownTextBox::CalculateBuffer()
{
	if (!DropDownTextBox::Active)
		DropDownTextBox::CutOffBuffer = 15;
	else
		DropDownTextBox::CutOffBuffer = 0;
}
void DropDownTextBox::ConvertSelectedName()
{
	auto it = DropDownTextBox::Names.begin();
	std::advance(it, *Index);
	float originalwidth = GetTextSize(*it, "Verdana", 11).x;

	if (originalwidth < DropDownTextBox::Size.x - DropDownTextBox::CutOffBuffer)
	{
		DropDownTextBox::MainString = *it;
		DropDownTextBox::VisiblePointerStart = 0;
		DropDownTextBox::VisiblePointerEnd = MainString.length();
		TextWidth = originalwidth;
		return;
	}
	else
	{
		std::wstring str = *it;
		for (int i = str.length(); i > 0; i--)
		{
			str.erase(std::prev((str).end()));
			float width = GetTextSize(str + L"..", "Verdana", 11).x;
			if (width < DropDownTextBox::Size.x - DropDownTextBox::CutOffBuffer)
			{
				DropDownTextBox::MainString = str + L"..";
				TextWidth = width;
				return;
			}
		}
		DropDownTextBox::MainString = str + L"..";
		TextWidth = GetTextSize(str + L"..", "Verdana", 11).x;
		DropDownTextBox::VisiblePointerStart = 0;
		DropDownTextBox::VisiblePointerEnd = MainString.length();

	}
}
void DropDownTextBox::SetDropDownWidth()
{
	float width = 0;
	for (std::wstring str : DropDownTextBox::Names)
	{
		float wdth = GetTextSize(str, "Verdana", 11).x;
		if (wdth > width)
			width = wdth;
	}
	DropDownTextBox::DropWidth = width;
}
void DropDownTextBox::SetStartIndex()
{
	// Sets the value to be the right most character at the end.
	DropDownTextBox::VisiblePointerStart = 0;
	DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana").x;
	while (DropDownTextBox::TextWidth > DropDownTextBox::Size.x - 6)
	{
		DropDownTextBox::VisiblePointerStart++; // update position
		DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana").x; // update width so we can exit
	}
}

void DropDownTextBox::SetState()
{
	WPARAM character = Char;
	if (IsMouseInRectangle(DropDownTextBox::Pos + DropDownTextBox::ParentPos, DropDownTextBox::Size) && IsKeyClicked(VK_LBUTTON) && !DropDownTextBox::Blocked && ((DropDownTextBox::ContextActive && !IsMouseInRectangle(DropDownTextBox::ContextPos, DropDownTextBox::ContextSize)) || !DropDownTextBox::ContextActive))
	{
		DropDownTextBox::Active = true;
		DropDownTextBox::ContextActive = false;
		Char = NULL;
	}
	else if (IsKeyClicked(VK_LBUTTON) && !IsMouseInRectangle(DropDownTextBox::Pos + DropDownTextBox::ParentPos, DropDownTextBox::Size) && DropDownTextBox::Active)
	{
		DropDownTextBox::Selecting = false;
		DropDownTextBox::Held = false;
		DropDownTextBox::Active = false; // prevent 2 being active at the same time unless they are somehow fucking merged
		DropDownTextBox::ValueChangeEvent();
	}
	else if ((character == VK_RETURN || character == VK_ESCAPE) && DropDownTextBox::Active)
	{
		DropDownTextBox::Active = false;
		DropDownTextBox::ValueChangeEvent();
		DropDownTextBox::Selecting = false;
		Char = NULL;
	}

	if (!IsKeyDown(VK_LBUTTON))
		DropDownTextBox::Held = false;
	if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionStart && DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
		DropDownTextBox::Selecting = false;
}

bool DropDownTextBox::IsKeyAcceptable()
{
	WPARAM character = Char;
	if (character == NULL)
		return false;
	if (character == VK_BACK)
	{
		DeleteText();
		return false;
	}
	if (character == VK_RETURN)
		return false;
	if (IsKeyDown(VK_CONTROL))
		return false;
	if (character == VK_TAB)
		return false;
	return true;
}

void DropDownTextBox::ArrowKeyNavition()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (IsKeyClicked(VK_LEFT) && DropDownTextBox::LastClick < (clock() * 0.00001f))
	{
		DropDownTextBox::Selecting = false;
		DropDownTextBox::Held = false;
		// pointer isn't behind visible text
		if (SelectedPoint > DropDownTextBox::VisiblePointerStart)
		{
			DropDownTextBox::SelectedPoint--;
		}
		// pointer is going behind currently visible text
		else if (DropDownTextBox::VisiblePointerStart != 0 && SelectedPoint == DropDownTextBox::VisiblePointerStart)
		{
			DropDownTextBox::SelectedPoint--;
			DropDownTextBox::VisiblePointerStart--;
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			// if the value exceeds the textbox bounds decrement the ending
			while (DropDownTextBox::TextWidth > DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerStart != 0)
			{
				DropDownTextBox::VisiblePointerEnd--;
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::MainString.length() > DropDownTextBox::VisiblePointerEnd && DropDownTextBox::VisiblePointerStart == 0)
			{
				DropDownTextBox::VisiblePointerEnd++; // update position
				DropDownTextBox::SelectedPoint++;
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		Char = NULL;
		DropDownTextBox::LastClick = (clock() * 0.00001f) + 0.002f;
	}
	if (IsKeyClicked(VK_RIGHT) && DropDownTextBox::LastClick < (clock() * 0.00001f))
	{
		DropDownTextBox::Selecting = false;
		DropDownTextBox::Held = false;
		if (DropDownTextBox::SelectedPoint < DropDownTextBox::VisiblePointerEnd)
			DropDownTextBox::SelectedPoint++;

		else if (DropDownTextBox::VisiblePointerEnd != DropDownTextBox::MainString.length() && DropDownTextBox::SelectedPoint == DropDownTextBox::VisiblePointerEnd)
		{
			DropDownTextBox::SelectedPoint++;
			DropDownTextBox::VisiblePointerEnd++;
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			// decrement start
			while (DropDownTextBox::TextWidth > DropDownTextBox::Size.x - 6)
			{
				DropDownTextBox::VisiblePointerStart++; // update position
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		Char = NULL;
		DropDownTextBox::LastClick = (clock() * 0.00001f) + 0.002f;
	}
}

void DropDownTextBox::InputText()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (DropDownTextBox::IsKeyAcceptable() && std::iswprint(Char))
	{
		if (DropDownTextBox::Selecting) // delete selected text
		{

			if (DropDownTextBox::SelectionStart != DropDownTextBox::SelectedPoint || DropDownTextBox::SelectionEnd != DropDownTextBox::SelectedPoint)
			{
				if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
				{
					DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
					DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
					DropDownTextBox::SelectedPoint -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
				}
				else
				{
					DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
					DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
				}
			}

			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerStart > 0)
			{
				DropDownTextBox::VisiblePointerStart--;
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerEnd < DropDownTextBox::MainString.length())
			{
				DropDownTextBox::VisiblePointerEnd++;
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			DropDownTextBox::SelectionStart = DropDownTextBox::SelectedPoint;
			DropDownTextBox::SelectionEnd = DropDownTextBox::SelectedPoint;
			DropDownTextBox::Held = false;
			DropDownTextBox::Selecting = false;
		}

		Selecting = false;
		DropDownTextBox::VisiblePointerEnd++;
		DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
		MainString.insert(DropDownTextBox::SelectedPoint, 1, Char);
		DropDownTextBox::SelectedPoint++;
		while (DropDownTextBox::TextWidth > DropDownTextBox::Size.x - 6)
		{
			DropDownTextBox::VisiblePointerStart++; // update position
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
		}

	}
	Char = NULL;
}

void DropDownTextBox::DeleteText()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	WPARAM character = Char;
	if (character == VK_BACK && (DropDownTextBox::MainString).length() != 0 && DropDownTextBox::VisiblePointerEnd != 0 && SelectedPoint != 0) // backspace
	{
		// no selection
		if (DropDownTextBox::SelectionStart == DropDownTextBox::SelectedPoint && DropDownTextBox::SelectionEnd == DropDownTextBox::SelectedPoint && !DropDownTextBox::Selecting)
		{
			if (DropDownTextBox::SelectedPoint == DropDownTextBox::VisiblePointerEnd)
			{
				(DropDownTextBox::MainString).erase(std::prev((DropDownTextBox::MainString).end()));
				DropDownTextBox::VisiblePointerEnd--;
				DropDownTextBox::SelectedPoint--;
			}
			else
			{
				DropDownTextBox::MainString.erase(DropDownTextBox::SelectedPoint - 1, 1);
				DropDownTextBox::SelectedPoint--;
				DropDownTextBox::VisiblePointerEnd--;
			}

			if (DropDownTextBox::VisiblePointerStart != 0 && GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x < DropDownTextBox::Size.x - 6)
			{
				DropDownTextBox::VisiblePointerStart--;
			}
			// detect if there is any other text that we might need to add so our string doesn't randomly get cut off
			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::MainString.length() > DropDownTextBox::VisiblePointerEnd)
			{
				DropDownTextBox::VisiblePointerEnd++; // update position
				DropDownTextBox::SelectedPoint++;
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart + 1, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		else
		{
			//selecting
			// only change selectedpoint if its going to be removed as well
			if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
			{
				DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
				DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
				DropDownTextBox::SelectedPoint -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
			}
			else
			{
				DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
				DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
			}
			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerStart > 0)
			{
				DropDownTextBox::VisiblePointerStart--; // Move the starting point up
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			// If the text still doesn't fill the TextBox, try to extend from the end
			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerEnd < DropDownTextBox::MainString.length())
			{
				DropDownTextBox::VisiblePointerEnd++; // Extend the ending point
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			//reset selected points
			DropDownTextBox::SelectionStart = DropDownTextBox::SelectedPoint;
			DropDownTextBox::SelectionEnd = DropDownTextBox::SelectedPoint;
			DropDownTextBox::Held = false;
			DropDownTextBox::Selecting = false;
		}
		Char = NULL;
	}
}

/*void DropDownTextBox::ClearText()
{
	Char = NULL;
}*/

void DropDownTextBox::SetSelection()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (DropDownTextBox::Held)
	{
		DropDownTextBox::Selecting = true;
		Vector2 relativemousepos = { MousePos.x - (DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x), MousePos.y - (DropDownTextBox::Pos.y + DropDownTextBox::ParentPos.y) };
		float lastdistance = 99999; // the user shouldn't ever have a resolution/position over this value
		int instance = 0;
		for (int i = DropDownTextBox::VisiblePointerStart; i <= DropDownTextBox::VisiblePointerEnd; i++)
		{
			float width = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, i - DropDownTextBox::VisiblePointerStart), "Verdana", 11).x;
			float distance = std::abs(relativemousepos.x - width);
			if (distance > lastdistance)
			{
				instance = --i;
				break;
			}
			lastdistance = distance;
			instance = i;
		}
		if (instance == -1)
			instance = DropDownTextBox::DropDownTextBox::VisiblePointerEnd;
		if (DropDownTextBox::SelectedPoint > instance)
		{
			DropDownTextBox::SelectionEnd = DropDownTextBox::SelectedPoint;
			DropDownTextBox::SelectionStart = instance;
		}
		else
		{
			DropDownTextBox::SelectionEnd = instance;
			DropDownTextBox::SelectionStart = DropDownTextBox::SelectedPoint;
		}
	}

	if (IsKeyDown(VK_CONTROL) && IsKeyDown(0x41)) //(A)
	{
		DropDownTextBox::SelectionStart = 0;
		DropDownTextBox::SelectionEnd = MainString.length();
	}
}

void DropDownTextBox::ContextSelectAll()
{
	DropDownTextBox::SelectionStart = 0;
	DropDownTextBox::SelectionEnd = MainString.length();
}

void DropDownTextBox::ContextCopyText()
{
	if (!OpenClipboard(nullptr))
		return;
	size_t size = (SelectionEnd - SelectionStart) * sizeof(wchar_t) + sizeof(wchar_t);

	HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE, size);
	if (!global)
	{
		CloseClipboard();
		return;
	}

	wchar_t* text = static_cast<wchar_t*>(GlobalLock(global));
	if (!text)
	{
		CloseClipboard();
		GlobalFree(global);
		return;
	}

	wcsncpy_s(text, size / sizeof(wchar_t), MainString.substr(SelectionStart, SelectionEnd - SelectionStart).c_str(), SelectionEnd - SelectionStart);

	text[SelectionEnd - SelectionStart] = L'\0';
	GlobalUnlock(global);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, global);
	CloseClipboard();
}

void DropDownTextBox::ContextPasteText()
{
	if (!OpenClipboard(nullptr))
		return;
	std::wstring clipboard = L"";
	HANDLE data = GetClipboardData(CF_UNICODETEXT);
	if (data != nullptr)
	{
		wchar_t* text = static_cast<wchar_t*>(GlobalLock(data));
		if (text != nullptr)
		{
			clipboard = text;
			GlobalUnlock(data);
		}
	}

	CloseClipboard();

	if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionStart && DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
	{
		DropDownTextBox::VisiblePointerEnd += clipboard.length();
		MainString.insert(DropDownTextBox::SelectedPoint, clipboard);
		DropDownTextBox::SelectedPoint += clipboard.length();
		DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
		while (DropDownTextBox::TextWidth > DropDownTextBox::Size.x - 6)
		{
			DropDownTextBox::VisiblePointerStart++; // update position
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
		}
	}
	else
	{
		if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
		{
			DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
			DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
			DropDownTextBox::SelectedPoint -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
		}
		else
		{
			DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
			DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
		}
		DropDownTextBox::VisiblePointerEnd += clipboard.length();
		MainString.insert(DropDownTextBox::SelectedPoint, clipboard);
		DropDownTextBox::SelectedPoint += clipboard.length();
		DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
		while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerStart > 0)
		{
			DropDownTextBox::VisiblePointerStart--; // Move the starting point up
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
		}

		// If the text still doesn't fill the TextBox, try to extend from the end
		while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerEnd < DropDownTextBox::MainString.length())
		{
			DropDownTextBox::VisiblePointerEnd++; // Extend the ending point
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
		}

		//reset selected points
		DropDownTextBox::SelectionStart = DropDownTextBox::SelectedPoint;
		DropDownTextBox::SelectionEnd = DropDownTextBox::SelectedPoint;
	}
}

void DropDownTextBox::SetSelectionPoint()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (IsMouseInRectangle(DropDownTextBox::Pos + DropDownTextBox::ParentPos, DropDownTextBox::Size) && IsKeyClicked(VK_LBUTTON))
	{
		DropDownTextBox::Held = true;
		Vector2 relativemousepos = { MousePos.x - (DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x), MousePos.y - (DropDownTextBox::Pos.y + DropDownTextBox::ParentPos.y) };
		// get width
		// get last position
		// is last position closer or this one?
		// if last position is closer return last position.
		float lastdistance = 999999;
		int instance = 0;
		for (int i = DropDownTextBox::VisiblePointerStart; i <= DropDownTextBox::VisiblePointerEnd; i++)
		{
			float width = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, i - DropDownTextBox::VisiblePointerStart), "Verdana", 11).x;
			float distance = std::abs(relativemousepos.x - width);
			if (distance > lastdistance)
			{
				instance = --i;
				break;
			}
			lastdistance = distance;
			instance = i;
		}
		if (instance == -1)
			instance = DropDownTextBox::VisiblePointerEnd;
		DropDownTextBox::SelectedPoint = instance;
	}
}

void DropDownTextBox::SelectionDragging()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (DropDownTextBox::Held)
	{
		const int delay = 100; // Set the desired delay in milliseconds
		auto time = std::chrono::high_resolution_clock::now();
		// push text forwards
		while (DropDownTextBox::VisiblePointerEnd != MainString.length() && DropDownTextBox::SelectionEnd == DropDownTextBox::VisiblePointerEnd)
		{
			auto curtime = std::chrono::high_resolution_clock::now();
			auto elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - time).count();
			if (elapsedtime >= delay)
			{
				DropDownTextBox::SelectedPoint++;
				DropDownTextBox::VisiblePointerEnd++;
				DropDownTextBox::VisiblePointerStart++;
			}
		}

		while (DropDownTextBox::VisiblePointerStart != 0 && DropDownTextBox::SelectionStart == DropDownTextBox::VisiblePointerStart)
		{
			auto curtime = std::chrono::high_resolution_clock::now();
			auto elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - time).count();
			if (elapsedtime >= delay)
			{
				DropDownTextBox::SelectedPoint--;
				DropDownTextBox::VisiblePointerEnd--;
				DropDownTextBox::VisiblePointerStart--;
				time = std::chrono::high_resolution_clock::now();
			}
		}
	}
}

void DropDownTextBox::CopyText()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionStart && DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
		return;
	if (!(IsKeyDown(VK_CONTROL) && IsKeyDown(0x43)))
		return;
	if (!OpenClipboard(nullptr))
		return;
	size_t size = (SelectionEnd - SelectionStart) * sizeof(wchar_t) + sizeof(wchar_t);

	HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE, size);
	if (!global)
	{
		CloseClipboard();
		return;
	}

	wchar_t* text = static_cast<wchar_t*>(GlobalLock(global));
	if (!text)
	{
		CloseClipboard();
		GlobalFree(global);
		return;
	}

	wcsncpy_s(text, size / sizeof(wchar_t), MainString.substr(SelectionStart, SelectionEnd - SelectionStart).c_str(), SelectionEnd - SelectionStart);

	text[SelectionEnd - SelectionStart] = L'\0';
	GlobalUnlock(global);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, global);
	CloseClipboard();
}

void DropDownTextBox::PasteText()
{
	if (DropDownTextBox::Blocked)
		return;
	if (!DropDownTextBox::Active)
		return;
	if (!(IsKeyDown(VK_CONTROL) && IsKeyDown(0x56)))
		return;
	if (!OpenClipboard(nullptr))
		return;
	std::wstring clipboard = L"";
	HANDLE data = GetClipboardData(CF_UNICODETEXT);
	if (data != nullptr)
	{
		wchar_t* text = static_cast<wchar_t*>(GlobalLock(data));
		if (text != nullptr)
		{
			clipboard = text;
			GlobalUnlock(data);
		}
	}

	CloseClipboard();
	if (DropDownTextBox::LastClick < (clock() * 0.00001f))
	{
		if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionStart && DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
		{
			DropDownTextBox::VisiblePointerEnd += clipboard.length();
			MainString.insert(DropDownTextBox::SelectedPoint, clipboard);
			DropDownTextBox::SelectedPoint += clipboard.length();
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			while (DropDownTextBox::TextWidth > DropDownTextBox::Size.x - 6)
			{
				DropDownTextBox::VisiblePointerStart++; // update position
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}
		}
		else
		{
			if (DropDownTextBox::SelectedPoint == DropDownTextBox::SelectionEnd)
			{
				DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
				DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
				DropDownTextBox::SelectedPoint -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
			}
			else
			{
				DropDownTextBox::MainString.erase(DropDownTextBox::SelectionStart, DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart);
				DropDownTextBox::VisiblePointerEnd -= DropDownTextBox::SelectionEnd - DropDownTextBox::SelectionStart;
			}
			DropDownTextBox::VisiblePointerEnd += clipboard.length();
			MainString.insert(DropDownTextBox::SelectedPoint, clipboard);
			DropDownTextBox::SelectedPoint += clipboard.length();
			DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerStart > 0)
			{
				DropDownTextBox::VisiblePointerStart--; // Move the starting point up
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			// If the text still doesn't fill the TextBox, try to extend from the end
			while (DropDownTextBox::TextWidth < DropDownTextBox::Size.x - 6 && DropDownTextBox::VisiblePointerEnd < DropDownTextBox::MainString.length())
			{
				DropDownTextBox::VisiblePointerEnd++; // Extend the ending point
				DropDownTextBox::TextWidth = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd), "Verdana", 11).x;
			}

			//reset selected points
			DropDownTextBox::SelectionStart = DropDownTextBox::SelectedPoint;
			DropDownTextBox::SelectionEnd = DropDownTextBox::SelectedPoint;
		}
		DropDownTextBox::LastClick = (clock() * 0.00001f) + 0.002f;
	}
}

void DropDownTextBox::ContextMenu()
{
	if (IsMouseInRectangle(DropDownTextBox::Pos + DropDownTextBox::ParentPos, DropDownTextBox::Size) && IsKeyClicked(VK_RBUTTON) && !DropDownTextBox::Blocked)
	{
		DropDownTextBox::ContextActive = true;
		DropDownTextBox::ContextPos = MousePos;
		DropDownTextBox::Selecting = false;
		DropDownTextBox::Held = false;
		DropDownTextBox::Active = false; // prevent 2 being active at the same time unless they are somehow fucking merged
		DropDownTextBox::LastClick = (clock() * 0.00001f) + 0.002f;
		DropDownTextBox::ValueChangeEvent();
		DropDownTextBox::SetBlockedSiblings(true);
		Sleep(50);
	}
	else if (IsKeyClicked(VK_LBUTTON) && !IsMouseInRectangle(DropDownTextBox::Pos + DropDownTextBox::ParentPos, DropDownTextBox::Size) && DropDownTextBox::ContextActive)
	{
		DropDownTextBox::ContextActive = false;
		DropDownTextBox::SetBlockedSiblings(false);
		Sleep(50);
	}
	if (!(IsMouseInRectangle(DropDownTextBox::Pos + DropDownTextBox::ParentPos, DropDownTextBox::Size) || IsMouseInRectangle(DropDownTextBox::ContextPos, DropDownTextBox::ContextSize)) && IsKeyClicked(VK_RBUTTON) && !DropDownTextBox::Blocked && DropDownTextBox::ContextActive)
	{
		DropDownTextBox::SetBlockedSiblings(false);
		DropDownTextBox::ContextActive = false;
		Sleep(50);
	}
	if (!DropDownTextBox::ContextActive)
		return;
	int i = 0;
	for (auto& pair : DropDownTextBox::ContextNames)
	{
		if (IsMouseInRectangle(DropDownTextBox::ContextPos.x, DropDownTextBox::ContextPos.y + (i * 20), DropDownTextBox::ContextSize.x, 20) && IsKeyClicked(VK_LBUTTON) && DropDownTextBox::LastClick < (clock() * 0.00001f))
		{
			pair.second();
			DropDownTextBox::LastClick = (clock() * 0.00001f) + 0.002f;
			DropDownTextBox::SetBlockedSiblings(false);
		}
		i++;
	}
}

void DropDownTextBox::Update()
{
	if (!DropDownTextBox::Parent)
		DropDownTextBox::SetVisible(false);
	DropDownTextBox::ParentPos = DropDownTextBox::Parent->GetParentPos();
	if (!DropDownTextBox::IsVisible())
		return;

	DropDownTextBox::TextWidth = GetTextSize(VisibleString, "Verdana", 11).x;
	DropDownTextBox::CalculateBuffer();
	DropDownTextBox::VisibleString = MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::VisiblePointerEnd);
	DropDownTextBox::SetState();
	DropDownTextBox::ArrowKeyNavition();
	DropDownTextBox::InputText();
	//	DropDownTextBox::DeleteText();
		//DropDownTextBox::ClearText();
	DropDownTextBox::SetSelectionPoint();
	DropDownTextBox::SetSelection();
	DropDownTextBox::SelectionDragging();
	DropDownTextBox::CopyText();
	DropDownTextBox::PasteText();
	DropDownTextBox::ContextMenu();
	if (DropDownTextBox::DropWidth < DropDownTextBox::Size.x)
	{
		DropDownTextBox::DropWidth = DropDownTextBox::Size.x;
		DropDownTextBox::SizeDifference = 0;
	}
	if (DropDownTextBox::Active) // take input
	{
		DropDownTextBox::SizeDifference = DropDownTextBox::DropWidth - DropDownTextBox::TextWidth;
		DropDownTextBox::SetDropDownWidth();

		if (!DropDownTextBox::Held && !DropDownTextBox::Selecting)
		{
			DropDownTextBox::SelectionStart = DropDownTextBox::SelectedPoint;
			DropDownTextBox::SelectionEnd = DropDownTextBox::SelectedPoint;
		}

		// Update the selected point if it is out of bounds
		if (DropDownTextBox::SelectedPoint > DropDownTextBox::VisiblePointerEnd)
		{
			DropDownTextBox::SelectedPoint = DropDownTextBox::VisiblePointerEnd;
		}

		DropDownTextBox::SelectedPosition = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::SelectedPoint - DropDownTextBox::VisiblePointerStart), "Verdana", 11).x;
		DropDownTextBox::SelectingStartPosition = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::SelectionStart - DropDownTextBox::VisiblePointerStart), "Verdana", 11).x;
		DropDownTextBox::SelectingEndPosition = GetTextSize(MainString.substr(DropDownTextBox::VisiblePointerStart, DropDownTextBox::SelectionEnd - DropDownTextBox::VisiblePointerStart), "Verdana", 11).x;

	}
		
}

void DropDownTextBox::Draw()
{
	if (!DropDownTextBox::Parent)
		DropDownTextBox::SetVisible(false);
	if (!DropDownTextBox::IsVisible())
		return;

	MyColour rectColour = MenuColours["TextBox"];
	MyColour rectOutlineColour = MenuColours["TextBoxOutline"];
	MyColour textColour = MenuColours["Text"];
	MyColour highlightColour = MenuColours["TextBoxHighlight"];
	MyColour currentLocColour = MenuColours["TextBoxCurrent"];
	MyColour contextOutlineColour = MenuColours["TextBoxContextOutline"];
	MyColour contextLineColour = MenuColours["TextBoxContextFirstLine"];

	MyColour selectedTextColour = MenuColours["DropDownSelectedText"];
	MyColour arrowColour = MenuColours["DropDownArrow"];
	MyColour activeArrowColour = MenuColours["DropDownActiveArrow"];
	MyColour sliderColour = MenuColours["DropDownSlider"];


	FilledRectangle(DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x - 1, DropDownTextBox::Pos.y + +DropDownTextBox::ParentPos.y - 1, DropDownTextBox::Size.x + 2, DropDownTextBox::Size.y + 2, rectOutlineColour);
	FilledRectangle(DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x, DropDownTextBox::Pos.y + +DropDownTextBox::ParentPos.y, DropDownTextBox::Size.x, DropDownTextBox::Size.y, rectColour);
	float trianglex1 = DropDownTextBox::ParentPos.x + DropDownTextBox::Pos.x + DropDownTextBox::Size.x - 12;
	float triangley1 = DropDownTextBox::ParentPos.y + DropDownTextBox::Pos.y + 3;
	float trianglex2 = DropDownTextBox::ParentPos.x + DropDownTextBox::Pos.x + DropDownTextBox::Size.x - 3;
	float triangley2 = DropDownTextBox::Pos.y + DropDownTextBox::ParentPos.y + 3;
	float trianglex3 = DropDownTextBox::ParentPos.x + DropDownTextBox::Pos.x + DropDownTextBox::Size.x - 7;
	float triangley3 = DropDownTextBox::ParentPos.y + DropDownTextBox::Pos.y + DropDownTextBox::Size.y - 3;

	if (!DropDownTextBox::Active)
		FilledTriangle(trianglex1, triangley1, trianglex2, triangley2, trianglex3, triangley3, arrowColour);


	DrawText(DropDownTextBox::ParentPos.x + DropDownTextBox::Pos.x, DropDownTextBox::ParentPos.y + DropDownTextBox::Pos.y - (DropDownTextBox::Size.y / 1.5) - 1, DropDownTextBox::Name + L":", "Verdana", 11, textColour, None); // Title
	DrawText(DropDownTextBox::ParentPos.x + DropDownTextBox::Pos.x + 3, (DropDownTextBox::ParentPos.y + DropDownTextBox::Pos.y) + (DropDownTextBox::Size.y / 6), DropDownTextBox::VisibleString, "Verdana", 11, textColour, None);

	std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - DropDownTextBox::AnimationStart;
	float time = std::fmodf(elapsed.count(), DropDownTextBox::AnimationInterval) / DropDownTextBox::AnimationInterval;
	float easedtime = InOutSine(time);
	if (DropDownTextBox::Active && std::fmod(elapsed.count(), DropDownTextBox::AnimationInterval) < DropDownTextBox::AnimationInterval / 2)
	{
		float alpha = 255.0f * (1.0f - easedtime * 2.0f);
		FilledLine(DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x + DropDownTextBox::SelectedPosition + 5.f, DropDownTextBox::Pos.y + DropDownTextBox::ParentPos.y + DropDownTextBox::Size.y - 3, DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x + DropDownTextBox::SelectedPosition + 5.f, DropDownTextBox::Pos.y + DropDownTextBox::ParentPos.y + 3, 1,
			currentLocColour.Modify(currentLocColour.r, currentLocColour.g, currentLocColour.b, static_cast<float>(alpha) / 255.0f));
	}
	if (DropDownTextBox::SelectingStartPosition >= 0 || DropDownTextBox::SelectingEndPosition >= 0)
	{
		float selectionWidth = std::abs(DropDownTextBox::SelectingEndPosition - DropDownTextBox::SelectingStartPosition);
		float startX = DropDownTextBox::Pos.x + DropDownTextBox::ParentPos.x + DropDownTextBox::SelectingStartPosition;
		// Check the direction of selection
		if (DropDownTextBox::SelectingEndPosition > DropDownTextBox::SelectingStartPosition)
			startX += 5.f;
		else
			startX -= 5.f;
		FilledRectangle(startX, DropDownTextBox::Pos.y + DropDownTextBox::ParentPos.y, selectionWidth, DropDownTextBox::Size.y, highlightColour);
	}
	if (DropDownTextBox::ContextActive)
	{
		OutlineRectangle(DropDownTextBox::ContextPos.x, DropDownTextBox::ContextPos.y, DropDownTextBox::ContextSize.x, DropDownTextBox::ContextSize.y, 1, textColour);
		FilledRectangle(DropDownTextBox::ContextPos.x, DropDownTextBox::ContextPos.y, DropDownTextBox::ContextSize.x, DropDownTextBox::ContextSize.y, rectColour);
		int i = 0;
		for (auto pair : DropDownTextBox::ContextNames)
		{
			if (i != 0)
				FilledLine(DropDownTextBox::ContextPos.x, DropDownTextBox::ContextPos.y + i * 20, DropDownTextBox::ContextPos.x + DropDownTextBox::ContextSize.x, DropDownTextBox::ContextPos.y + i * 20, 1.0f, contextLineColour);

			if (IsMouseInRectangle(DropDownTextBox::ContextPos.x, DropDownTextBox::ContextPos.y + (i * 20), DropDownTextBox::ContextSize.x, 20))
				FilledRectangle(DropDownTextBox::ContextPos.x, DropDownTextBox::ContextPos.y + (i * 20), DropDownTextBox::ContextSize.x, 20, contextOutlineColour);
			DrawText(DropDownTextBox::ContextPos.x + (DropDownTextBox::ContextSize.x / 2), DropDownTextBox::ContextPos.y + (i * 20) + 10, pair.first, "Verdana", 11, textColour, CentreCentre);

			i++;
		}
	}
	if (!DropDownTextBox::ContextActive && !DropDownTextBox::Active)
		DrawTooltip();
}
