/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../core/Guard.hpp"
#include "../core/String.hpp"
#include "../core/Util.hpp"
#include "ScenarioSources.h"

#include "Scenario.h"

namespace ScenarioSources
{

    #pragma region Scenario Data

    // clang-format off
    static constexpr const ScenarioAlias ScenarioAliases[] =
    {
        // UK - US differences:
        { "Katie's Dreamland",                      "Katie's World"                             },
        { "Pokey Park",                             "Dinky Park"                                },
        { "White Water Park",                       "Aqua Park"                                 },
        { "Mystic Mountain",                        "Mothball Mountain"                         },
        { "Paradise Pier",                          "Big Pier"                                  },
        { "Paradise Pier 2",                        "Big Pier 2"                                },
        { "Haunted Harbour",                        "Haunted Harbor"                            },
        { "Mythological - Cradle of Civilisation",  "Mythological - Cradle of Civilization"     },

        // RCT1 pack by RCTScenarioLover has a mistake:
        { "Geoffrey Gardens",                       "Geoffery Gardens"                          },

        // CD Projekt Polish Edition
        { "Alpine Adventures",                          "Górska przygoda"                           },
        { "Amity Airfield",                             "Lotnisko Pogięcie"                         },
        { "Botany Breakers",                            "Wyspa Odludna"                             },
        { "Bumbly Bazaar",                              "Gwarne targowisko"                         },
        { "Crazy Castle",                               "Zwariowany zamek"                          },
        { "Dusty Greens",                               "Zapiaszczone Dołki"                        },
        { "Electric Fields",                            "Pola elektryczne"                          },
        { "Extreme Heights",                            "Ekstremalne wysokości"                     },
        { "Factory Capers",                             "300% normy"                                },
        { "Fungus Woods",                               "Grzybowe Lasy"                             },
        { "Ghost Town",                                 "Miasto-widmo"                              },
        { "Gravity Gardens",                            "Ogrody Grawitacji"                         },
        { "Infernal Views",                             "Piekielny widok"                           },
        { "Lucky Lake",                                 "Jezioro Szczęściarzy"                      },
        { "Rainbow Summit",                             "Tęczowy Szczyt"                            },

        { "Six Flags Belgium",                          "\"Sześć Flag\" - Belgia"                   },
        { "Six Flags Holland",                          "\"Sześć Flag\" - Holandia"                 },
        { "Six Flags Magic Mountain",                   "Czarodziejska góra \"Sześciu Flag\""       },
        { "Six Flags Great Adventure",                  "\"Sześć Flag\" - wielka przygoda"          },
        { "Six Flags over Texas",                       "\"Sześć Flag nad Teksasem\""             },
        { "Build your own Six Flags Belgium",           "Wybuduj własny park \"Sześć Flag\" - Belgia" },
        { "Build your own Six Flags Holland",           "Wybuduj własny park \"Sześć Flag\" - Holandia" },
        { "Build your own Six Flags Magic Mountain",    "Wybuduj własny park czarodziejska góra \"Sześciu Flag\"" },
        { "Build your own Six Flags Great Adventure",   "Wybuduj własny park \"Sześć Flag\" - wielka przygoda" },
        { "Build your own Six Flags over Texas",        "Wybuduj własny park \"Sześć Flag nad Teksasem\"" },
        { "Build your own Six Flags Park",              "Wybuduj własny park \"Sześć Flag\""        },

        { "Africa - African Diamond Mine",              "Afryka - kopalnia diamentów"               },
        { "Africa - Oasis",                             "Afryka - oaza"                             },
        { "Africa - Victoria Falls",                    "Afryka - Wodospad Wiktorii"                },
        { "Antarctic - Ecological Salvage",             "Antarktyka - utylizacja"                   },
        { "Asia - Great Wall of China Tourism Enhancement", "Azja - rozwój turystyki wokół Wielkiego Muru Chińskiego" },
        { "Asia - Japanese Coastal Reclaim",            "Azja - odzysk japońskiego wybrzeża"        },
        { "Asia - Maharaja Palace",                     "Azja - pałac maharadży"                    },
        { "Australasia - Ayers Rock",                   "Australazja - Ayers Rock"                  },
        { "Australasia - Fun at the beach",             "Australazja - zabawa na plaży"             },
        { "Europe - European Cultural Festival",        "Europa - Europejski Festiwal Kulturalny"   },
        { "Europe - Renovation",                        "Europa - odbudowa"                         },
        { "N. America - Extreme Hawaiian Island",       "Ameryka Płn. - ekstremalne Hawaje"         },
        { "North America - Grand Canyon",               "Ameryka Płn. - Wielki Kanion"              },
        { "North America - Rollercoaster Heaven",       "Ameryka Płn. - raj kolejek górskich"       },
        { "South America - Inca Lost City",             "Ameryka Płd. - zaginione miasto Inków"     },
        { "South America - Rain Forest Plateau",        "Ameryka Płd. - tropikalny płaskowyż"       },
        { "South America - Rio Carnival",               "Ameryka Płd. - karnawał w Rio"             },

        { "Dark Age - Castle",                          "Mroczne Wieki - zamek"                     },
        { "Dark Age - Robin Hood",                      "Mroczne Wieki - Robin Hood"                },
        { "Future - First Encounters",                  "Przyszłość - pierwsze spotkania"           },
        { "Future - Future World",                      "Przyszłość - świat przyszłości"            },
        { "Mythological - Animatronic Film Set",        "Mitologia - mechaniczny plan filmowy"      },
        { "Mythological - Cradle of Civilisation",      "Mitologia - kołyska cywilizacji"           },
        { "Prehistoric - After the Asteroid",           "Prehistoria - po asteroidzie"              },
        { "Prehistoric - Jurassic Safari",              "Prehistoria - jurajskie safari"            },
        { "Prehistoric - Stone Age",                    "Prehistoria - epoka kamienna"              },
        { "Roaring Twenties - Prison Island",           "Ryczące Dwudziestki - wyspa więzienna"     },
        { "Roaring Twenties - Schneider Cup",           "Ryczące Dwudziestki - puchar Schneidera"   },
        { "Roaring Twenties - Skyscrapers",             "Ryczące Dwudziestki - drapacze chmur"      },
        { "Rock 'n' Roll - Flower Power",               "Rock 'n' Roll - dzieci kwiaty"             },
        // Skipping Rock 'n' Roll - Rock 'n' Roll as that name matches exactly.

    };

