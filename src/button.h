/*----------------------------------------------------------------------------*\
  MODULE NAME: button.h
      PROJECT: F3DB
       AUTHOR: Evert Bauwens
         DATE: september 8 2003
  DESCRIPTION: A button object represents a single key or button ont the
  keyboard or mouse. There are two types of buttons, a 'key' and a 'trigger'. A
  key simply follosw the state of the input, while a trigger produces a single
  pulse when the input rises.

          ----
         |    |
     ----      ---- input (keyboard or mouse)
          ----
         |    |
     ----      ---- 'key'

         |
     ---- --------- 'trigger'

\*----------------------------------------------------------------------------*/
#ifndef F3DB_BUTTON_H
#define F3DB_BUTTON_H
#include "controls.h"



namespace F3DB {


	enum ButtonType {
		F3DB_BT_KEY,
		F3DB_BT_TRIGGER,
	};

	
	class Button {

	public:

		Button(Controls *controls, uint8 key, ButtonType type);
		Button(Controls *controls, MouseButton mb, ButtonType type);

		void Enable(bool enable);
		bool IsEnabled();

		bool IsActive();

	private:

		Controls *controls;
		uint8 key;
		MouseButton mb;
		ButtonType type;
		bool mouse;
		bool q;
		bool enabled;
	};

}


#endif // #ifndef F3DB_BUTTON_H