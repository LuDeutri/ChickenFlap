#include "ssd1306_startAnimation.h"


startAnimation_t startAnimation;
pictureDef_t heartDef;
pictureDef_t chickenStandardDef;
pictureDef_t chickenStrechedDef;
pictureDef_t chickenFlapOpenedDef;
pictureDef_t chickenFlapClosedDef;


// 18 x 52
static const uint8_t chickenFlapOpenedTable [] = {

	0x20, 0x00, 0x00,	//0				x
	0x30, 0x00, 0x00,	//1				xx
	0x28, 0x00, 0x00,	//2				x x
	0x24, 0x00, 0x00,	//3				x  x
	0x22, 0x00, 0x00,	//4				x   x
	0x21, 0x00, 0x00,	//5				x	 x
	0xE0, 0x80, 0x00,	//6			  xxx	  x
	0xE0, 0x40, 0x00,	//7			  xxx	   x
	0xE0, 0x20, 0x00,	//8			  xxx		x
	0xE0, 0x10, 0x00,	//9			  xxx		 x
	0xE0, 0x08, 0x00,	//10		  xxx		  x
	0xE0, 0x04, 0x00,	//11		  xxx		   x
	0xE0, 0x02, 0x00,	//12		  xxx			x
	0xE0, 0x01, 0x00,	//13		  xxx			 x
	0xE0, 0x01, 0x00,	//14		  xxx			 x
	0xE0, 0x01, 0x00,	//15		  xxx			 x
	0xE0, 0x01, 0x00,	//16		  xxx 			 x
	0xE0, 0x01, 0xC0,	//17		  xxx  			 xxx
	0xE0, 0x01, 0xC0,	//18		  xxx   		 xxx
	0xE0, 0x01, 0xC0,	//19		  xxx    		 xxx
	0xF0, 0x01, 0xC0,	//20		  xxxx	  		 xxx
	0xE8, 0x01, 0xC0,	//			  xxx x   	     xxx
	0xE4, 0x01, 0xC0,	//			  xxx  x	     xxx
	0xE2, 0x01, 0xC0,	//			  xxx	x	     xxx
	0xE1, 0x01, 0xC0,	//			  xxx	 x	     xxx
	0xE0, 0x81, 0xC0,	//			  xxx	  x      xxx
	0xE0, 0x41, 0xC0,	//			  xxx	   x	 xxx
	0xE0, 0x21, 0xC0,	//			  xxx 		x	 xxx
	0xE0, 0x11, 0xC0,	//			  xxx		 x	 xxx
	0xE0, 0x09, 0xC0,	//			  xxx		  x  xxx
	0xE0, 0x07, 0xC0,	//			  xxx		   x xxx
	0xE0, 0x03, 0xC0,	//			  xxx			xxxx
	0xE0, 0x00, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x00, 0xC0,	//			  xxx			 xxx
	0x00, 0x00, 0xC0,	//			  				 xxx
	0x00, 0x00, 0xC0,	//			  	 			 xxx
	0x00, 0x00, 0xC0,	//			        	     xxx
	0x00, 0x00, 0xC0,	//				 			 xxx
	0x00, 0x00, 0xC0,	//				 	  		 xxx
	0x00, 0x00, 0xC0,	//					  		 xxx
	0x00, 0x00, 0xC0,	//					    	 xxx
	0x00, 0x00, 0xC0,	//							 xxx
	0x00, 0x00, 0xC0,	//						 	 xxx
	0x00, 0x00, 0xC0,	//				 		   	 xxx
	0x00, 0x00, 0xC0	//						     xxx
};

