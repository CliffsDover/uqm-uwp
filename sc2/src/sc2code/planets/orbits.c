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

#include "encount.h"
#include "planets/planets.h"
#include "libs/compiler.h"
#include "libs/mathlib.h"


//#define DEBUG_ORBITS

enum
{
	PLANET_NEVER = 0,
	PLANET_RARE = 15,
	PLANET_FEW = 63,
	PLANET_COMMON = 127,
	PLANET_ALWAYS = 255
};

static BYTE
BlueDistribution (BYTE which_world)
{
	BYTE PlanetDistribution[NUMBER_OF_PLANET_TYPES] =
	{
		PLANET_ALWAYS, /* OOLITE_WORLD */
		PLANET_ALWAYS, /* YTTRIC_WORLD */
		PLANET_ALWAYS, /* QUASI_DEGENERATE_WORLD */
		PLANET_ALWAYS, /* LANTHANIDE_WORLD */
		PLANET_ALWAYS, /* TREASURE_WORLD */
		PLANET_ALWAYS, /* UREA_WORLD */
		PLANET_ALWAYS, /* METAL_WORLD */
		PLANET_ALWAYS, /* RADIOACTIVE_WORLD */
		PLANET_ALWAYS, /* OPALESCENT_WORLD */
		PLANET_ALWAYS, /* CYANIC_WORLD */
		PLANET_ALWAYS, /* ACID_WORLD */
		PLANET_ALWAYS, /* ALKALI_WORLD */
		PLANET_ALWAYS, /* HALIDE_WORLD */
		PLANET_ALWAYS, /* GREEN_WORLD */
		PLANET_ALWAYS, /* COPPER_WORLD */
		PLANET_ALWAYS, /* CARBIDE_WORLD */
		PLANET_ALWAYS, /* ULTRAMARINE_WORLD */
		PLANET_ALWAYS, /* NOBLE_WORLD */
		PLANET_ALWAYS, /* AZURE_WORLD */
		PLANET_NEVER, /* CHONDRITE_WORLD */
		PLANET_NEVER, /* PURPLE_WORLD */
		PLANET_NEVER, /* SUPER_DENSE_WORLD */
		PLANET_NEVER, /* PELLUCID_WORLD */
		PLANET_NEVER, /* DUST_WORLD */
		PLANET_NEVER, /* CRIMSON_WORLD */
		PLANET_NEVER, /* CIMMERIAN_WORLD */
		PLANET_NEVER, /* INFRARED_WORLD */
		PLANET_ALWAYS, /* SELENIC_WORLD */
		PLANET_ALWAYS, /* AURIC_WORLD */

		PLANET_ALWAYS, /* FLUORESCENT_WORLD */
		PLANET_ALWAYS, /* ULTRAVIOLET_WORLD */
		PLANET_ALWAYS, /* PLUTONIC_WORLD */
		PLANET_NEVER, /* RAINBOW_WORLD */
		PLANET_NEVER, /* CRACKED_WORLD */
		PLANET_ALWAYS, /* SAPPHIRE_WORLD */
		PLANET_ALWAYS, /* ORGANIC_WORLD */
		PLANET_ALWAYS, /* XENOLITHIC_WORLD */
		PLANET_ALWAYS, /* REDUX_WORLD */
		PLANET_ALWAYS, /* PRIMORDIAL_WORLD */
		PLANET_NEVER, /* EMERALD_WORLD */
		PLANET_ALWAYS, /* CHLORINE_WORLD */
		PLANET_ALWAYS, /* MAGNETIC_WORLD */
		PLANET_ALWAYS, /* WATER_WORLD */
		PLANET_ALWAYS, /* TELLURIC_WORLD */
		PLANET_NEVER, /* HYDROCARBON_WORLD */
		PLANET_NEVER, /* IODINE_WORLD */
		PLANET_NEVER, /* VINYLOGOUS_WORLD */
		PLANET_NEVER, /* RUBY_WORLD */
		PLANET_NEVER, /* MAGMA_WORLD */
		PLANET_NEVER, /* MAROON_WORLD */

		PLANET_ALWAYS, /* BLU_GAS_GIANT */
		PLANET_ALWAYS, /* CYA_GAS_GIANT */
		PLANET_ALWAYS, /* GRN_GAS_GIANT */
		PLANET_ALWAYS, /* GRY_GAS_GIANT */
		PLANET_ALWAYS, /* ORA_GAS_GIANT */
		PLANET_ALWAYS, /* PUR_GAS_GIANT */
		PLANET_ALWAYS, /* RED_GAS_GIANT */
		PLANET_ALWAYS, /* VIO_GAS_GIANT */
		PLANET_ALWAYS, /* YEL_GAS_GIANT */
	};

	return (PlanetDistribution[which_world]);
}

