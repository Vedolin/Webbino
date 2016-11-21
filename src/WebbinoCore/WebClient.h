/***************************************************************************
 *   This file is part of Webbino                                          *
 *                                                                         *
 *   Copyright (C) 2012-2016 by SukkoPera                                  *
 *                                                                         *
 *   Webbino is free software: you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Webbino is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Webbino. If not, see <http://www.gnu.org/licenses/>.       *
 ***************************************************************************/

#ifndef _WEBCLIENT_H_
#define _WEBCLIENT_H_

#include <Arduino.h>
#include <webbino_config.h>
#include "HTTPRequestParser.h"


class WebClient: public Print {
protected:
	byte buf[CLIENT_BUFSIZE];
	size_t avail;

	void flushBuffer () {
		if (avail > 0) {
			//~ DPRINT (F("Flushing "));
			//~ DPRINT (avail);
			//~ DPRINTLN (F(" bytes to client"));

			doWrite ((const uint8_t *) buf, avail);
			avail = 0;
		}

	}

	/* Override this to implement the actual sending of the buffer contents to
	 * the client
	 */
	virtual size_t doWrite (const uint8_t *buf, size_t n) {
		return 0;
	}

public:
	HTTPRequestParser request;

	virtual void begin (char* req) {
		request.parse (req);
		avail = 0;
	}

	virtual size_t write (uint8_t c) override {
		buf[avail++] = c;

		if (avail >= CLIENT_BUFSIZE) {
			flushBuffer ();
		}

		return 1;
	}

	virtual void sendReply () {
		flushBuffer ();
	}
};

#endif