// 39 x 30 (bxh)
static const uint8_t heartTable [] = {
	0x00, 0x00, 0x10, 0x00, 0x00, //1 										x
	0x00, 0x00, 0x28, 0x00, 0x00, //2									   x x
	0x00, 0x00, 0x44, 0x00, 0x00, //3 									  x   x
	0x00, 0x00, 0x82, 0x00, 0x00, //4									 x     x
	0x00, 0x01, 0x01, 0x00, 0x00, //5 									x       x
	0x00, 0x02, 0x00, 0x80, 0x00, //6 								   x         x
	0x00, 0x04, 0x00, 0x40, 0x00, //7 								  x           x
	0x00, 0x08, 0x00, 0x20, 0x00, //8 								 x             x
	0x00, 0x10, 0x00, 0x10, 0x00, //9 								x               x
	0x00, 0x20, 0x00, 0x08, 0x00, //10 							   x                 x
	0x00, 0x40, 0x00, 0x04, 0x00, //11 							  x                   x
	0x00, 0x80, 0x00, 0x02, 0x00, //12 							 x                     x
	0x01, 0x00, 0x00, 0x01, 0x00, //13 							x                       x
	0x02, 0x00, 0x00, 0x00, 0x80, //14 						   x                         x
	0x04, 0x00, 0x00, 0x00, 0x40, //15 						  x                           x
	0x08, 0x00, 0x00, 0x00, 0x20, //16 						 x                             x
	0x10, 0x00, 0x00, 0x00, 0x10, //17 						x                               x
	0x20, 0x00, 0x00, 0x00, 0x08, //18 					   x                                 x
	0x40, 0x00, 0x00, 0x00, 0x04, //19 					  x                                   x
	0x80, 0x00, 0x00, 0x00, 0x02, //20 					 x                                     x
	0x80, 0x00, 0x00, 0x00, 0x02, //21 					 x                                     x
	0x80, 0x00, 0x00, 0x00, 0x02, //22 					 x                                     x
	0x40, 0x00, 0x00, 0x00, 0x04, //23 					  x                                   x
	0x40, 0x00, 0x10, 0x00, 0x04, //24 					  x                 x                 x
	0x20, 0x00, 0x38, 0x00, 0x08, //25 					   x               xxx               x
	0x20, 0x00, 0x28, 0x00, 0x08, //26 					   x               x x               x
	0x10, 0x00, 0x44, 0x00, 0x10, //27 					    x             x   x             x
	0x08, 0x00, 0x82, 0x00, 0x20, //28 					     x           x     x           x
	0x04, 0x03, 0x01, 0x80, 0x40, //29 					      x        xx       xx        x
	0x03, 0xFC, 0x00, 0x7F, 0x80  //30 					       xxxxxxxx           xxxxxxxx
};