static BYTE
GreenDistribution (BYTE which_world)
{
	BYTE PlanetDistribution[NUMBER_OF_PLANET_TYPES] =
	{
		PLANET_NEVER, /* OOLITE_WORLD */
		PLANET_NEVER, /* YTTRIC_WORLD */
		PLANET_ALWAYS, /* QUASI_DEGENERATE_WORLD */
		PLANET_ALWAYS, /* LANTHANIDE_WORLD */
		PLANET_ALWAYS, /* TREASURE_WORLD */
		PLANET_ALWAYS, /* UREA_WORLD */
		PLANET_ALWAYS, /* METAL_WORLD */
		PLANET_ALWAYS, /* RADIOACTIVE_WORLD */
		PLANET_ALWAYS, /* OPALESCENT_WORLD */
		PLANET_ALWAYS, /* CYANIC_WORLD */
		PLANET_ALWAYS, /* ACID_WORLD */
		PLANET_ALWAYS, /* ALKALI_WORLD */
		PLANET_ALWAYS, /* HALIDE_WORLD */
		PLANET_ALWAYS, /* GREEN_WORLD */
		PLANET_ALWAYS, /* COPPER_WORLD */
		PLANET_ALWAYS, /* CARBIDE_WORLD */
		PLANET_ALWAYS, /* ULTRAMARINE_WORLD */
		PLANET_ALWAYS, /* NOBLE_WORLD */
		PLANET_ALWAYS, /* AZURE_WORLD */
		PLANET_ALWAYS, /* CHONDRITE_WORLD */
		PLANET_ALWAYS, /* PURPLE_WORLD */
		PLANET_ALWAYS, /* SUPER_DENSE_WORLD */
		PLANET_ALWAYS, /* PELLUCID_WORLD */
		PLANET_NEVER, /* DUST_WORLD */
		PLANET_NEVER, /* CRIMSON_WORLD */
		PLANET_NEVER, /* CIMMERIAN_WORLD */
		PLANET_NEVER, /* INFRARED_WORLD */
		PLANET_ALWAYS, /* SELENIC_WORLD */
		PLANET_ALWAYS, /* AURIC_WORLD */

		PLANET_ALWAYS, /* FLUORESCENT_WORLD */
		PLANET_ALWAYS, /* ULTRAVIOLET_WORLD */
		PLANET_ALWAYS, /* PLUTONIC_WORLD */
		PLANET_NEVER, /* RAINBOW_WORLD */
		PLANET_NEVER, /* CRACKED_WORLD */
		PLANET_NEVER, /* SAPPHIRE_WORLD */
		PLANET_ALWAYS, /* ORGANIC_WORLD */
		PLANET_ALWAYS, /* XENOLITHIC_WORLD */
		PLANET_ALWAYS, /* REDUX_WORLD */
		PLANET_ALWAYS, /* PRIMORDIAL_WORLD */
		PLANET_ALWAYS, /* EMERALD_WORLD */
		PLANET_ALWAYS, /* CHLORINE_WORLD */
		PLANET_ALWAYS, /* MAGNETIC_WORLD */
		PLANET_ALWAYS, /* WATER_WORLD */
		PLANET_ALWAYS, /* TELLURIC_WORLD */
		PLANET_ALWAYS, /* HYDROCARBON_WORLD */
		PLANET_ALWAYS, /* IODINE_WORLD */
		PLANET_NEVER, /* VINYLOGOUS_WORLD */
		PLANET_NEVER, /* RUBY_WORLD */
		PLANET_NEVER, /* MAGMA_WORLD */
		PLANET_NEVER, /* MAROON_WORLD */

		PLANET_ALWAYS, /* BLU_GAS_GIANT */
		PLANET_ALWAYS, /* CYA_GAS_GIANT */
		PLANET_ALWAYS, /* GRN_GAS_GIANT */
		PLANET_ALWAYS, /* GRY_GAS_GIANT */
		PLANET_ALWAYS, /* ORA_GAS_GIANT */
		PLANET_ALWAYS, /* PUR_GAS_GIANT */
		PLANET_ALWAYS, /* RED_GAS_GIANT */
		PLANET_ALWAYS, /* VIO_GAS_GIANT */
		PLANET_ALWAYS, /* YEL_GAS_GIANT */
	};

	return (PlanetDistribution[which_world]);
}