    // RCT
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRCT1[] =
    {
        { SC_FOREST_FRONTIERS,          "Forest Frontiers",     SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1 + 0    },
        { SC_DYNAMITE_DUNES,            "Dynamite Dunes",       SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1 + 1    },
        { SC_LEAFY_LAKE,                "Leafy Lake",           SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1 + 2    },
        { SC_DIAMOND_HEIGHTS,           "Diamond Heights",      SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1 + 3    },
        { SC_EVERGREEN_GARDENS,         "Evergreen Gardens",    SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1 + 4    },
        { SC_BUMBLY_BEACH,              "Bumbly Beach",         SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1 + 5    },
        { SC_TRINITY_ISLANDS,           "Trinity Islands",      SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 6    },
        { SC_KATIES_DREAMLAND,          "Katie's Dreamland",    SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 7    },
        { SC_POKEY_PARK,                "Pokey Park",           SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 8    },
        { SC_WHITE_WATER_PARK,          "White Water Park",     SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 9    },
        { SC_MILLENNIUM_MINES,          "Millennium Mines",     SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 10   },
        { SC_KARTS_COASTERS,            "Karts & Coasters",     SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 11   },
        { SC_MELS_WORLD,                "Mel's World",          SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 12   },
        { SC_MYSTIC_MOUNTAIN,           "Mystic Mountain",      SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 13   },
        { SC_PACIFIC_PYRAMIDS,          "Pacific Pyramids",     SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 14   },
        { SC_CRUMBLY_WOODS,             "Crumbly Woods",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 15   },
        { SC_PARADISE_PIER,             "Paradise Pier",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1 + 16   },
        { SC_LIGHTNING_PEAKS,           "Lightning Peaks",      SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1 + 17   },
        { SC_IVORY_TOWERS,              "Ivory Towers",         SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1 + 18   },
        { SC_RAINBOW_VALLEY,            "Rainbow Valley",       SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1 + 19   },
        { SC_THUNDER_ROCK,              "Thunder Rock",         SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1 + 20   },
        { SC_MEGA_PARK,                 "Mega Park",            SCENARIO_CATEGORY_OTHER,         SC_ADV_INDEX_NONE        },
    };

