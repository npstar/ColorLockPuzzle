﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security;
using System.IO;

#if OS_W8
using Windows.Security.Cryptography;
using Windows.Security.Cryptography.Core;
using Windows.Storage.Streams;
#else
using System.Security.Cryptography;
#endif

namespace WCPToolkit
{
    namespace Utils
    {
        namespace Controller
        {
            public enum ControllerEvent
            {
                UNDEFINED = 0,

			//////////////////////////////////////////////////////////////////////////
			/// will take continuous values between (0, 1) 
			//////////////////////////////////////////////////////////////////////////
			LeftTrigger = 1,
			//////////////////////////////////////////////////////////////////////////
			/// will take continuous values between (0, 1) 
			//////////////////////////////////////////////////////////////////////////
			RightTrigger = 2,
			//////////////////////////////////////////////////////////////////////////
			/// will take continuous values between (-1, 1)
			//////////////////////////////////////////////////////////////////////////
			LeftStickX = 3,
			//////////////////////////////////////////////////////////////////////////
			/// will take continuous values between (-1, 1)
			//////////////////////////////////////////////////////////////////////////
			LeftStickY = 4,
			//////////////////////////////////////////////////////////////////////////
			/// will take continuous values between (-1, 1)
			//////////////////////////////////////////////////////////////////////////
			RightStickX = 5,
			//////////////////////////////////////////////////////////////////////////
			/// will take continuous values between (-1, 1)
			//////////////////////////////////////////////////////////////////////////
			RightStickY = 6,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			DpadEventUp = 7,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			DpadEventDown = 8,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			DpadEventLeft = 9,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			DpadEventRight = 10,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			LeftBumper = 11,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			RightBumper = 12,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonY = 13,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonA = 14,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonX = 15,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonB = 16,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonStart = 17,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonSelect = 18,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			ButtonBack = 19,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			LeftStickButton = 20,
			//////////////////////////////////////////////////////////////////////////
			/// will take fixed values, 1 when pressed, 0 when released
			//////////////////////////////////////////////////////////////////////////
			RightStickButton = 21,
            }
        }
    }
}