static BYTE
OrangeDistribution (BYTE which_world)
{
	BYTE PlanetDistribution[NUMBER_OF_PLANET_TYPES] =
	{
		PLANET_NEVER, /* OOLITE_WORLD */
		PLANET_NEVER, /* YTTRIC_WORLD */
		PLANET_NEVER, /* QUASI_DEGENERATE_WORLD */
		PLANET_NEVER, /* LANTHANIDE_WORLD */
		PLANET_NEVER, /* TREASURE_WORLD */
		PLANET_ALWAYS, /* UREA_WORLD */
		PLANET_NEVER, /* METAL_WORLD */
		PLANET_ALWAYS, /* RADIOACTIVE_WORLD */
		PLANET_NEVER, /* OPALESCENT_WORLD */
		PLANET_NEVER, /* CYANIC_WORLD */
		PLANET_ALWAYS, /* ACID_WORLD */
		PLANET_ALWAYS, /* ALKALI_WORLD */
		PLANET_ALWAYS, /* HALIDE_WORLD */
		PLANET_ALWAYS, /* GREEN_WORLD */
		PLANET_ALWAYS, /* COPPER_WORLD */
		PLANET_ALWAYS, /* CARBIDE_WORLD */
		PLANET_ALWAYS, /* ULTRAMARINE_WORLD */
		PLANET_ALWAYS, /* NOBLE_WORLD */
		PLANET_ALWAYS, /* AZURE_WORLD */
		PLANET_ALWAYS, /* CHONDRITE_WORLD */
		PLANET_ALWAYS, /* PURPLE_WORLD */
		PLANET_ALWAYS, /* SUPER_DENSE_WORLD */
		PLANET_ALWAYS, /* PELLUCID_WORLD */
		PLANET_ALWAYS, /* DUST_WORLD */
		PLANET_ALWAYS, /* CRIMSON_WORLD */
		PLANET_ALWAYS, /* CIMMERIAN_WORLD */
		PLANET_ALWAYS, /* INFRARED_WORLD */
		PLANET_ALWAYS, /* SELENIC_WORLD */
		PLANET_NEVER, /* AURIC_WORLD */

		PLANET_NEVER, /* FLUORESCENT_WORLD */
		PLANET_NEVER, /* ULTRAVIOLET_WORLD */
		PLANET_NEVER, /* PLUTONIC_WORLD */
		PLANET_NEVER, /* RAINBOW_WORLD */
		PLANET_NEVER, /* CRACKED_WORLD */
		PLANET_NEVER, /* SAPPHIRE_WORLD */
		PLANET_NEVER, /* ORGANIC_WORLD */
		PLANET_NEVER, /* XENOLITHIC_WORLD */
		PLANET_NEVER, /* REDUX_WORLD */
		PLANET_NEVER, /* PRIMORDIAL_WORLD */
		PLANET_NEVER, /* EMERALD_WORLD */
		PLANET_ALWAYS, /* CHLORINE_WORLD */
		PLANET_ALWAYS, /* MAGNETIC_WORLD */
		PLANET_ALWAYS, /* WATER_WORLD */
		PLANET_ALWAYS, /* TELLURIC_WORLD */
		PLANET_ALWAYS, /* HYDROCARBON_WORLD */
		PLANET_ALWAYS, /* IODINE_WORLD */
		PLANET_ALWAYS, /* VINYLOGOUS_WORLD */
		PLANET_NEVER, /* RUBY_WORLD */
		PLANET_ALWAYS, /* MAGMA_WORLD */
		PLANET_ALWAYS, /* MAROON_WORLD */

		PLANET_ALWAYS, /* BLU_GAS_GIANT */
		PLANET_ALWAYS, /* CYA_GAS_GIANT */
		PLANET_ALWAYS, /* GRN_GAS_GIANT */
		PLANET_ALWAYS, /* GRY_GAS_GIANT */
		PLANET_ALWAYS, /* ORA_GAS_GIANT */
		PLANET_ALWAYS, /* PUR_GAS_GIANT */
		PLANET_ALWAYS, /* RED_GAS_GIANT */
		PLANET_ALWAYS, /* VIO_GAS_GIANT */
		PLANET_ALWAYS, /* YEL_GAS_GIANT */
	};

	return (PlanetDistribution[which_world]);
}