    // RCT: Added Attractions
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRCT1AA[] =
    {
        { SC_WHISPERING_CLIFFS,         "Whispering Cliffs",    SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 0     },
        { SC_THREE_MONKEYS_PARK,        "Three Monkeys Park",   SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 1     },
        { SC_CANARY_MINES,              "Canary Mines",         SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 2     },
        { SC_BARONY_BRIDGE,             "Barony Bridge",        SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 3     },
        { SC_FUNTOPIA,                  "Funtopia",             SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 4     },
        { SC_HAUNTED_HARBOUR,           "Haunted Harbour",      SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 5     },
        { SC_FUN_FORTRESS,              "Fun Fortress",         SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 6     },
        { SC_FUTURE_WORLD,              "Future World",         SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 7     },
        { SC_GENTLE_GLEN,               "Gentle Glen",          SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1AA + 8     },
        { SC_JOLLY_JUNGLE,              "Jolly Jungle",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 9     },
        { SC_HYDRO_HILLS,               "Hydro Hills",          SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 10    },
        { SC_SPRIGHTLY_PARK,            "Sprightly Park",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 11    },
        { SC_MAGIC_QUARTERS,            "Magic Quarters",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 12    },
        { SC_FRUIT_FARM,                "Fruit Farm",           SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 13    },
        { SC_BUTTERFLY_DAM,             "Butterfly Dam",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 14    },
        { SC_COASTER_CANYON,            "Coaster Canyon",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 15    },
        { SC_THUNDERSTORM_PARK,         "Thunderstorm Park",    SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 16    },
        { SC_HARMONIC_HILLS,            "Harmonic Hills",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 17    },
        { SC_ROMAN_VILLAGE,             "Roman Village",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 18    },
        { SC_SWAMP_COVE,                "Swamp Cove",           SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 19    },
        { SC_ADRENALINE_HEIGHTS,        "Adrenaline Heights",   SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 20    },
        { SC_UTOPIA,                    "Utopia",               SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1AA + 21    },
        { SC_ROTTING_HEIGHTS,           "Rotting Heights",      SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 22    },
        { SC_FIASCO_FOREST,             "Fiasco Forest",        SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 23    },
        { SC_PICKLE_PARK,               "Pickle Park",          SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 24    },
        { SC_GIGGLE_DOWNS,              "Giggle Downs",         SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 25    },
        { SC_MINERAL_PARK,              "Mineral Park",         SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 26    },
        { SC_COASTER_CRAZY,             "Coaster Crazy",        SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 27    },
        { SC_URBAN_PARK,                "Urban Park",           SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 28    },
        { SC_GEOFFREY_GARDENS,          "Geoffrey Gardens",     SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1AA + 29    },
    };

