#pragma once
#include "Entity.h"
#include "Drawing.h"
#include "Input.h"
#include "Animation.h"

// due to using templates in c++ classes it is illegal to use a c++ file so this will look rather odd to the other code.
template <typename T>
class NumericSlider : public Entity
{
protected:
	T MinValue;
	T MaxValue;
	T* Value;
	T OutputValue;

	std::wstring Measurement;
	std::wstring OutputString;
	Vector2 MeasurementSize;
	std::wstring VisibleString;
	float TextWidth = 0;
	bool Active = false;

	std::chrono::time_point<std::chrono::high_resolution_clock> AnimationStart;
	float AnimationInterval = 1.0f;

	std::chrono::time_point<std::chrono::high_resolution_clock> SelectingAnimationStart;
	float SelectingAnimationInterval = 0.005f;

	int SelectedPoint;
	float SelectedPosition;

	int SelectionStart;
	int SelectionEnd;
	float SelectingStartPosition = 0;
	float SelectingEndPosition = 0;

	int VisiblePointerStart;
	int VisiblePointerEnd;

	bool Selecting = false;
	bool Held = false;

	void ConvertValueToString()
	{
		OutputString = std::to_wstring(OutputValue);
		size_t dotpos = OutputString.find(L".");
		if (dotpos != std::string::npos)
		{
			if (OutputString.length() > dotpos + 2)
			{
				OutputString.resize(dotpos + 3); // Truncate to two places after the decimal place
			}
		}
	}
	void SetValue()
	{
		if (OutputString.size() == 0)
		{
			*Value = 0;
			return;
		}
		try
		{
			if (typeid(*Value) == typeid(int))
			{
				*Value = std::stoi(OutputString);
			}
			else if (typeid(*Value) == typeid(float))
			{
				*Value = std::stof(OutputString);
			}
			else if (typeid(*Value) == typeid(double))
			{
				*Value = std::stod(OutputString);
			}
		}
		catch (std::exception ex) {}
	}
	bool IsKeyValid(WPARAM c)
	{
		if(c == VK_BACK)
			DeleteText();
		if (typeid(*Value) == typeid(int))
		{
			if (!isdigit(c))
				return false;
			
		}
		else
		{
			if (!isdigit(c) && c != '.')
				return false;
			
		}
		return true;
	}
	bool IsKeyValid(char c)
	{
		if (c == VK_BACK)
			DeleteText();
		if (typeid(*Value) == typeid(int))
		{
			if (!isdigit(c))
				return false;
		}
		else
		{
			if (!isdigit(c) && c != '.')
				return false;
		}
		return true;
	}
	bool IsKeyValid(wchar_t c)
	{
		if (typeid(*Value) == typeid(int))
		{
			if (!isdigit(c))
				return false;
		}
		else
		{
			if (!isdigit(c) && c != L'.')
				return false;
		}
	}
	void SetStartIndex()
	{
		// Sets the value to be the right most character at the end.
		 VisiblePointerStart = 0;
		 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana").x;
		while ( TextWidth > Size.x - (MeasurementSize.x + 6))
		{
			 VisiblePointerStart++; // update position
			 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana").x; // update width so we can exit
		}
	}
	void SetState()
	{
		WPARAM character = Char;
		if (IsMouseInRectangle( Pos +  ParentPos,  Size) && IsKeyClicked(VK_LBUTTON) && ! Blocked && !Active)
		{
			 Active = true;
			Char = NULL;
			OutputString = L"";
			VisiblePointerStart = 0;
			VisiblePointerEnd = 0;
		}
		else if (IsKeyClicked(VK_LBUTTON) && !IsMouseInRectangle( Pos +  ParentPos,  Size) &&  Active)
		{
			 Selecting = false;
			 Held = false;
			 Active = false; // prevent 2 being active at the same time unless they are somehow fucking merged
			 ValueChangeEvent();
		}
		else if ((character == VK_RETURN || character == VK_ESCAPE) &&  Active)
		{
			 Active = false;
			 ValueChangeEvent();
			 Selecting = false;
			Char = NULL;
		}

		if (!IsKeyDown(VK_LBUTTON))
			Held = false;
		if (SelectedPoint == SelectionStart && SelectedPoint == SelectionEnd)
			Selecting = false;
	}
	void  ArrowKeyNavition()
	{
		if ( Blocked)
			return;
		if (! Active)
			return;
		if (IsKeyClicked(VK_LEFT) &&  LastClick < (clock() * 0.00001f))
		{
			 Selecting = false;
			 Held = false;
			// pointer isn't behind visible text
			if (SelectedPoint >  VisiblePointerStart)
			{
				 SelectedPoint--;
			}
			// pointer is going behind currently visible text
			else if ( VisiblePointerStart != 0 && SelectedPoint ==  VisiblePointerStart)
			{
				 SelectedPoint--;
				 VisiblePointerStart--;
				 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x;
				// if the value exceeds the textbox bounds decrement the ending
				while ( TextWidth > Size.x - (MeasurementSize.x + 6) &&  VisiblePointerStart != 0)
				{
					 VisiblePointerEnd--;
					 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
				}
				while ( TextWidth <  Size.x - (MeasurementSize.x + 6) &&  OutputString.length() >  VisiblePointerEnd &&  VisiblePointerStart == 0)
				{
					 VisiblePointerEnd++; // update position
					 SelectedPoint++;
					 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
				}
			}
			Char = NULL;
			 LastClick = (clock() * 0.00001f) + 0.002f;
		}
		if (IsKeyClicked(VK_RIGHT) &&  LastClick < (clock() * 0.00001f))
		{
			 Selecting = false;
			 Held = false;
			if ( SelectedPoint <  VisiblePointerEnd)
				 SelectedPoint++;

			else if ( VisiblePointerEnd !=  OutputString.length() &&  SelectedPoint ==  VisiblePointerEnd)
			{
				 SelectedPoint++;
				 VisiblePointerEnd++;
				 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x;
				// decrement start
				while ( TextWidth > Size.x - (MeasurementSize.x + 6))
				{
					 VisiblePointerStart++; // update position
					 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
				}
			}
			Char = NULL;
			 LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}

	void InputText()
	{
		if ( Blocked)
			return;
		if (! Active)
			return;
		if ( IsKeyValid(Char) && std::isprint(Char) && std::to_wstring(MaxValue).length() > OutputString.length())
		{
			if ( Selecting) // delete selected text
			{

				if ( SelectionStart !=  SelectedPoint ||  SelectionEnd !=  SelectedPoint)
				{
					if ( SelectedPoint ==  SelectionEnd)
					{
						 OutputString.erase( SelectionStart,  SelectionEnd -  SelectionStart);
						 VisiblePointerEnd -=  SelectionEnd -  SelectionStart;
						 SelectedPoint -=  SelectionEnd -  SelectionStart;
					}
					else
					{
						 OutputString.erase( SelectionStart,  SelectionEnd -  SelectionStart);
						 VisiblePointerEnd -=  SelectionEnd -  SelectionStart;
					}
				}

				while ( TextWidth < Size.x - (MeasurementSize.x + 6) &&  VisiblePointerStart > 0)
				{
					 VisiblePointerStart--;
					 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x;
				}

				while ( TextWidth < Size.x - (MeasurementSize.x + 6) &&  VisiblePointerEnd <  OutputString.length())
				{
					 VisiblePointerEnd++;
					 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x;
				}

				 SelectionStart =  SelectedPoint;
				 SelectionEnd =  SelectedPoint;
				 Held = false;
				 Selecting = false;
			}

			Selecting = false;
			 VisiblePointerEnd++;
			 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x;
			OutputString.insert( SelectedPoint, 1, Char);
			SetValue();

			 SelectedPoint++;
			while ( TextWidth > Size.x - (MeasurementSize.x + 6))
			{
				 VisiblePointerStart++; // update position
				 TextWidth = GetTextSize(OutputString.substr( VisiblePointerStart,  VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
			}

		}
		Char = NULL;
	}
	void SetSelection()
	{
		if (Blocked)
			return;
		if (!Active)
			return;
		if (Held)
		{
			Selecting = true;
			Vector2 relativemousepos = { MousePos.x - (Pos.x + ParentPos.x), MousePos.y - (Pos.y + ParentPos.y) };
			float lastdistance = 99999; // the user shouldn't ever have a resolution/position over this value
			int instance = 0;
			for (int i = VisiblePointerStart; i <= VisiblePointerEnd; i++)
			{
				float width = GetTextSize(OutputString.substr(VisiblePointerStart, i - VisiblePointerStart), "Verdana", 11).x;
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
				instance = VisiblePointerEnd;
			if (SelectedPoint > instance)
			{
				SelectionEnd = SelectedPoint;
				SelectionStart = instance;
			}
			else
			{
				SelectionEnd = instance;
				SelectionStart = SelectedPoint;
			}
		}

		if (IsKeyDown(VK_CONTROL) && IsKeyDown(0x41)) //(A)
		{
			SelectionStart = 0;
			SelectionEnd = OutputString.length();
		}
	}

	void SetSelectionPoint()
	{
		if (Blocked)
			return;
		if (!Active)
			return;
		if (IsMouseInRectangle(Pos + ParentPos, Size) && IsKeyClicked(VK_LBUTTON))
		{
			Held = true;
			Vector2 relativemousepos = { MousePos.x - (Pos.x + ParentPos.x), MousePos.y - (Pos.y + ParentPos.y) };
			// get width
			// get last position
			// is last position closer or this one?
			// if last position is closer return last position.
			float lastdistance = 999999;
			int instance = 0;
			for (int i = VisiblePointerStart; i <= VisiblePointerEnd; i++)
			{
				float width = GetTextSize(OutputString.substr(VisiblePointerStart, i - VisiblePointerStart), "Verdana", 11).x;
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
				instance = VisiblePointerEnd;
			SelectedPoint = instance;
		}
	}

	void SelectionDragging()
	{
		if (Blocked)
			return;
		if (!Active)
			return;
		if (Held)
		{
			const int delay = 100; // Set the desired delay in milliseconds
			auto time = std::chrono::high_resolution_clock::now();
			// push text forwards
			while (VisiblePointerEnd != OutputString.length() && SelectionEnd == VisiblePointerEnd)
			{
				auto curtime = std::chrono::high_resolution_clock::now();
				auto elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - time).count();
				if (elapsedtime >= delay)
				{
					SelectedPoint++;
					VisiblePointerEnd++;
					VisiblePointerStart++;
				}
			}

			while (VisiblePointerStart != 0 && SelectionStart == VisiblePointerStart)
			{
				auto curtime = std::chrono::high_resolution_clock::now();
				auto elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - time).count();
				if (elapsedtime >= delay)
				{
					SelectedPoint--;
					VisiblePointerEnd--;
					VisiblePointerStart--;
					time = std::chrono::high_resolution_clock::now();
				}
			}
		}
	}
	void DeleteText()
	{
		if (Blocked)
			return;
		if (!Active)
			return;
		WPARAM character = Char;
		
		if (character == VK_BACK && OutputString.length() != 0 && VisiblePointerEnd != 0 && SelectedPoint != 0) // backspace
		{
		
			if (SelectionStart == SelectedPoint && SelectionEnd == SelectedPoint && !Selecting)
			{
				if (SelectedPoint == VisiblePointerEnd)
				{
					OutputString.erase(std::prev(OutputString.end()));
					VisiblePointerEnd--;
					SelectedPoint--;
					SetValue();
				}
				else
				{
					OutputString.erase(SelectedPoint - 1, 1);
					SelectedPoint--;
					VisiblePointerEnd--;
					SetValue();
				}

				if (VisiblePointerStart != 0 && GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x < Size.x - (MeasurementSize.x + 6))
				{
					VisiblePointerStart--;
				}
				// detect if there is any other text that we might need to add so our string doesn't randomly get cut off
				while (TextWidth < Size.x - (MeasurementSize.x + 6) && OutputString.length() > VisiblePointerEnd)
				{
					VisiblePointerEnd++; // update position
					SelectedPoint++;
					TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart + 1, VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
				}
			}
			else
			{
				
				//selecting
				// only change selectedpoint if its going to be removed as well
				if (SelectedPoint == SelectionEnd)
				{
					OutputString.erase(SelectionStart, SelectionEnd - SelectionStart);
					VisiblePointerEnd -= SelectionEnd - SelectionStart;
					SelectedPoint -= SelectionEnd - SelectionStart;
					SetValue();
				}
				else
				{
					OutputString.erase(SelectionStart, SelectionEnd - SelectionStart);
					VisiblePointerEnd -= SelectionEnd - SelectionStart;
					SetValue();
				}
				while (TextWidth < Size.x - (MeasurementSize.x + 6) && VisiblePointerStart > 0)
				{
					VisiblePointerStart--; // Move the starting point up
					TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x;
				}

				// If the text still doesn't fill the TextBox, try to extend from the end
				while (TextWidth < Size.x - (MeasurementSize.x + 6) && VisiblePointerEnd < OutputString.length())
				{
					VisiblePointerEnd++; // Extend the ending point
					TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x;
				}

				//reset selected points
				SelectionStart = SelectedPoint;
				SelectionEnd = SelectedPoint;
				Held = false;
				Selecting = false;
			}
			Char = NULL;
		}
		
	}
	void CopyText()
	{
		if (Blocked)
			return;
		if (!Active)
			return;
		if (SelectedPoint == SelectionStart && SelectedPoint == SelectionEnd)
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

		wcsncpy_s(text, size / sizeof(wchar_t), OutputString.substr(SelectionStart, SelectionEnd - SelectionStart).c_str(), SelectionEnd - SelectionStart);

		text[SelectionEnd - SelectionStart] = L'\0';
		GlobalUnlock(global);
		EmptyClipboard();
		SetClipboardData(CF_UNICODETEXT, global);
		CloseClipboard();
	}

	void PasteText()
	{
		if (Blocked)
			return;
		if (!Active)
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
		for(wchar_t c : clipboard)
		{
			if (!IsKeyValid(c))
				return; // if clipboard isn't a number then return

		}
		if (LastClick < (clock() * 0.00001f))
		{
			if (SelectedPoint == SelectionStart && SelectedPoint == SelectionEnd)
			{
				VisiblePointerEnd += clipboard.length();
				OutputString.insert(SelectedPoint, clipboard);
				SelectedPoint += clipboard.length();
				TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x;
				while (TextWidth > Size.x - (MeasurementSize.x + 6))
				{
					VisiblePointerStart++; // update position
					TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x; // update width so we can exit
				}
				SetValue();
			}
			else
			{
				if (SelectedPoint == SelectionEnd)
				{
					OutputString.erase(SelectionStart, SelectionEnd - SelectionStart);
					VisiblePointerEnd -= SelectionEnd - SelectionStart;
					SelectedPoint -= SelectionEnd - SelectionStart;
					SetValue();
				}
				else
				{
					OutputString.erase(SelectionStart, SelectionEnd - SelectionStart);
					VisiblePointerEnd -= SelectionEnd - SelectionStart;
					SetValue();
				}
				VisiblePointerEnd += clipboard.length();
				OutputString.insert(SelectedPoint, clipboard);
				SetValue();
				SelectedPoint += clipboard.length();
				TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x;
				while (TextWidth < Size.x - (MeasurementSize.x + 6) && VisiblePointerStart > 0)
				{
					VisiblePointerStart--; // Move the starting point up
					TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x;
				}

				// If the text still doesn't fill the TextBox, try to extend from the end
				while (TextWidth < Size.x - (MeasurementSize.x + 6) && VisiblePointerEnd < OutputString.length())
				{
					VisiblePointerEnd++; // Extend the ending point
					TextWidth = GetTextSize(OutputString.substr(VisiblePointerStart, VisiblePointerEnd), "Verdana", 11).x;
				}

				//reset selected points
				SelectionStart = SelectedPoint;
				SelectionEnd = SelectedPoint;
			}
			LastClick = (clock() * 0.00001f) + 0.002f;
		}
	}

public:
	void Update()
	{
		if (!Parent)
			SetVisible(false);
		if (!IsVisible())
			return;
		TextWidth = GetTextSize(VisibleString, "Verdana", 11).x;
		ParentPos = Parent->GetParentPos();
		VisibleString = OutputString.substr(VisiblePointerStart, VisiblePointerEnd);
		SetState();
		ArrowKeyNavition();
		InputText();
		SetSelectionPoint();
		SetSelection();
		SelectionDragging();
		CopyText();
		PasteText();
		
		

		if (Active) // take input
		{
			if (!Held && !Selecting)
			{
				SelectionStart = SelectedPoint;
				SelectionEnd = SelectedPoint;
			}

			// Update the selected point if it is out of bounds
			if (SelectedPoint > VisiblePointerEnd)
			{
				SelectedPoint = VisiblePointerEnd;
			}


			SelectedPosition = GetTextSize(OutputString.substr(VisiblePointerStart, SelectedPoint - VisiblePointerStart), "Verdana", 11).x;
			SelectingStartPosition = GetTextSize(OutputString.substr(VisiblePointerStart, SelectionStart - VisiblePointerStart), "Verdana", 11).x;
			SelectingEndPosition = GetTextSize(OutputString.substr(VisiblePointerStart, SelectionEnd - VisiblePointerStart), "Verdana", 11).x;

		}
		if (!Active)
		{
			if (*Value < MinValue)
			{
				*Value = MinValue;
			}
			if (*Value > MaxValue)
			{
				*Value = MaxValue;
			}
			OutputValue = *Value;
			ConvertValueToString();
		}
		

	}

	void Draw()
	{
		if (!IsVisible())
			return;
		if (!Parent)
			SetVisible(false);

		MyColour text = MenuColours["NumericText"];
		MyColour outline = MenuColours["NumericOutline"];
		MyColour fill = MenuColours["NumericFill"];
		MyColour textactive = MenuColours["NumericTextActive"];
		MyColour currentLocColour = MenuColours["TextBoxCurrent"];
		MyColour highlightColour = MenuColours["TextBoxHighlight"];
		MyColour bordercolour = MenuColours["NumericBorder"];
		OutlineRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x + 1, Size.y + 1, 1, bordercolour);
		FilledRectangle(ParentPos.x + Pos.x, ParentPos.y + Pos.y, Size.x, Size.y, outline);
		FilledRectangle(ParentPos.x + Pos.x + 2, ParentPos.y + Pos.y + 2, Size.x - (MeasurementSize.x + 4), Size.y - 4, fill);

		DrawText(ParentPos.x + Pos.x + (Size.x - (MeasurementSize.x +1)), ParentPos.y + (Pos.y + 2), Measurement, "Verdana", 12, Active ? textactive : text, None);

		DrawText(ParentPos.x + Pos.x + 3, (ParentPos.y + Pos.y) + (Size.y / 6), VisibleString, "Verdana", 11, text, None);

		std::chrono::duration<float> elapsed = std::chrono::high_resolution_clock::now() - AnimationStart;
		float time = std::fmodf(elapsed.count(), AnimationInterval) / AnimationInterval;
		float easedtime = InOutSine(time);
		if (Active && std::fmod(elapsed.count(), AnimationInterval) < AnimationInterval / 2)
		{
			float alpha = 255.0f * (1.0f - easedtime * 2.0f);
			FilledLine(Pos.x + ParentPos.x + SelectedPosition + 5.f, Pos.y + ParentPos.y + Size.y - 3, Pos.x + ParentPos.x + SelectedPosition + 5.f, Pos.y + ParentPos.y + 3, 1,
				currentLocColour.Modify(currentLocColour.r, currentLocColour.g, currentLocColour.b, static_cast<float>(alpha) / 255.0f));
		}
		if (SelectingStartPosition >= 0 || SelectingEndPosition >= 0)
		{
			float selectionWidth = std::abs(SelectingEndPosition - SelectingStartPosition);
			float startX = Pos.x + ParentPos.x + SelectingStartPosition;
			// Check the direction of selection
			if (SelectingEndPosition > SelectingStartPosition)
				startX += 5.f;
			else
				startX -= 5.f;
			FilledRectangle(startX, Pos.y + ParentPos.y, selectionWidth, Size.y, highlightColour);
		}

	}

	NumericSlider(int x, int y, std::wstring name, std::wstring measurement, T minvalue, T maxvalue, T* value) : Measurement(measurement), MaxValue(maxvalue), MinValue(minvalue), Value(value)
	{
		Pos = { (float)x, (float)y };
		Name = name;
		Size = { 55, 19 };
		OutputValue = *Value;
		ConvertValueToString();
		SetVisible(true);
		MeasurementSize = GetTextSize(Measurement, "Verdana", 12);
	

		VisiblePointerEnd = OutputString.length();
		SetStartIndex(); // this sets start value
		VisibleString = OutputString.substr(VisiblePointerStart, VisiblePointerEnd);
		SelectedPoint = VisiblePointerEnd - VisiblePointerStart;
		SelectedPosition = GetTextSize(OutputString.substr(VisiblePointerStart,SelectedPoint), "Verdana").x;
	

	}
};