static BYTE
RedDistribution (BYTE which_world)
{
	BYTE PlanetDistribution[NUMBER_OF_PLANET_TYPES] =
	{
		PLANET_NEVER, /* OOLITE_WORLD */
		PLANET_NEVER, /* YTTRIC_WORLD */
		PLANET_NEVER, /* QUASI_DEGENERATE_WORLD */
		PLANET_NEVER, /* LANTHANIDE_WORLD */
		PLANET_NEVER, /* TREASURE_WORLD */
		PLANET_ALWAYS, /* UREA_WORLD */
		PLANET_ALWAYS, /* METAL_WORLD */
		PLANET_NEVER, /* RADIOACTIVE_WORLD */
		PLANET_NEVER, /* OPALESCENT_WORLD */
		PLANET_NEVER, /* CYANIC_WORLD */
		PLANET_NEVER, /* ACID_WORLD */
		PLANET_NEVER, /* ALKALI_WORLD */
		PLANET_NEVER, /* HALIDE_WORLD */
		PLANET_NEVER, /* GREEN_WORLD */
		PLANET_NEVER, /* COPPER_WORLD */
		PLANET_NEVER, /* CARBIDE_WORLD */
		PLANET_ALWAYS, /* ULTRAMARINE_WORLD */
		PLANET_ALWAYS, /* NOBLE_WORLD */
		PLANET_ALWAYS, /* AZURE_WORLD */
		PLANET_ALWAYS, /* CHONDRITE_WORLD */
		PLANET_ALWAYS, /* PURPLE_WORLD */
		PLANET_ALWAYS, /* SUPER_DENSE_WORLD */
		PLANET_ALWAYS, /* PELLUCID_WORLD */
		PLANET_ALWAYS, /* DUST_WORLD */
		PLANET_ALWAYS, /* CRIMSON_WORLD */
		PLANET_ALWAYS, /* CIMMERIAN_WORLD */
		PLANET_ALWAYS, /* INFRARED_WORLD */
		PLANET_ALWAYS, /* SELENIC_WORLD */
		PLANET_NEVER, /* AURIC_WORLD */

		PLANET_NEVER, /* FLUORESCENT_WORLD */
		PLANET_NEVER, /* ULTRAVIOLET_WORLD */
		PLANET_NEVER, /* PLUTONIC_WORLD */
		PLANET_NEVER, /* RAINBOW_WORLD */
		PLANET_NEVER, /* CRACKED_WORLD */
		PLANET_NEVER, /* SAPPHIRE_WORLD */
		PLANET_NEVER, /* ORGANIC_WORLD */
		PLANET_NEVER, /* XENOLITHIC_WORLD */
		PLANET_NEVER, /* REDUX_WORLD */
		PLANET_NEVER, /* PRIMORDIAL_WORLD */
		PLANET_NEVER, /* EMERALD_WORLD */
		PLANET_NEVER, /* CHLORINE_WORLD */
		PLANET_ALWAYS, /* MAGNETIC_WORLD */
		PLANET_ALWAYS, /* WATER_WORLD */
		PLANET_ALWAYS, /* TELLURIC_WORLD */
		PLANET_ALWAYS, /* HYDROCARBON_WORLD */
		PLANET_ALWAYS, /* IODINE_WORLD */
		PLANET_ALWAYS, /* VINYLOGOUS_WORLD */
		PLANET_ALWAYS, /* RUBY_WORLD */
		PLANET_ALWAYS, /* MAGMA_WORLD */
		PLANET_ALWAYS, /* MAROON_WORLD */

		PLANET_ALWAYS, /* BLU_GAS_GIANT */
		PLANET_ALWAYS, /* CYA_GAS_GIANT */
		PLANET_ALWAYS, /* GRN_GAS_GIANT */
		PLANET_ALWAYS, /* GRY_GAS_GIANT */
		PLANET_ALWAYS, /* ORA_GAS_GIANT */
		PLANET_ALWAYS, /* PUR_GAS_GIANT */
		PLANET_ALWAYS, /* RED_GAS_GIANT */
		PLANET_ALWAYS, /* VIO_GAS_GIANT */
		PLANET_ALWAYS, /* YEL_GAS_GIANT */
	};

	return (PlanetDistribution[which_world]);
}