    // RCT: Loopy Landscapes
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRCT1LL[] =
    {
        {   SC_ICEBERG_ISLANDS,         "Iceberg Islands",      SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1LL + 0     },
        {   SC_VOLCANIA,                "Volcania",             SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1LL + 1     },
        {   SC_ARID_HEIGHTS,            "Arid Heights",         SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1LL + 2     },
        {   SC_RAZOR_ROCKS,             "Razor Rocks",          SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1LL + 3     },
        {   SC_CRATER_LAKE,             "Crater Lake",          SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1LL + 4     },
        {   SC_VERTIGO_VIEWS,           "Vertigo Views",        SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT1LL + 5     },
        {   SC_PARADISE_PIER_2,         "Paradise Pier 2",      SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 6     },
        {   SC_DRAGONS_COVE,            "Dragon's Cove",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 7     },
        {   SC_GOOD_KNIGHT_PARK,        "Good Knight Park",     SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 8     },
        {   SC_WACKY_WARREN,            "Wacky Warren",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 9     },
        {   SC_GRAND_GLACIER,           "Grand Glacier",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 10    },
        {   SC_CRAZY_CRATERS,           "Crazy Craters",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 11    },
        {   SC_DUSTY_DESERT,            "Dusty Desert",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 12    },
        {   SC_WOODWORM_PARK,           "Woodworm Park",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 13    },
        {   SC_ICARUS_PARK,             "Icarus Park",          SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 14    },
        {   SC_SUNNY_SWAMPS,            "Sunny Swamps",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 15    },
        {   SC_FRIGHTMARE_HILLS,        "Frightmare Hills",     SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 16    },
        {   SC_THUNDER_ROCKS,           "Thunder Rocks",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 17    },
        {   SC_OCTAGON_PARK,            "Octagon Park",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 18    },
        {   SC_PLEASURE_ISLAND,         "Pleasure Island",      SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 19    },
        {   SC_ICICLE_WORLDS,           "Icicle Worlds",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 20    },
        {   SC_SOUTHERN_SANDS,          "Southern Sands",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 21    },
        {   SC_TINY_TOWERS,             "Tiny Towers",          SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 22    },
        {   SC_NEVERMORE_PARK,          "Nevermore Park",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 23    },
        {   SC_PACIFICA,                "Pacifica",             SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT1LL + 24    },
        {   SC_URBAN_JUNGLE,            "Urban Jungle",         SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1LL + 25    },
        {   SC_TERROR_TOWN,             "Terror Town",          SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1LL + 26    },
        {   SC_MEGAWORLD_PARK,          "Megaworld Park",       SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1LL + 27    },
        {   SC_VENUS_PONDS,             "Venus Ponds",          SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1LL + 28    },
        {   SC_MICRO_PARK,              "Micro Park",           SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT1LL + 29    },
    };

    // RCT2
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRCT2[] =
    {
        { SC_UNIDENTIFIED,              "Crazy Castle",         SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT2 + 0       },
        { SC_UNIDENTIFIED,              "Electric Fields",      SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT2 + 1       },
        { SC_UNIDENTIFIED,              "Factory Capers",       SCENARIO_CATEGORY_BEGINNER,      SC_ADV_INDEX_RCT2 + 2       },
        { SC_UNIDENTIFIED,              "Amity Airfield",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 3       },
        { SC_UNIDENTIFIED,              "Botany Breakers",      SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 4       },
        { SC_UNIDENTIFIED,              "Bumbly Bazaar",        SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 5       },
        { SC_UNIDENTIFIED,              "Dusty Greens",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 6       },
        { SC_UNIDENTIFIED,              "Fungus Woods",         SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 7       },
        { SC_UNIDENTIFIED,              "Gravity Gardens",      SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 8       },
        { SC_UNIDENTIFIED,              "Infernal Views",       SCENARIO_CATEGORY_CHALLENGING,   SC_ADV_INDEX_RCT2 + 9       },
        { SC_UNIDENTIFIED,              "Alpine Adventures",    SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT2 + 10      },
        { SC_UNIDENTIFIED,              "Extreme Heights",      SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT2 + 11      },
        { SC_UNIDENTIFIED,              "Ghost Town",           SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT2 + 12      },
        { SC_UNIDENTIFIED,              "Lucky Lake",           SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT2 + 13      },
        { SC_UNIDENTIFIED,              "Rainbow Summit",       SCENARIO_CATEGORY_EXPERT,        SC_ADV_INDEX_RCT2 + 14      },
    };

