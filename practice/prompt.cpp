//
// C++ Implementation: prompt
//
// Description: Implementation of the prompt (or question) displaying class
//
//
// Author: David Capel <wot.narg@gmail.com>, (C) 2008
//

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "prompt.h"

TextualPrompt::TextualPrompt(QWidget * parent) : QLabel(parent) {};
SoundPrompt::SoundPrompt(QWidget * parent) : QLabel(parent) {};
ImagePrompt::ImagePrompt(QWidget * parent) : QLabel(parent) {};