static BYTE
WhiteDistribution (BYTE which_world)
{
	BYTE PlanetDistribution[NUMBER_OF_PLANET_TYPES] =
	{
		PLANET_ALWAYS, /* OOLITE_WORLD */
		PLANET_ALWAYS, /* YTTRIC_WORLD */
		PLANET_ALWAYS, /* QUASI_DEGENERATE_WORLD */
		PLANET_ALWAYS, /* LANTHANIDE_WORLD */
		PLANET_ALWAYS, /* TREASURE_WORLD */
		PLANET_ALWAYS, /* UREA_WORLD */
		PLANET_ALWAYS, /* METAL_WORLD */
		PLANET_ALWAYS, /* RADIOACTIVE_WORLD */
		PLANET_ALWAYS, /* OPALESCENT_WORLD */
		PLANET_ALWAYS, /* CYANIC_WORLD */
		PLANET_ALWAYS, /* ACID_WORLD */
		PLANET_ALWAYS, /* ALKALI_WORLD */
		PLANET_ALWAYS, /* HALIDE_WORLD */
		PLANET_NEVER, /* GREEN_WORLD */
		PLANET_NEVER, /* COPPER_WORLD */
		PLANET_NEVER, /* CARBIDE_WORLD */
		PLANET_NEVER, /* ULTRAMARINE_WORLD */
		PLANET_NEVER, /* NOBLE_WORLD */
		PLANET_NEVER, /* AZURE_WORLD */
		PLANET_NEVER, /* CHONDRITE_WORLD */
		PLANET_NEVER, /* PURPLE_WORLD */
		PLANET_NEVER, /* SUPER_DENSE_WORLD */
		PLANET_NEVER, /* PELLUCID_WORLD */
		PLANET_NEVER, /* DUST_WORLD */
		PLANET_NEVER, /* CRIMSON_WORLD */
		PLANET_NEVER, /* CIMMERIAN_WORLD */
		PLANET_NEVER, /* INFRARED_WORLD */
		PLANET_ALWAYS, /* SELENIC_WORLD */
		PLANET_ALWAYS, /* AURIC_WORLD */

		PLANET_ALWAYS, /* FLUORESCENT_WORLD */
		PLANET_ALWAYS, /* ULTRAVIOLET_WORLD */
		PLANET_ALWAYS, /* PLUTONIC_WORLD */
		PLANET_NEVER, /* RAINBOW_WORLD */
		PLANET_NEVER, /* CRACKED_WORLD */
		PLANET_ALWAYS, /* SAPPHIRE_WORLD */
		PLANET_ALWAYS, /* ORGANIC_WORLD */
		PLANET_ALWAYS, /* XENOLITHIC_WORLD */
		PLANET_ALWAYS, /* REDUX_WORLD */
		PLANET_ALWAYS, /* PRIMORDIAL_WORLD */
		PLANET_ALWAYS, /* EMERALD_WORLD */
		PLANET_NEVER, /* CHLORINE_WORLD */
		PLANET_NEVER, /* MAGNETIC_WORLD */
		PLANET_NEVER, /* WATER_WORLD */
		PLANET_NEVER, /* TELLURIC_WORLD */
		PLANET_NEVER, /* HYDROCARBON_WORLD */
		PLANET_NEVER, /* IODINE_WORLD */
		PLANET_ALWAYS, /* VINYLOGOUS_WORLD */
		PLANET_ALWAYS, /* RUBY_WORLD */
		PLANET_NEVER, /* MAGMA_WORLD */
		PLANET_NEVER, /* MAROON_WORLD */

		PLANET_ALWAYS, /* BLU_GAS_GIANT */
		PLANET_ALWAYS, /* CYA_GAS_GIANT */
		PLANET_ALWAYS, /* GRN_GAS_GIANT */
		PLANET_ALWAYS, /* GRY_GAS_GIANT */
		PLANET_ALWAYS, /* ORA_GAS_GIANT */
		PLANET_ALWAYS, /* PUR_GAS_GIANT */
		PLANET_ALWAYS, /* RED_GAS_GIANT */
		PLANET_ALWAYS, /* VIO_GAS_GIANT */
		PLANET_ALWAYS, /* YEL_GAS_GIANT */
	};

	return (PlanetDistribution[which_world]);
}

