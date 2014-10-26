/*----------------------------------------------------------------------------*\
  MODULE NAME: button.cpp
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: 
\*----------------------------------------------------------------------------*/
#include "button.h"



namespace F3DB {


	Button::Button(Controls *controls, uint8 key, ButtonType type) {
		this->controls = controls;
		this->key = key;
		this->type = type;
		mouse = false;
		q = false;
		enabled = true;
	}


	Button::Button(Controls *controls, MouseButton mb, ButtonType type) {
		this->controls = controls;
		this->mb = mb;
		this->type = type;
		mouse = true;
		q = false;
		enabled = true;
	}


	void Button::Enable(bool enable) {
		enabled = enable;
	}


	bool Button::IsEnabled() {
		return enabled;
	}


	bool Button::IsActive() {

		if(!enabled) return false;

		bool s;
		
		if(mouse) s = controls->IsMouseButtonDown(mb);
		else s = controls->IsKeyDown(key);
		
		if(type == F3DB_BT_KEY) {
			return s;
		} else {
			if(s) {
				if(q) return false;
				q = true;
				return true;
			} else {
				q = false;
				return false;
			}
		}
	}

}