// 52 x 46
static const uint8_t chickenStandardTable [] = {

	0x00, 0x00, 0x00, 0x00, 0x0D, 0xC0, 0x00,  //											XX XXX
	0x00, 0x00, 0x00, 0x00, 0x12, 0x50, 0x00,  //										   X  X  X XX
	0x00, 0x00, 0x00, 0x00, 0xFD, 0x28, 0x00,  //										XXXX	  X X
	0x00, 0x00, 0x00, 0x00, 0x80, 0x08, 0x00,  //										X			X
	0x00, 0x00, 0x00, 0x00, 0x60, 0x08, 0x00,  //										 XX			X
	0x00, 0x00, 0x00, 0x00, 0x10, 0x04, 0x00,  //										   X		 X
	0x00, 0x00, 0x00, 0x00, 0x0F, 0xE4, 0x00,  //										   XXXXXXX   X
	0x00, 0x00, 0x00, 0x00, 0x09, 0x14, 0x00,  //										   X  X   X  X
	0x00, 0x00, 0x00, 0x00, 0x12, 0x48, 0x00,  //										  X  X  X  X X
	0x00, 0x00, 0x00, 0x00, 0x22, 0x44, 0x00,  //										 X   X  X   X
	0x00, 0x00, 0x00, 0x00, 0x21, 0x0F, 0x00,  //										 X    X    XXXX
	0x00, 0x00, 0x00, 0x00, 0x42, 0x10, 0x80,  //										X    X    X    X
	0x00, 0x00, 0x00, 0x00, 0x41, 0x5E, 0x40,  //										X     X X XXXX  X
	0x00, 0x00, 0x00, 0x00, 0x80, 0xC5, 0x80,  //									   X	   XX   X XX
	0x00, 0x00, 0x00, 0x00, 0x80, 0x44, 0x00,  //									   X		X   X
	0x00, 0x00, 0x00, 0x00, 0x80, 0x42, 0x00,  //									   X		X    X
	0x00, 0x00, 0x00, 0x01, 0xA0, 0x21, 0x00,  //									  XX x		 X    X
	0x00, 0x00, 0x00, 0x01, 0x40, 0x21, 0x00,  //									  X X		 X     X
	0x00, 0x3C, 0x00, 0x01, 0x50, 0x11, 0x00,  //				 XXXX				  X X		  X	  X
	0x00, 0xC3, 0x80, 0x02, 0x20, 0x0E, 0x00,  //			   XX	 XXX			 X  X X		   XXX
	0x01, 0x00, 0x40, 0x04, 0x20, 0x02, 0x00,  //			  X			X			X    X			 X
	0x02, 0x00, 0x20, 0x04, 0x24, 0x02, 0x00,  //			 X			 X		    X	 X			 X
	0x04, 0x11, 0x10, 0x08, 0x18, 0x02, 0x00,  //			X	  X	  X	  X 	   X     XX X		 X
	0x04, 0x62, 0x08, 0x10, 0x0A, 0x02, 0x00,  //			X	XX	 X	   X	  X       X			 X
	0x08, 0xA2, 0x08, 0x12, 0x04, 0x02, 0x00,  //		   X   X X 	 X	   X	  X  X	   X		 X
	0x09, 0x44, 0x04, 0x64, 0x04, 0x02, 0x00,  //		   X  X	X	X		X	XX  X		X X		 X
	0x13, 0x4C, 0x47, 0x88, 0x04, 0x02, 0x00,  //		  X	 X	X  XX   X	XXXX   X		X		 X
	0x12, 0x54, 0x84, 0x30, 0x84, 0x02, 0x00,  //		  X	 X	X X X  X	X	 XX    X    X		 X
	0x24, 0x94, 0x88, 0xC1, 0x04, 0x82, 0x00,  //		 X	X  X  X	X  X   X   XX  	  X     X  X    X
	0x24, 0xA5, 0x97, 0x02, 0x07, 0x84, 0x00,  //		 X	X  X X	X XX  X XXX		 X      XXX     X
	0x28, 0xA9, 0x90, 0x0C, 0x08, 0xA8, 0x00,  //		 X X   X X X  XX  X   	   XX      X    X  X
	0x49, 0x2A, 0xAA, 0xF0, 0x30, 0x48, 0x00,  //		X  X  X	 X X X X X X X XXXX      XX	    X X
	0x51, 0x4A, 0xA9, 0x00, 0xC0, 0x50, 0x00,  //		X X   X X  X X X X X  X		   XX	    XX
	0x51, 0x4A, 0xA8, 0xC3, 0x00, 0x60, 0x00,  //		X X   X X  X X X X X   XX	 XX		   X
	0x91, 0x4A, 0xA4, 0x3C, 0x00, 0x80, 0x00,  //	   X  X   X X  X X X X  X X	 XXXX	  X    X
	0x91, 0x4A, 0xA2, 0x00, 0x10, 0x80, 0x00,  //	   X  X   X X  X X X X   XX			  X   X
	0x91, 0x4A, 0xC1, 0x00, 0x11, 0x00, 0x00,  //	   X  X   X X  X X XX     X		     X  XX
	0xA1, 0x4C, 0x81, 0x00, 0x26, 0x00, 0x00,  //	   X X    X X  XX  X	   X	     XXX
	0xA1, 0x88, 0x00, 0x80, 0x38, 0x00, 0x00,  //	   X X	  XX   X           X	     X
	0xA1, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,  //	   X X	  X					X       X
	0xA0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,  //	   X X						 XXXXXXX
	0xA0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,  //	   X X						   X X
	0xA0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,  //	   X X						   X X
	0xC0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,  //	   XX						   X X
	0x80, 0x00, 0x00, 0x0B, 0x80, 0x00, 0x00,  //	   X						   X XXX
	0x00, 0x00, 0x00, 0x14, 0x40, 0x00, 0x00,  //								  X X   X
	0x00, 0x00, 0x00, 0x1B, 0x80, 0x00, 0x00   //								  XX XXXX
};