static BYTE
YellowDistribution (BYTE which_world)
{
	BYTE PlanetDistribution[NUMBER_OF_PLANET_TYPES] =
	{
		PLANET_NEVER, /* OOLITE_WORLD */
		PLANET_NEVER, /* YTTRIC_WORLD */
		PLANET_NEVER, /* QUASI_DEGENERATE_WORLD */
		PLANET_NEVER, /* LANTHANIDE_WORLD */
		PLANET_ALWAYS, /* TREASURE_WORLD */
		PLANET_ALWAYS, /* UREA_WORLD */
		PLANET_ALWAYS, /* METAL_WORLD */
		PLANET_ALWAYS, /* RADIOACTIVE_WORLD */
		PLANET_ALWAYS, /* OPALESCENT_WORLD */
		PLANET_ALWAYS, /* CYANIC_WORLD */
		PLANET_ALWAYS, /* ACID_WORLD */
		PLANET_ALWAYS, /* ALKALI_WORLD */
		PLANET_ALWAYS, /* HALIDE_WORLD */
		PLANET_ALWAYS, /* GREEN_WORLD */
		PLANET_ALWAYS, /* COPPER_WORLD */
		PLANET_ALWAYS, /* CARBIDE_WORLD */
		PLANET_ALWAYS, /* ULTRAMARINE_WORLD */
		PLANET_ALWAYS, /* NOBLE_WORLD */
		PLANET_ALWAYS, /* AZURE_WORLD */
		PLANET_ALWAYS, /* CHONDRITE_WORLD */
		PLANET_ALWAYS, /* PURPLE_WORLD */
		PLANET_ALWAYS, /* SUPER_DENSE_WORLD */
		PLANET_ALWAYS, /* PELLUCID_WORLD */
		PLANET_ALWAYS, /* DUST_WORLD */
		PLANET_ALWAYS, /* CRIMSON_WORLD */
		PLANET_ALWAYS, /* CIMMERIAN_WORLD */
		PLANET_ALWAYS, /* INFRARED_WORLD */
		PLANET_ALWAYS, /* SELENIC_WORLD */
		PLANET_ALWAYS, /* AURIC_WORLD */

		PLANET_NEVER, /* FLUORESCENT_WORLD */
		PLANET_NEVER, /* ULTRAVIOLET_WORLD */
		PLANET_NEVER, /* PLUTONIC_WORLD */
		PLANET_NEVER, /* RAINBOW_WORLD */
		PLANET_NEVER, /* CRACKED_WORLD */
		PLANET_NEVER, /* SAPPHIRE_WORLD */
		PLANET_ALWAYS, /* ORGANIC_WORLD */
		PLANET_ALWAYS, /* XENOLITHIC_WORLD */
		PLANET_ALWAYS, /* REDUX_WORLD */
		PLANET_ALWAYS, /* PRIMORDIAL_WORLD */
		PLANET_NEVER, /* EMERALD_WORLD */
		PLANET_ALWAYS, /* CHLORINE_WORLD */
		PLANET_ALWAYS, /* MAGNETIC_WORLD */
		PLANET_ALWAYS, /* WATER_WORLD */
		PLANET_ALWAYS, /* TELLURIC_WORLD */
		PLANET_ALWAYS, /* HYDROCARBON_WORLD */
		PLANET_ALWAYS, /* IODINE_WORLD */
		PLANET_ALWAYS, /* VINYLOGOUS_WORLD */
		PLANET_NEVER, /* RUBY_WORLD */
		PLANET_ALWAYS, /* MAGMA_WORLD */
		PLANET_ALWAYS, /* MAROON_WORLD */

		PLANET_ALWAYS, /* BLU_GAS_GIANT */
		PLANET_ALWAYS, /* CYA_GAS_GIANT */
		PLANET_ALWAYS, /* GRN_GAS_GIANT */
		PLANET_ALWAYS, /* GRY_GAS_GIANT */
		PLANET_ALWAYS, /* ORA_GAS_GIANT */
		PLANET_ALWAYS, /* PUR_GAS_GIANT */
		PLANET_ALWAYS, /* RED_GAS_GIANT */
		PLANET_ALWAYS, /* VIO_GAS_GIANT */
		PLANET_ALWAYS, /* YEL_GAS_GIANT */
	};

	return (PlanetDistribution[which_world]);
}