    // RCT2: Wacky Worlds
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRCT2WW[] =
    {
        { SC_UNIDENTIFIED,              "Africa - Victoria Falls",                          SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2WW + 0            },
        { SC_UNIDENTIFIED,              "Asia - Great Wall of China Tourism Enhancement",   SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2WW + 1            },
        { SC_UNIDENTIFIED,              "North America - Grand Canyon",                     SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2WW + 2            },
        { SC_UNIDENTIFIED,              "South America - Rio Carnival",                     SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2WW + 3            },
        { SC_UNIDENTIFIED,              "Africa - African Diamond Mine",                    SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2WW + 4            },
        { SC_UNIDENTIFIED,              "Asia - Maharaja Palace",                           SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2WW + 5            },
        { SC_UNIDENTIFIED,              "Australasia - Ayers Rock",                         SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2WW + 6            },
        { SC_UNIDENTIFIED,              "Europe - European Cultural Festival",              SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2WW + 7            },
        { SC_UNIDENTIFIED,              "North America - Rollercoaster Heaven",             SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2WW + 8            },
        { SC_UNIDENTIFIED,              "South America - Inca Lost City",                   SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2WW + 9            },
        { SC_UNIDENTIFIED,              "Africa - Oasis",                                   SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 10           },
        { SC_UNIDENTIFIED,              "Antarctic - Ecological Salvage",                   SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 11           },
        { SC_UNIDENTIFIED,              "Asia - Japanese Coastal Reclaim",                  SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 12           },
        { SC_UNIDENTIFIED,              "Australasia - Fun at the Beach",                   SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 13           },
        { SC_UNIDENTIFIED,              "Europe - Renovation",                              SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 14           },
        { SC_UNIDENTIFIED,              "N. America - Extreme Hawaiian Island",             SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 15           },
        { SC_UNIDENTIFIED,              "South America - Rain Forest Plateau",              SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2WW + 16           },
    };

    // RCT2: Time Twister
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRCT2TT[] =
    {
        { SC_UNIDENTIFIED,              "Dark Age - Robin Hood",                            SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2TT + 0            },
        { SC_UNIDENTIFIED,              "Prehistoric - After the Asteroid",                 SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2TT + 1            },
        { SC_UNIDENTIFIED,              "Roaring Twenties - Prison Island",                 SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2TT + 2            },
        { SC_UNIDENTIFIED,              "Rock 'n' Roll - Flower Power",                     SCENARIO_CATEGORY_BEGINNER,         SC_ADV_INDEX_RCT2TT + 3            },
        { SC_UNIDENTIFIED,              "Dark Age - Castle",                                SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2TT + 4            },
        { SC_UNIDENTIFIED,              "Future - First Encounters",                        SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2TT + 5            },
        { SC_UNIDENTIFIED,              "Mythological - Animatronic Film Set",              SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2TT + 6            },
        { SC_UNIDENTIFIED,              "Prehistoric - Jurassic Safari",                    SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2TT + 7            },
        { SC_UNIDENTIFIED,              "Roaring Twenties - Schneider Cup",                 SCENARIO_CATEGORY_CHALLENGING,      SC_ADV_INDEX_RCT2TT + 8            },
        { SC_UNIDENTIFIED,              "Future - Future World",                            SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2TT + 9            },
        { SC_UNIDENTIFIED,              "Mythological - Cradle of Civilisation",            SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2TT + 12           },
        { SC_UNIDENTIFIED,              "Prehistoric - Stone Age",                          SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2TT + 13           },
        { SC_UNIDENTIFIED,              "Roaring Twenties - Skyscrapers",                   SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2TT + 14           },
        { SC_UNIDENTIFIED,              "Rock 'n' Roll - Rock 'n' Roll",                    SCENARIO_CATEGORY_EXPERT,           SC_ADV_INDEX_RCT2TT + 15           },
    };      