// 52 x 46
static const uint8_t chickenStrechedTable [] = {

	0x00, 0x01, 0xC0, 0x00, 0x0D, 0xC0, 0x00,  //				   XXXX						XX XXX
	0x00, 0x01, 0x30, 0x00, 0x12, 0x50, 0x00,  //				  X    X				   X  X  X XX
	0x00, 0x01, 0x08, 0x00, 0xFD, 0x28, 0x00,  //				  X	    X				XXXX	  X X
	0x00, 0x00, 0x84, 0x00, 0x80, 0x08, 0x00,  //				   X   	 X			   X			X
	0x00, 0x03, 0x42, 0x00, 0x60, 0x08, 0x00,  //					X 	  X				 XX			X
	0x00, 0x02, 0xA1, 0x00, 0x10, 0x04, 0x00,  //					 XX	   X			   X		 X
	0x00, 0x02, 0x60, 0x80, 0x0F, 0xE4, 0x00,  //				  XX X X    X			   XXXXXXX   X
	0x00, 0x01, 0x10, 0x40, 0x09, 0x14, 0x00,  //				  X	  X	X	 X			   X  X   X  X
	0x00, 0x00, 0x88, 0x20, 0x12, 0x48, 0x00,  //				  X	   X X	  X			  X  X  X  X X
	0x00, 0x03, 0xC0, 0x20, 0x22, 0x44, 0x00,  //				   X    XXX	   X		 X   X  X   X
	0x00, 0x02, 0x20, 0x10, 0x21, 0x0F, 0x00,  //					X	  X    X		 X    X    XXXX
	0x00, 0x02, 0x18, 0x10, 0x42, 0x10, 0x80,  //				  XXXX	   X	X		X    X    X    X
	0x00, 0x01, 0x80, 0x08, 0x41, 0x5E, 0x40,  //				  X	  X			 X		X     X X XXXX  X
	0x00, 0x00, 0x60, 0x08, 0x80, 0xC5, 0x80,  //				  X	   XX		 X	   X	   XX   X XX
	0x00, 0x01, 0xF8, 0x04, 0x80, 0x44, 0x00,  //				   XX			  X	   X		X   X
	0x00, 0x06, 0x00, 0x05, 0x80, 0x42, 0x00,  //					 XX			  X	   X		X    X
	0x00, 0x04, 0x00, 0x02, 0xA0, 0x21, 0x00,  //				XXXXXXXX		   X  XX 		 X    X
	0x00, 0x03, 0xE6, 0x03, 0x40, 0x21, 0x00,  //			  XX  			       X XX   X		 X     X
	0x00, 0x3C, 0x18, 0x01, 0x50, 0x11, 0x00,  //				X		    XX		X  XX		  X	  X
	0x00, 0xC3, 0xE0, 0x01, 0x20, 0x0E, 0x00,  //			   XX XXXX 	  XX		X	X    	   XXX
	0x01, 0x00, 0x84, 0x00, 0xA0, 0x02, 0x00,  //			  X		  XXXX			 X	 X   		 X
	0x02, 0x01, 0x18, 0x00, 0xA4, 0x02, 0x00,  //			 X			X	  X	     X	 X	X		 X
	0x04, 0x13, 0xF0, 0x00, 0x98, 0x02, 0x00,  //			X	  X	   X   	XX        X  X X		 X
	0x04, 0x62, 0x10, 0x00, 0x8A, 0x02, 0x00,  //			X	XX	  XXXXXXX         X   X			 X
	0x08, 0xA2, 0x42, 0x00, 0x84, 0x02, 0x00,  //		   X   X X 	 X	   X 	      X	   X X		 X
	0x09, 0x44, 0x84, 0x20, 0x04, 0x02, 0x00,  //		   X  X	X	X	  X		      X		X 		 X
	0x13, 0x4D, 0x98, 0x48, 0x07, 0x02, 0x00,  //		  X	 X	X  XX    X	  X    X  		X  X	 X
	0x12, 0x54, 0xE8, 0x92, 0x04, 0x02, 0x00,  //		  X	 X	X X X   XX	XX	  X           X		 X
	0x24, 0x94, 0x91, 0x24, 0x04, 0x82, 0x00,  //		 X	X  X  X	X  X  XX X   X   X	       X    X
	0x24, 0xA5, 0x93, 0x24, 0x07, 0x84, 0x00,  //		 X	X  X X	X XX    X 	X	 X           XXX X
	0x28, 0xA9, 0x9D, 0x48, 0x00, 0xA8, 0x00,  //		 X X   X X X  XX   XX  X    X              X
	0x49, 0x2A, 0xA9, 0x48, 0x00, 0x48, 0x00,  //		X  X  X	 X X X X X X XX X   X      	     X
	0x51, 0x4A, 0xA9, 0xA8, 0x00, 0x50, 0x00,  //		X X   X X  X X X X X  	 X X    	    XX
	0x51, 0x4A, 0xA8, 0x30, 0x00, 0x60, 0x00,  //		X X   X X  X X X X X   	 XX 		    X
	0x91, 0x4A, 0xA4, 0x00, 0x00, 0x80, 0x00,  //	   X  X   X X  X X X X  X 	    	  X    X
	0x91, 0x4A, 0xA2, 0x00, 0x10, 0x80, 0x00,  //	   X  X   X X  X X X X   X			  X   X
	0x91, 0x4A, 0xC1, 0x00, 0x11, 0x00, 0x00,  //	   X  X   X X  X X XX     X		     X  XX
	0xA1, 0x4C, 0x81, 0x00, 0x26, 0x00, 0x00,  //	   X X    X X  XX  X	   X	     XXX
	0xA1, 0x88, 0x00, 0x80, 0x38, 0x00, 0x00,  //	   X X	  XX   X           X	     X
	0xA1, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,  //	   X X	  X					X       X
	0xA0, 0x00, 0x00, 0x3F, 0x80, 0x00, 0x00,  //	   X X						 XXXXXXX
	0xA0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,  //	   X X						   X X
	0xA0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,  //	   X X						   X X
	0xC0, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,  //	   XX						   X X
	0x80, 0x00, 0x00, 0x0B, 0x80, 0x00, 0x00,  //	   X						   X XXX
	0x00, 0x00, 0x00, 0x14, 0x40, 0x00, 0x00,  //								  X X   X
	0x00, 0x00, 0x00, 0x1B, 0x80, 0x00, 0x00   //								  XX XXXX
};