#define DWARF_ROCK_DIST MIN_PLANET_RADIUS
#define DWARF_GASG_DIST SCALE_RADIUS (12)

#define GIANT_ROCK_DIST SCALE_RADIUS (8)
#define GIANT_GASG_DIST SCALE_RADIUS (13)

#define SUPERGIANT_ROCK_DIST SCALE_RADIUS (16)
#define SUPERGIANT_GASG_DIST SCALE_RADIUS (33)

void
FillOrbits (BYTE NumPlanets, PPLANET_DESC pBaseDesc, BOOLEAN
		TypesDefined)
{ /* Generate Planets in orbit around star */
	BYTE StarColor, PlanetCount, MaxPlanet;
	BOOLEAN GeneratingMoons;
	COUNT StarSize;
	PPLANET_DESC pPD;
	struct
	{
		COUNT MinRockyDist, MinGasGDist;
	} Suns[] =
	{
		{DWARF_ROCK_DIST, DWARF_GASG_DIST},
		{GIANT_ROCK_DIST, GIANT_GASG_DIST},
		{SUPERGIANT_ROCK_DIST, SUPERGIANT_GASG_DIST},
	};
#ifdef DEBUG_ORBITS
UNICODE buf[40];
char stype[] = {'D', 'G', 'S'};
char scolor[] = {'B', 'G', 'O', 'R', 'W', 'Y'};
#endif /* DEBUG_ORBITS */

	pPD = pBaseDesc;
	StarSize = pSolarSysState->SunDesc[0].data_index;
	StarColor = STAR_COLOR (CurStarDescPtr->Type);

	if (NumPlanets == (BYTE)~0)
	{
#define MAX_GENERATED_PLANETS 10
		while ((NumPlanets = (BYTE)(LOWORD (TFB_Random())
				% MAX_GENERATED_PLANETS)) == 0)
			;
		pSolarSysState->SunDesc[0].NumPlanets = NumPlanets;
	}

#ifdef DEBUG_ORBITS
	GetClusterName (CurStarDescPtr, buf);
	fprintf (stderr, "cluster name = %s  color = %c type = %c\n", buf,
			scolor[STAR_COLOR (CurStarDescPtr->Type)],
			stype[STAR_TYPE (CurStarDescPtr->Type)]);
#endif /* DEBUG_ORBITS */
	GeneratingMoons = (BOOLEAN) (pBaseDesc == pSolarSysState->MoonDesc);
	if (GeneratingMoons)
		MaxPlanet = FIRST_LARGE_ROCKY_WORLD;
	else
		MaxPlanet = NUMBER_OF_PLANET_TYPES;
	PlanetCount = NumPlanets;
	while (NumPlanets--)
	{
		BYTE chance;
		DWORD rand_val;
		COUNT min_radius, angle;
		SIZE delta_r;
		PPLANET_DESC pLocPD;

		do
		{
			rand_val = TFB_Random ();
			if (TypesDefined)
				rand_val = 0;
			else
				pPD->data_index = (BYTE)(
						HIBYTE (LOWORD (rand_val))
						% MaxPlanet
						);

			chance = PLANET_NEVER;
			switch (StarColor)
			{
				case BLUE_BODY:
					chance = BlueDistribution (pPD->data_index);
					break;
				case GREEN_BODY:
					chance = GreenDistribution (pPD->data_index);
					break;
				case ORANGE_BODY:
					chance = OrangeDistribution (pPD->data_index);
					break;
				case RED_BODY:
					chance = RedDistribution (pPD->data_index);
					break;
				case WHITE_BODY:
					chance = WhiteDistribution (pPD->data_index);
					break;
				case YELLOW_BODY:
					chance = YellowDistribution (pPD->data_index);
					break;
			}
		} while (LOBYTE (LOWORD (rand_val)) >= chance);

		if (pPD->data_index < FIRST_GAS_GIANT)
			min_radius = Suns[StarSize].MinRockyDist;
		else
			min_radius = Suns[StarSize].MinGasGDist;
RelocatePlanet:
		rand_val = TFB_Random ();
		if (GeneratingMoons)
		{
			pPD->radius = MIN_MOON_RADIUS
					+ ((LOWORD (rand_val) % MAX_MOONS) * MOON_DELTA);
			for (pLocPD = pPD - 1; pLocPD >= pBaseDesc; --pLocPD)
			{
				if (pPD->radius == pLocPD->radius)
					goto RelocatePlanet;
			}
			pPD->NumPlanets = 0;
		}
		else
		{
			pPD->radius =  (LOWORD (rand_val)
					% (MAX_PLANET_RADIUS - min_radius))
					+ min_radius;
			for (pLocPD = pPD - 1; pLocPD >= pBaseDesc; --pLocPD)
			{
				delta_r = UNSCALE_RADIUS (pLocPD->radius) / 5
						- UNSCALE_RADIUS (pPD->radius) / 5;
				if (delta_r < 0)
					delta_r = -delta_r;
				if (delta_r <= 1)
					goto RelocatePlanet;
			}
		}

		rand_val = TFB_Random ();
		angle = NORMALIZE_ANGLE (LOWORD (rand_val));
		pPD->location.x =
				COSINE (angle, pPD->radius);
		pPD->location.y =
				SINE (angle, pPD->radius);
		pPD->rand_seed = MAKE_DWORD (
				pPD->location.x,
				pPD->location.y
				);

		++pPD;
	}

	{
		BYTE i;

		for (i = 0; i < PlanetCount; ++i)
		{
			BYTE j;

			for (j = (BYTE)(PlanetCount - 1); j > i; --j)
			{
				if (pBaseDesc[i].radius > pBaseDesc[j].radius)
				{
					PLANET_DESC temp;

					temp = pBaseDesc[i];
					pBaseDesc[i] = pBaseDesc[j];
					pBaseDesc[j] = temp;
				}
			}
		}
	}
}