    // Real parks
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesRealParks[] =
    {
        { SC_ALTON_TOWERS,              "Alton Towers",                                     SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT1_REAL_PARKS + 0   },
        { SC_HEIDE_PARK,                "Heide-Park",                                       SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT1_REAL_PARKS + 1   },
        { SC_BLACKPOOL_PLEASURE_BEACH,  "Blackpool Pleasure Beach",                         SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT1_REAL_PARKS + 2   },
        { SC_UNIDENTIFIED,              "Six Flags Belgium",                                SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT2_REAL_PARKS + 0   },
        { SC_UNIDENTIFIED,              "Six Flags Great Adventure",                        SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT2_REAL_PARKS + 1   },
        { SC_UNIDENTIFIED,              "Six Flags Holland",                                SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT2_REAL_PARKS + 2   },
        { SC_UNIDENTIFIED,              "Six Flags Magic Mountain",                         SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT2_REAL_PARKS + 3   },
        { SC_UNIDENTIFIED,              "Six Flags over Texas",                             SCENARIO_CATEGORY_REAL,             SC_ADV_INDEX_RCT2_REAL_PARKS + 4   },
    };  

    // Other parks
    static constexpr const ScenarioTitleDescriptor ScenarioTitlesOtherParks[] =
    {
        { SC_FORT_ANACHRONISM,          "Fort Anachronism",                                 SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_PCPLAYER,                  "PC Player",                                        SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_PCGW,                      "PC Gaming World",                                  SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_GAMEPLAY,                  "gameplay",                                         SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_UNIDENTIFIED,              "Panda World",                                      SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_UNIDENTIFIED,              "Competition Land 1",                               SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_UNIDENTIFIED,              "Competition Land 2",                               SCENARIO_CATEGORY_DLC,              SC_ADV_INDEX_NONE                  },
        { SC_UNIDENTIFIED,              "Build your own Six Flags Belgium",                 SCENARIO_CATEGORY_BUILD_YOUR_OWN,   SC_ADV_INDEX_RCT2_OTHER_PARKS + 0  },
        { SC_UNIDENTIFIED,              "Build your own Six Flags Great Adventure",         SCENARIO_CATEGORY_BUILD_YOUR_OWN,   SC_ADV_INDEX_RCT2_OTHER_PARKS + 1  },
        { SC_UNIDENTIFIED,              "Build your own Six Flags Holland",                 SCENARIO_CATEGORY_BUILD_YOUR_OWN,   SC_ADV_INDEX_RCT2_OTHER_PARKS + 2  },
        { SC_UNIDENTIFIED,              "Build your own Six Flags Magic Mountain",          SCENARIO_CATEGORY_BUILD_YOUR_OWN,   SC_ADV_INDEX_RCT2_OTHER_PARKS + 3  },
        { SC_UNIDENTIFIED,              "Build your own Six Flags Park",                    SCENARIO_CATEGORY_BUILD_YOUR_OWN,   SC_ADV_INDEX_RCT2_OTHER_PARKS + 4  },
        { SC_UNIDENTIFIED,              "Build your own Six Flags over Texas",              SCENARIO_CATEGORY_BUILD_YOUR_OWN,   SC_ADV_INDEX_RCT2_OTHER_PARKS + 5  },
    };     
    
