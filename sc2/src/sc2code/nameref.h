//Copyright Paul Reiche, Fred Ford. 1992-2002

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _NAMEREF_H
#define _NAMEREF_H

#include "restypes.h"

#define LoadCodeRes(r) LoadCodeResInstance (r)
#define LoadColorMap(r) LoadColorMapInstance (r)
#define LoadStringTable(r) LoadStringTableInstance(r)
#define LoadSound(r) LoadSoundInstance(r)
#define LoadMusic(r) LoadMusicInstance(r)

#define INIT_INSTANCES() do \
		{ \
			InstallGraphicResTypes (GFXRES, FONTRES); \
			InstallStringTableResType (STRTAB); \
			InstallAudioResTypes (SNDRES, MUSICRES); \
			InstallCodeResType (CODE); \
		} while (0)
#define UNINIT_INSTANCES()

#endif /* _NAMEREF_H */