// 18 x 52
static const uint8_t chickenFlapClosedTable [] = {

	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xE0, 0x00, 0x00,	//			  xxx
	0xF0, 0x00, 0x00,	//			  xxxx
	0xE8, 0x00, 0x00,	//			  xxx x
	0xE4, 0x00, 0x00,	//			  xxx  x		 xxx
	0xE2, 0x01, 0xC0,	//			  xxx   x		 xxx
	0xE1, 0x01, 0xC0,	//			  xxx    x		 xxx
	0xE0, 0x81, 0xC0,	//			  xxx	  x		 xxx
	0xE0, 0x41, 0xC0,	//			  xxx	   x	 xxx
	0xE0, 0x21, 0xC0,	//			  xxx 		x    xxx
	0xE0, 0x11, 0xC0,	//			  xxx		 x   xxx
	0xE0, 0x09, 0xC0,	//			  xxx		  x  xxx
	0xE0, 0x05, 0xC0,	//			  xxx		   x xxx
	0xE0, 0x03, 0xC0,	//			  xxx			xxxx
	0xE0, 0x01, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx 			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xE0, 0x01, 0xC0,	//			  xxx			 xxx
	0xF0, 0x01, 0xC0,	//			  xxxx			 xxx
	0xE8, 0x01, 0xC0,	//			  xxx x			 xxx
	0xE4, 0x01, 0xC0,	//			  xxx  x		 xxx
	0x02, 0x01, 0xC0,	//			  	    x		 xxx
	0x01, 0x01, 0xC0,	//			  	     x		 xxx
	0x00, 0x81, 0xC0,	//			  		  x		 xxx
	0x00, 0x41, 0xC0,	//			  		   x	 xxx
	0x00, 0x21, 0xC0,	//			  	 		x    xxx
	0x00, 0x11, 0xC0,	//			  			 x   xxx
	0x00, 0x09, 0xC0,	//			  			  x  xxx
	0x00, 0x05, 0xC0,	//			  			   x xxx
	0x00, 0x03, 0xC0,	//			  	 			xxxx
	0x00, 0x01, 0xC0,	//						 	 xxx
	0x00, 0x01, 0xC0	//							 xxx
};