    #define DEFINE_SCENARIO_TITLE_DESC_GROUP(x) { Util::CountOf(x), x }
    const struct {
        size_t count;
        const ScenarioTitleDescriptor * const titles;
    } ScenarioTitlesBySource[] = {
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRCT1),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRCT1AA),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRCT1LL),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRCT2),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRCT2WW),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRCT2TT),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesRealParks),
        DEFINE_SCENARIO_TITLE_DESC_GROUP(ScenarioTitlesOtherParks),
    };
    // clang-format on

    #pragma endregion

    bool TryGetByName(const utf8 * name, source_desc * outDesc)
    {
        Guard::ArgumentNotNull(outDesc, GUARD_LINE);

        int32_t currentIndex = 0;
        for (size_t i = 0; i < Util::CountOf(ScenarioTitlesBySource); i++)
        {
            for (size_t j = 0; j < ScenarioTitlesBySource[i].count; j++)
            {
                const ScenarioTitleDescriptor *desc = &ScenarioTitlesBySource[i].titles[j];
                if (String::Equals(name, desc->Title, true))
                {
                    outDesc->title = desc->Title;
                    outDesc->id = desc->Id;
                    outDesc->source = (uint8_t)i;
                    outDesc->index = currentIndex;
                    outDesc->category = desc->Category;
                    return true;
                }
                currentIndex++;
            }
        }

        outDesc->title = nullptr;
        outDesc->id = SC_UNIDENTIFIED;
        outDesc->source = SCENARIO_SOURCE_OTHER;
        outDesc->index = -1;
        outDesc->category = SCENARIO_CATEGORY_OTHER;
        return false;
    }

    bool TryGetById(uint8_t id, source_desc * outDesc)
    {
        Guard::ArgumentNotNull(outDesc, GUARD_LINE);

        int32_t currentIndex = 0;
        for (size_t i = 0; i < Util::CountOf(ScenarioTitlesBySource); i++)
        {
            for (size_t j = 0; j < ScenarioTitlesBySource[i].count; j++)
            {
                const ScenarioTitleDescriptor * desc = &ScenarioTitlesBySource[i].titles[j];
                if (id == desc->Id)
                {
                    outDesc->title = desc->Title;
                    outDesc->id = desc->Id;
                    outDesc->source = (uint8_t)i;
                    outDesc->index = currentIndex;
                    outDesc->category = desc->Category;
                    return true;
                }
                currentIndex++;
            }
        }

        outDesc->title = "";
        outDesc->id = SC_UNIDENTIFIED;
        outDesc->source = SCENARIO_SOURCE_OTHER;
        outDesc->index = -1;
        outDesc->category = SCENARIO_CATEGORY_OTHER;
        return false;
    }

    void NormaliseName(utf8 * buffer, size_t bufferSize, const utf8 * name)
    {
        size_t nameLength = String::LengthOf(name);

        // Strip "RCT(1|2)?" prefix off scenario names.
        if (nameLength >= 3 && (name[0] == 'R' && name[1] == 'C' && name[2] == 'T'))
        {
            if (nameLength >= 4 && (name[3] == '1' || name[3] == '2'))
            {
                log_verbose("Stripping RCT/1/2 from name: %s", name);
                String::Set(buffer, bufferSize, name + 4);
            }
            else
            {
                String::Set(buffer, bufferSize, name + 3);
            }
        }

        // Trim (for the sake of the above and WW / TT scenarios
        String::TrimStart(buffer, bufferSize, name);

        // American scenario titles should be converted to British name
        // Don't worry, names will be translated using language packs later
        for (const ScenarioAlias &alias : ScenarioAliases)
        {
            if (String::Equals(alias.Alternative, name))
            {
                log_verbose("Found alias: %s; will treat as: %s", name, alias.Original);
                String::Set(buffer, bufferSize, alias.Original);
            }
        }
    }
} // namespace ScenarioSources

bool scenario_get_source_desc(const utf8 * name, source_desc * outDesc)
{
    return ScenarioSources::TryGetByName(name, outDesc);
}

bool scenario_get_source_desc_by_id(uint8_t id, source_desc * outDesc)
{
    return ScenarioSources::TryGetById(id, outDesc);
}

void scenario_normalise_name(utf8 * buffer, size_t bufferSize, utf8 * name)
{
    ScenarioSources::NormaliseName(buffer, bufferSize, name);
}