void ssd1306StartAnimation_init(){
	// Check if start animation is enabled
	startAnimation.enable = false;
	#ifdef SSD13006_ENABLE_START_ANIMATION
		startAnimation.enable = true;
	#endif

	// Check if slogan animation is enabled
	startAnimation.sloganFinished = true;
	#ifdef SSD13006_START_ANIMATION_SLOGAN
		startAnimation.sloganFinished = false;
	#endif

	startAnimation.roundMovie = 0;
	startAnimation.roundSlogan = -1;
	startAnimation.drawHeartFinished = false;

	heartDef.tableHeight = 30;
	heartDef.tableWidth = 5;
	heartDef.pictureWidth = 39;
	heartDef.data = heartTable;
	heartDef.displayUpdateWhileBuilding = true;
	heartDef.buildDirection = 1;

	chickenFlapOpenedDef.tableHeight = 52;
	chickenFlapOpenedDef.tableWidth = 3;
	chickenFlapOpenedDef.pictureWidth = 18;
	chickenFlapOpenedDef.data = chickenFlapOpenedTable;
	chickenFlapOpenedDef.displayUpdateWhileBuilding = false;
	chickenFlapOpenedDef.buildDirection = -1;

	chickenFlapClosedDef.tableHeight = 51;
	chickenFlapClosedDef.tableWidth = 3;
	chickenFlapClosedDef.pictureWidth = 18;
	chickenFlapClosedDef.data = chickenFlapClosedTable;
	chickenFlapClosedDef.displayUpdateWhileBuilding = false;
	chickenFlapClosedDef.buildDirection = -1;

	chickenStandardDef.tableHeight = 47;
	chickenStandardDef.tableWidth = 7;
	chickenStandardDef.pictureWidth = 52;
	chickenStandardDef.data = chickenStandardTable;
	chickenStandardDef.displayUpdateWhileBuilding = false;
	chickenStandardDef.buildDirection = -1;

	chickenStrechedDef.tableHeight = 47;
	chickenStrechedDef.tableWidth = 7;
	chickenStrechedDef.pictureWidth = 52;
	chickenStrechedDef.data = chickenStrechedTable;
	chickenStrechedDef.displayUpdateWhileBuilding = false;
	chickenStrechedDef.buildDirection = -1;
}

void ssd1306StartAnimation(){
	if (!startAnimation.enable || millis() > TIMEOUT_MAX_START_ANIMATION_TIME)
		return;
	if (!SSD1306.Initialized)
		return;

	#ifdef SSD13006_START_ANIMATION_SLOGAN
		ssd1306StartAnimation_slogan();
	#endif

	ssd1306_UpdateScreen();
	ssd1306_Fill(Black);

	#ifdef SSD13006_START_ANIMATION_MOVIE_SCENE
		ssd1306StartAnimation_movieScene();
	#endif

	if(startAnimation.movieFinished && startAnimation.sloganFinished)
		startAnimation.enable = false;
}

void ssd1306StartAnimation_slogan(){
	// Stop if already finished
	if(startAnimation.sloganFinished){
		startAnimation.roundSlogan = -1;
		return;
	}

	// Reset diplay
	ssd1306_Fill(Black);

	char strHeadline[30] = "Chickenflap v2.1|9'23";
	ssd1306_SetCursor(0,0);
	ssd1306_WriteString(strHeadline, Font_6x8, White);

	char strSecondLine[20] = "Luca Deutrich";
	ssd1306_SetCursor(0,9);
	ssd1306_WriteString(strSecondLine, Font_6x8, White);

	char strhappyChicken[30] = "For happy chickens";
	ssd1306_SetCursor(10,22);
	ssd1306_WriteString(strhappyChicken, Font_6x8, White);

	if (millis()>1000 && millis() < 15000) {
		drawPicture(40, 60, heartDef);
		// Hold display for a couple of seconds
		delayMillis(6000);
		// Finish slogan
		startAnimation.sloganFinished = true;
	} else if(millis()>2000)
		// Overtime after 15 seconds
		startAnimation.sloganFinished = true;
}

void ssd1306StartAnimation_movieScene(){
	// Wait for end of slogan view
	if (!startAnimation.sloganFinished)
		return;

	// Stop if movie is already ended
	if (startAnimation.movieFinished)
		return;

	switch(startAnimation.roundMovie){
	case 0 ... 12:
		drawPicture(3+startAnimation.roundMovie, 10, chickenStandardDef);
		break;
	case 13 ... 18:
		drawPicture(15+(startAnimation.roundMovie-13), 10, chickenStandardDef);
		drawPicture(100, 5, chickenFlapClosedDef);
		break;
	case 19 ... 23:
		drawPicture(15, 10, chickenStandardDef);
		drawPicture(100, 5, chickenFlapOpenedDef);
		break;
	case 24 ... 28:
		drawPicture(15, 10, chickenStrechedDef);
		drawPicture(100, 5, chickenFlapOpenedDef);
		break;
	case 29 ... 39:
		drawPicture(50+(startAnimation.roundMovie-29), 10, chickenStandardDef);
		drawPicture(15, 5, chickenFlapOpenedDef);
		break;
	default:
		warning.startAnimation = true;
		startAnimation.movieFinished = true;
	}
	ssd1306_UpdateScreen();

	if(startAnimation.roundMovie < 39){
		startAnimation.roundMovie++;
	} else {
		// Finish movie scene
		delayMillis(2000);
		startAnimation.movieFinished = true;
		startAnimation.roundMovie = 0;
	}
}

void drawPicture(uint8_t startX, uint8_t startY, pictureDef_t pictureDef){
	uint8_t data = 0;

	// Check safed builddirection
	if(pictureDef.buildDirection != 1 && pictureDef.buildDirection != -1)
		pictureDef.buildDirection = 1;

	// Check remaining space on coordinates
	if (SSD1306_WIDTH < (startX + pictureDef.tableWidth)
		|| ((SSD1306_HEIGHT < (startY + pictureDef.tableHeight)) && pictureDef.buildDirection == -1)
		|| ((SSD1306_HEIGHT < (startY - pictureDef.tableHeight)) && pictureDef.buildDirection == 1))
	{
		warning.startAnimation = true;
		return;
	}

	for (int y=0; y < pictureDef.tableHeight; y++){
		for (int r=0; r < pictureDef.tableWidth; r++){
			data = pictureDef.data[r+(pictureDef.tableWidth*y)];
			for (int x=0; x <= 7; x++){
				if((data << x) & 0x80){
					ssd1306_DrawPixel(startX+(x+(8*r)), startY-(y*pictureDef.buildDirection), White);
				}
			}
		}
		// Update display
		if(pictureDef.displayUpdateWhileBuilding)
		ssd1306_UpdateScreen();
	}
}
